using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    [DrumType(DrumType.ThreeZoneCymbal)]
    public class ThreeZoneCymbal : DualZoneCymbal
    {
        [DataMember]
        public byte BellNote { get; set; }

        protected ThreeZoneCymbal(DrumType type, byte channel) : base(type, channel)
        {
        }

        public ThreeZoneCymbal(byte channel) : base(DrumType.ThreeZoneCymbal, channel)
        {
        }

        public override void ResetToDefault()
        {
            base.ResetToDefault();
            PadNote = 51;
            BellNote = 53;
            EdgeNote = 59;
        }
    }
}