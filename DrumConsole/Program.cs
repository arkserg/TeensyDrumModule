using System;
using Arkserg.TeensyDrumModule.DrumModuleLibrary;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands;
using Microsoft.Extensions.Logging;
using System.Threading.Tasks;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels;
using System.Collections.Generic;
using Newtonsoft.Json;

namespace Arkserg.TeensyDrumModule.DrumConsole
{
    class Program
    {
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
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(1)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(2)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(3)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(4)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(5)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(6)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(7)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(8)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(9)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(10)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
                        ThresholdMin = 12, ThresholdMax = 700,
                    },
                    new ThreeZoneCymbal(11)
                    {
                        Name = "Second crash cymbal",
                        PadNote = 12, BellNote = 14, EdgeNote = 15,
                        Scan = 1, Hold = 3,
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
                    Scan = 1,
                    Hold = 3,
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
            var logger = loggerFactory.CreateLogger<DrumModuleConnection>();

            using (var megaDrum = new DrumModuleConnection("COM3", 115200, logger))
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
                        case 6:
                            EnableDrumAsync(megaDrum).Wait();
                            break;
                        case 7:
                            DisableDrumAsync(megaDrum).Wait();
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
            Console.WriteLine("6. Enable drum");
            Console.WriteLine("7. Disable drum");
            Console.WriteLine("8. Delete all drums");

            Console.WriteLine("9. Exit");
        }

        private static void WaitUserInput()
        {
            while (!Console.KeyAvailable) { }
        }

        private static async Task PingAsync(DrumModuleConnection megaDrum)
        {
            var result = await megaDrum.PingAsync();
            Console.WriteLine(result ? "Success" : "Error");
        }

        private static async Task GetAllDrumsAsync(DrumModuleConnection megaDrum)
        {
            var result = await megaDrum.GetAllDrumsAsync();
            if (result == null) Console.WriteLine("Error");
            Console.WriteLine(JsonConvert.SerializeObject(result, Formatting.Indented));
        }

        private static async Task SetDrumParametersAsync(DrumModuleConnection megaDrum)
        {
            var drum = new SinglePiezoPad(0)
            {
                Name = "Snare",
                PadNote = 38,
                Scan = 3,
                Hold = 70,
                ThresholdMin = 30,
                ThresholdMax = 1023,
                Enabled = true,
                Gain = 10
            };

            var response = await megaDrum.SetDrumParametersAsync(drum);

            if (response == null)
                Console.WriteLine("Error");

            Console.WriteLine(JsonConvert.SerializeObject(response, Formatting.Indented));
        }

        private static async Task SaveSettingsAsync(DrumModuleConnection megaDrum)
        {
            var result = await megaDrum.SaveSettingsAsync();
            Console.WriteLine(result ? "Success" : "Error");
        }

        private static async Task ReloadSettingsAsync(DrumModuleConnection megaDrum)
        {
            var result = await megaDrum.ReloadSettingsAsync();
            Console.WriteLine(result ? "Success" : "Error");
        }

        private static async Task EnableDrumAsync(DrumModuleConnection megaDrum)
        {
            var result = await megaDrum.EnableDrumAsync(0);
            Console.WriteLine(result ? "Success" : "Error");
        }

        private static async Task DisableDrumAsync(DrumModuleConnection megaDrum)
        {
            var result = await megaDrum.DisableDrumAsync(0);
            Console.WriteLine(result ? "Success" : "Error");
        }
    }    
}