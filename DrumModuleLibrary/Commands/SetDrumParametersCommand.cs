using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class SetDrumParametersCommand : DrumModuleCommand<SetDrumParametersCommandResponse>
    {
        [DataMember]
        public DrumPad Drum { get; set; }

        public SetDrumParametersCommand(int id) : base(CommandType.SetDrumParameters, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.SetDrumParameters)]
    public class SetDrumParametersCommandResponse : DrumModuleCommandResponse
    {
        [DataMember]
        public DrumPad Drum { get; set; }

        protected SetDrumParametersCommandResponse()
        {
        }

        public SetDrumParametersCommandResponse(int commandId) : base(CommandType.SetDrumParameters, commandId)
        {
        }
    }
}