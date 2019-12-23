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
        public byte SideClickNote { get; set; }

        [DataMember]
        public byte RimShotNote { get; set; }

        [DataMember]
        public Int16 SecondZoneThresholdMin { get; set; }

        [DataMember]
        public Int16 SecondZoneThresholdMax { get; set; }

        [DataMember]
        public byte SecondZoneScanTime { get; set; }

        [DataMember]
        public byte SecondZoneMaskTime { get; set; }

        [DataMember]
        public byte SecondZoneAmplification { get; set; }

        [DataMember]
        public byte RimShotBoundLeft { get; set; }

        [DataMember]
        public byte RimShotBoundRight { get; set; }

        protected DualPiezoPad()
        {
        }

        protected DualPiezoPad(DrumType type, byte channel) : base(type, channel)
        {
        }

        public DualPiezoPad(byte channel) : base(DrumType.DualPiezoPad, channel)
        {
        }

        public override void ResetToDefault()
        {
            base.ResetToDefault();
            PadNote = 38;
            SideClickNote = 40;
            RimShotNote = 37;

            SecondZoneThresholdMin = 20;
            SecondZoneThresholdMax = 750;
            SecondZoneScanTime = 3;
            SecondZoneMaskTime = 70;
            SecondZoneAmplification = 20;
        }
    }
}