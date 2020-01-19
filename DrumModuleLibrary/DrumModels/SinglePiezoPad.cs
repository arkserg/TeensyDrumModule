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
        public Int16 Scan { get; set; }

        [DataMember]
        public Int16 Hold { get; set; }

        [DataMember]
        public Int16 Decay { get; set; }

        [DataMember]
        public byte Gain { get; set; }

        [DataMember]
        public ScaleType Scale { get; set; }

        [DataMember]
        public byte Lift { get; set; }

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
            Scan = 3;
            Hold = 70;
            Decay = 200;
            Gain = 20;
            Scale = ScaleType.Linear;
            Lift = 40;
        }
    }
}