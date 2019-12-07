using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Collections.Generic;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public class GetAllDrumsCommand : DrumModuleCommand<GetAllDrumsCommandResponse>
    {
        public GetAllDrumsCommand(int id) : base(CommandType.GetAllDrums, id)
        {
        }
    }

    [DataContract]
    [CommandType(CommandType.GetAllDrums)]
    public class GetAllDrumsCommandResponse : DrumModuleCommandResponse
    {
        [DataMember]
        public List<DrumPad> Drums { get; set; }

        protected GetAllDrumsCommandResponse()
        {
        }

        public GetAllDrumsCommandResponse(int commandId) : base(CommandType.GetAllDrums, commandId)
        {
        }
    }
}