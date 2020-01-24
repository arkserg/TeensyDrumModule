using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    [DrumType(DrumType.DualZoneHiHat)]
    public class DualZoneHiHat : DualZoneCymbal
    {
        [DataMember]
        public byte ClosedBowNote { get; set; }

        [DataMember]
        public byte ClosedEdgeNote { get; set; }

        protected DualZoneHiHat()
        {
        }

        public DualZoneHiHat(byte channel) : base(DrumType.DualZoneHiHat, channel)
        {
        }

        public override void ResetToDefault()
        {
            base.ResetToDefault();
            ClosedBowNote = 51; //todo
            ClosedEdgeNote = 53; //todo
        }
    }
}