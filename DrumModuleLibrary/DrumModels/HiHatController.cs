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
        public Int16 ThresholdMin { get; set; }

        [DataMember]
        public Int16 ThresholdMax { get; set; }

        protected HiHatController()
        {
        }

        public HiHatController(byte channel) : base(DrumType.HiHatController, channel)
        {
        }
    }
}