using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class PingCommand : DrumModuleCommand<PingCommandResponse>
    {
        public PingCommand(int id) : base(CommandType.Ping, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.Ping)]
    public class PingCommandResponse : DrumModuleCommandResponse
    {
        protected PingCommandResponse()
        {
        }

        public PingCommandResponse(int commandId) : base(CommandType.Ping, commandId)
        {
        }
    }
}