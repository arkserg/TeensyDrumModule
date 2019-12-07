using System;
using System.Threading;
using System.Threading.Tasks;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary
{
    public class CommandResultAwaiter : IDisposable
    {
        public readonly int CommandId;
        private readonly SemaphoreSlim _semaphore;
        private readonly TimeSpan _timeout;
        private DrumModuleCommandResponse _commandResponse;

        public CommandResultAwaiter(int commandId, int timeout = 5000)
        {
            CommandId = commandId;
            _semaphore = new SemaphoreSlim(0, 1);
            _timeout = TimeSpan.FromMilliseconds(timeout);
        }

        public async Task<DrumModuleCommandResponse> WaitAsync()
        {
            using (var cts = new CancellationTokenSource(_timeout))
            {
                await _semaphore.WaitAsync(cts.Token);
                return _commandResponse;
            }         
        }

        public void SetResult(DrumModuleCommandResponse response)
        {
            _commandResponse = response;
            _semaphore.Release();
        }

        public void Dispose()
        {
            _semaphore?.Dispose();
        }
    }
}