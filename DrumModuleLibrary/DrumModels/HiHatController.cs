using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    [DrumType(DrumType.HiHatController)]
    public class HiHatController : DrumPad
    {
        [DataMember]
        public byte CcControl { get; set; }

        [DataMember]
        public byte PedalNote { get; set; }

        [DataMember]
        public byte SplashNote { get; set; }

        [DataMember]
        public Int16 ThresholdMin { get; set; }

        [DataMember]
        public Int16 ThresholdMax { get; set; }

        protected HiHatController()
        {
        }

        public HiHatController(byte channel) : base(DrumType.HiHatController, channel)
        {
        }

        public override void ResetToDefault()
        {
            CcControl = 4;
            PedalNote = 5; //todo
            SplashNote = 6; //todo
            ThresholdMin = 0;
            ThresholdMax = 650;
            base.ResetToDefault();
        }
    }
}