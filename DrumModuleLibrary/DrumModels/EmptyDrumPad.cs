using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    [DrumType(DrumType.EmptyDrumPad)]
    public class EmptyDrumPad : DrumPad
    {
        protected EmptyDrumPad()
        {
        }       

        public EmptyDrumPad(byte channel) : base(DrumType.EmptyDrumPad, channel)
        {
        }
    }
}