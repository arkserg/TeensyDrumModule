using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
    internal class CommandTypeAttribute : Attribute
    {
        public CommandType CommandType { get; set; }

        public CommandTypeAttribute(CommandType type)
        {
            CommandType = type;
        }
    }
}