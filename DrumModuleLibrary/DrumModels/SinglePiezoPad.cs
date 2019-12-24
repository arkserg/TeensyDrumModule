using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    [DrumType(DrumType.SinglePiezoPad)]
    public class SinglePiezoPad : DrumPad
    {
        [DataMember]
        public byte PadNote { get; set; }

        [DataMember]
        public Int16 ThresholdMin { get; set; }

        [DataMember]
        public Int16 ThresholdMax { get; set; }

        [DataMember]
        public byte ScanTime { get; set; }

        [DataMember]
        public byte MaskTime { get; set; }

        [DataMember]
        public byte Amplification { get; set; }

        protected SinglePiezoPad()
        {
        }

        protected SinglePiezoPad(DrumType type, byte channel) : base(type, channel)
        {
        }

        public SinglePiezoPad(byte channel) : base(DrumType.SinglePiezoPad, channel)
        {
        }

        public override void ResetToDefault()
        {
            base.ResetToDefault();
            PadNote = 38;
            ThresholdMin = 20;
            ThresholdMax = 750;
            ScanTime = 3;
            MaskTime = 70;
            Amplification = 20;
        }
    }
}