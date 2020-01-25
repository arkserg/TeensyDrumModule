using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class RestartCommand : DrumModuleCommand<RestartCommandResponse>
    {
        public RestartCommand(int id) : base(CommandType.Restart, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.Restart)]
    public class RestartCommandResponse : DrumModuleCommandResponse
    {
        protected RestartCommandResponse()
        {
        }

        public RestartCommandResponse(int commandId) : base(CommandType.Restart, commandId)
        {
        }
    }
}