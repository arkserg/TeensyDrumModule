using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class DisableDrumCommand : DrumModuleCommand<DisableDrumCommandResponse>
    {
        [DataMember]
        public int ChannelId { get; set; }

        public DisableDrumCommand(int id) : base(CommandType.DisableDrum, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.DisableDrum)]
    public class DisableDrumCommandResponse : DrumModuleCommandResponse
    {
        protected DisableDrumCommandResponse()
        {
        }

        public DisableDrumCommandResponse(int commandId) : base(CommandType.DisableDrum, commandId)
        {
        }
    }
}
