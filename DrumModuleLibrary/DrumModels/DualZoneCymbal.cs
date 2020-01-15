using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    [DrumType(DrumType.DualZoneCymbal)]
    public class DualZoneCymbal : SinglePiezoPad
    {
        [DataMember]
        public byte EdgeNote { get; set; }

        [DataMember]
        public byte ChokeTimeThreshold { get; set; }

        [DataMember]
        public bool ChokeEnabled { get; set; }

        protected DualZoneCymbal()
        {
        }

        protected DualZoneCymbal(DrumType type, byte channel) : base(type, channel)
        {
        }

        public DualZoneCymbal(byte channel) : base(DrumType.DualZoneCymbal, channel)
        {
        }

        public override void ResetToDefault()
        {
            base.ResetToDefault();
            PadNote = 51;
            EdgeNote = 53;
            ChokeTimeThreshold = 3;
            ChokeEnabled = true;
        }
    }
}