using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class EnableDrumCommand : DrumModuleCommand<EnableDrumCommandResponse>
    {
        [DataMember]
        public int ChannelId { get; set; }

        public EnableDrumCommand(int id) : base(CommandType.EnableDrum, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.EnableDrum)]
    public class EnableDrumCommandResponse : DrumModuleCommandResponse
    {
        protected EnableDrumCommandResponse()
        {
        }

        public EnableDrumCommandResponse(int commandId) : base(CommandType.EnableDrum, commandId)
        {
        }
    }
}
