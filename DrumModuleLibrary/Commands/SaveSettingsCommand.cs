using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class SaveSettingsCommand : DrumModuleCommand<SaveSettingsCommandResponse>
    {
        public SaveSettingsCommand(int id) : base(CommandType.SaveSettings, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.SaveSettings)]
    public class SaveSettingsCommandResponse : DrumModuleCommandResponse
    {
        protected SaveSettingsCommandResponse()
        {
        }

        public SaveSettingsCommandResponse(int commandId) : base(CommandType.SaveSettings, commandId)
        {
        }
    }
}