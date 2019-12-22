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

        [DataMember]
        public bool Enabled { get; set; }

        protected DrumPad()
        {
        }

        protected DrumPad(DrumType type, byte channel, bool enabled = true)
        {
            Type = type;
            Channel = channel;
            Enabled = enabled;
        }

        public virtual void ResetToDefault()
        {
            Name = $"Drum pad {Channel}";
        }
    }
}