using System;
using Arkserg.TeensyDrumModule.DrumModuleLibrary;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands;
using Microsoft.Extensions.Logging;
using System.Threading.Tasks;
using System.Threading;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels;
using System.Collections.Generic;
using Newtonsoft.Json;

namespace Arkserg.TeensyDrumModule.DrumConsole
{
    class Program
    {
        private static int CommandId; //todo: не должно быть здесь

        static void Main(string[] args)
        {
            var response2 = new GetAllDrumsCommandResponse(178892)
            {
                Drums = new List<DrumPad>
                {
                    new ThreeZoneCymbal(0)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(1)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(2)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(3)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(4)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(5)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(6)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(7)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(8)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(9)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(10)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(11)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        SensorScantime = 1, SensorMasktime = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                }
            };

            var response = new SetDrumParametersCommandResponse(65431)
            {
                Drum = new ThreeZoneCymbal(0)
                {
                    Name = "Second crash cymbal",
                    PadNote = 12,
                    BellNote = 14,
                    EdgeNote = 15,
                    SensorScantime = 1,
                    SensorMasktime = 3,
                    ThresholdMin = 12,
                    ThresholdMax = 700,
                }
            };

            var settings = new JsonSerializerSettings { TypeNameHandling = TypeNameHandling.None };
            var str = JsonConvert.SerializeObject(response);


            MegaDrumTest();
        }

        static void MegaDrumTest()
        {
            var loggerFactory = LoggerFactory.Create(b => b.AddConsole());
            var logger = loggerFactory.CreateLogger<MegaDrumHelper>();

            using (var megaDrum = new MegaDrumHelper("COM3", 115200, logger))
            {
                megaDrum.OpenConnection();
                bool loopFlag = true;

                while (loopFlag)
                {
                    Console.Clear();
                    PrintMenu();
                    WaitUserInput();

                    var input = Console.ReadLine();

                    if (!int.TryParse(input, out var choice))
                        continue;

                    switch (choice)
                    {
                        case 1:
                            PingAsync(megaDrum).Wait();
                            break;
                        case 2:
                            GetAllDrumsAsync(megaDrum).Wait();
                            break;
                        case 3:
                            SetDrumParametersAsync(megaDrum).Wait();
                            break;
                        case 4:
                            SaveSettingsAsync(megaDrum).Wait();
                            break;
                        case 5:
                            ReloadSettingsAsync(megaDrum).Wait();
                            break;
                        case 9:
                            loopFlag = false;
                            continue;
                        default:
                            continue;
                    }
                    Console.ReadKey();
                }

                Console.ReadKey();
                Console.WriteLine("GoodBye");
            }
        }

        private static void PrintMenu()
        {
            Console.WriteLine("Enter command:");
            Console.WriteLine("1. Ping");
            Console.WriteLine("2. Get all drums");
            Console.WriteLine("3. Set drum parameters");
            Console.WriteLine("4. Save settings to EEPROM");
            Console.WriteLine("5. Reload settings from EEPROM");

            Console.WriteLine("9. Exit");
        }

        private static void WaitUserInput()
        {
            while (!Console.KeyAvailable) { }
        }

        private static async Task PingAsync(MegaDrumHelper megaDrum)
        {
            var command = new PingCommand(Interlocked.Increment(ref CommandId));
            var result = await megaDrum.InvokeCommandAsync(command);

            var message = result != null ? "Success" : "Error";

            Console.WriteLine(message);
        }

        private static async Task GetAllDrumsAsync(MegaDrumHelper megaDrum)
        {
            var command = new GetAllDrumsCommand(Interlocked.Increment(ref CommandId));
            var response = await megaDrum.InvokeCommandAsync(command);

            if (response == null)
                Console.WriteLine("Error");

            Console.WriteLine(JsonConvert.SerializeObject(response, Formatting.Indented));
        }

        private static async Task SetDrumParametersAsync(MegaDrumHelper megaDrum)
        {
            var command = new SetDrumParametersCommand(Interlocked.Increment(ref CommandId))
            {
                Drum = new ThreeZoneCymbal(0)
                {
                    Name = "Completely new drum",
                    PadNote = 12,
                    BellNote = 14,
                    EdgeNote = 15,
                    SensorScantime = 1,
                    SensorMasktime = 70,
                    ThresholdMin = 17,
                    ThresholdMax = 717,
                }
            };
            var response = await megaDrum.InvokeCommandAsync(command);

            if (response == null)
                Console.WriteLine("Error");

            Console.WriteLine(JsonConvert.SerializeObject(response, Formatting.Indented));
        }

        private static async Task SaveSettingsAsync(MegaDrumHelper megaDrum)
        {
            var command = new SaveSettingsCommand(Interlocked.Increment(ref CommandId));
            var result = await megaDrum.InvokeCommandAsync(command);

            var message = result != null ? "Success" : "Error";

            Console.WriteLine(message);
        }

        private static async Task ReloadSettingsAsync(MegaDrumHelper megaDrum)
        {
            var command = new ReloadSettingsCommand(Interlocked.Increment(ref CommandId));
            var result = await megaDrum.InvokeCommandAsync(command);

            var message = result != null ? "Success" : "Error";

            Console.WriteLine(message);
        }
    }    
}