using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
    internal class DrumTypeAttribute : Attribute
    {
        public DrumType DrumType { get; set; }

        public DrumTypeAttribute(DrumType type)
        {
            DrumType = type;
        }
    }
}