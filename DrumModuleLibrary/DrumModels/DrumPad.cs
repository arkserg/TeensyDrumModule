using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels
{
    [DataContract]
    public abstract class DrumPad
    {
        [DataMember]
        public DrumType Type { get; private set; }

        [DataMember]
        public byte Channel { get; private set; }

        [DataMember]
        public string Name { get; set; }

        protected DrumPad()
        {
        }

        protected DrumPad(DrumType type, byte channel)
        {
            Type = type;
            Channel = channel;
        }
    }
}