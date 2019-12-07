using System;
using System.Buffers;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.IO.Ports;
using System.IO;
using System.IO.Pipelines;
using System.Threading;
using Nerdbank.Streams;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Logging.Abstractions;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands;
using Newtonsoft.Json;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Json;
using System.Text;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary
{
    public class MegaDrumHelper : IDisposable
    {
        private readonly SerialPort _port;
        private readonly Dictionary<int, CommandResultAwaiter> _responses;
        private readonly ILogger<MegaDrumHelper> _log;
        private readonly JsonSerializer _serializer;
        private readonly JsonSerializerSettings _serializerSettings;
        private CancellationTokenSource _cts;
        private Pipe _pipe;

        public MegaDrumHelper(string portName, int baudRate, ILogger<MegaDrumHelper> log = null)
        {
            _port = new SerialPort(portName, baudRate);
            _port.Parity = Parity.None;
            _port.DataBits = 8;
            _port.StopBits = StopBits.Two;
            _port.DataReceived += Port_DataReceived;
            _port.DtrEnable = true;
            _port.RtsEnable = true;
            _responses = new Dictionary<int, CommandResultAwaiter>();
            _serializer = new JsonSerializer
            {
                TypeNameHandling = TypeNameHandling.None,
                Converters =
                {
                    new DrumPadJsonConverter(),
                    new CommandResponseJsonConverter(),
                }
            };
            _serializerSettings = new JsonSerializerSettings
            {
                TypeNameHandling = TypeNameHandling.None,
                Converters =
                {
                    new DrumPadJsonConverter(),
                    new CommandResponseJsonConverter(),
                }
            };
            _log = log ?? new NullLogger<MegaDrumHelper>();
        }

        public void OpenConnection()
        {
            _cts = new CancellationTokenSource();
            _port.Open();
            _pipe = new Pipe();
            var task = ReadPipeAsync(_cts.Token);
        }

        public void CloseConnection()
        {
            _cts.Cancel();
            _port.Close();
            _pipe = null;
        }

        public async Task<TResponse> InvokeCommandAsync<TResponse>(DrumModuleCommand<TResponse> command)
            where TResponse : DrumModuleCommandResponse
        {
            var response = new CommandResultAwaiter(command.CommandId);
            _responses.Add(command.CommandId, response);

            var str = JsonConvert.SerializeObject(command, _serializerSettings);
            var bytes = Encoding.UTF8.GetBytes(str);
            _port.Write(bytes, 0, bytes.Length);

            try
            {
                var result = await response.WaitAsync().ConfigureAwait(false);
                return (TResponse) result; //todo: type check
            }
            catch(OperationCanceledException)
            {
                return null;
            }
            finally
            {
                if(_responses.TryGetValue(command.CommandId, out var responseAwaiter))
                {
                    _responses.Remove(command.CommandId);
                    responseAwaiter.Dispose();
                }
            }
        }

        private void Port_DataReceived(object sender,
            SerialDataReceivedEventArgs e)
        {
            var task = FillPipeAsync(_cts.Token);
        }

        private async Task FillPipeAsync(CancellationToken cancellation)
        {
            try
            {
                var buffer = new byte[_port.BytesToRead];
                await _port.BaseStream.ReadAsync(buffer, 0, _port.BytesToRead, cancellation).ConfigureAwait(false);
                var result = await _pipe.Writer.WriteAsync(buffer, cancellation).ConfigureAwait(false);
            }
            catch (Exception e)
            {
                _log.LogError(e, "FillPipeAsync error");
            }            
        }

        async Task ReadPipeAsync(CancellationToken cancellation)
        {
            try
            {
                while (_port.IsOpen && !cancellation.IsCancellationRequested)
                {
                    var result = await _pipe.Reader.ReadAsync(cancellation).ConfigureAwait(false);
                    var buffer = result.Buffer;
                    var bytesConsumed = HandleInput(buffer);
                    var consumed = bytesConsumed > 0 ? buffer.GetPosition(bytesConsumed) : buffer.Start;
                    _pipe.Reader.AdvanceTo(consumed, buffer.End);
                }
            }
            catch (Exception e)
            {
                _log.LogError(e, "ReadPipeAsync error");
            }
        }
        
        private long HandleInput(ReadOnlySequence<byte> buffer)
        {
            using(var stream = buffer.AsStream())
            {
                DrumModuleCommandResponse response;
                try
                {
                    response = DeserializeFromStream<DrumModuleCommandResponse>(stream);
                }
                catch (Exception)
                {
                    return 0;
                }

                if (_responses.TryGetValue(response.CommandId, out var resoponseAwaiter))
                {
                    resoponseAwaiter.SetResult(response);
                }
                else
                {
                    _log.LogError($"Unexpected command response.{Environment.NewLine}{JsonConvert.SerializeObject(response)}");
                }

                return stream.Position;
            }            
        }

        public T DeserializeFromStream<T>(Stream stream)
        {
            using (var sr = new StreamReader(stream))
            using (var jsonTextReader = new JsonTextReader(sr))
            {
                return _serializer.Deserialize<T>(jsonTextReader);
            }
        }

        public void Dispose()
        {
            if (_port != null)
            {
                if (_port.IsOpen)
                    _port.Close();
                _port?.Dispose();
                _cts?.Dispose();
            }
            _cts?.Dispose();
        }
    }   
}
