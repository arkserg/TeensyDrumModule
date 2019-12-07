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
        public byte SensorScantime { get; set; }

        [DataMember]
        public byte SensorMasktime { get; set; }

        protected SinglePiezoPad()
        {
        }

        protected SinglePiezoPad(DrumType type, byte channel) : base(type, channel)
        {
        }

        public SinglePiezoPad(byte channel) : base(DrumType.SinglePiezoPad, channel)
        {
        }
    }
}