using System;
using MegaDrum;
using MegaDrum.Commands;
using MegaDrum.DrumModels;
using Newtonsoft.Json;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Logging.Console;
using System.Threading.Tasks;
using MegaDrum.Enums;
using System.Linq;

namespace TestConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            MegaDrumTest();
        }       

        static void MegaDrumTest()
        {
            using (var megaDrum = new MegaDrumHelper("COM3", 115200, Console.Out, new ConsoleLogger<MegaDrumHelper>()))
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
                        case 0:
                            PingAsync(megaDrum).Wait();
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
            Console.WriteLine("0. Ping");
            //Console.WriteLine("1. Add new drum");
            //Console.WriteLine("2. Remove drum by id");
            //Console.WriteLine("3. Update drum by id");
            //Console.WriteLine("4. Remove all drums");
            //Console.WriteLine("5. Save settings to EEPROM");
            //Console.WriteLine("6. Load settings from EEPROM");
            Console.WriteLine("9. Exit");
        }

        private static void WaitUserInput()
        {
            while (!Console.KeyAvailable) { }
        }

        private static async Task PingAsync(MegaDrumHelper megaDrum)
        {
            var command = new GetAllDrumsCommand();
            var result = await megaDrum.InvokeCommandAsync(command);

            if (result.ResponseType == CommandResponseType.Success)
                Console.WriteLine(Serialize(result.Data));
        }        
    }

    public class ConsoleLogger<T> : ConsoleLogger, ILogger<T>
    {
        public ConsoleLogger()
            : base(typeof(T).Name, (m, l) => true, true)
        {
        }
    }    
}