using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    [DrumType(DrumType.DualPiezoPad)]
    public class DualPiezoPad : SinglePiezoPad
    {
        [DataMember]
        public Int16 SecondZoneThresholdMin { get; set; }

        [DataMember]
        public Int16 SecondZoneThresholdMax { get; set; }

        [DataMember]
        public byte SecondZoneSensorScantime { get; set; }

        [DataMember]
        public byte SecondZoneSensorMasktime { get; set; }

        [DataMember]
        public byte SecondZoneNote { get; set; }

        protected DualPiezoPad()
        {
        }

        protected DualPiezoPad(DrumType type, byte channel) : base(type, channel)
        {
        }

        public DualPiezoPad(byte channel) : base(DrumType.DualPiezoPad, channel)
        {
        }
    }
}