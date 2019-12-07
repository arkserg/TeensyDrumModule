using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class ReloadSettingsCommand : DrumModuleCommand<ReloadSettingsCommandResponse>
    {
        public ReloadSettingsCommand(int id) : base(CommandType.ReloadSettings, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.ReloadSettings)]
    public class ReloadSettingsCommandResponse : DrumModuleCommandResponse
    {
        protected ReloadSettingsCommandResponse()
        {
        }

        public ReloadSettingsCommandResponse(int commandId) : base(CommandType.ReloadSettings, commandId)
        {
        }
    }
}