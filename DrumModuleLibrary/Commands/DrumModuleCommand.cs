using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using System.Runtime.Serialization;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Commands
{
    [DataContract]
    public abstract class DrumModuleCommand<TResponse>
        where TResponse : DrumModuleCommandResponse
    {
        [DataMember]
        public CommandType CommandType { get; private set; }

        [DataMember]
        public int CommandId { get; private set; }

        protected DrumModuleCommand()
        {
        }

        public DrumModuleCommand(CommandType type, int id)
        {
            CommandType = type;
            CommandId = id;
        }
    }

    [DataContract]
    public abstract class DrumModuleCommandResponse
    {
        [DataMember]
        public CommandType CommandType { get; private set; }

        [DataMember]
        public int CommandId { get; private set; }

        protected DrumModuleCommandResponse()
        {
        }

        public DrumModuleCommandResponse(CommandType commandType, int commandId)
        {
            CommandType = commandType;
            CommandId = commandId;
        }
    }
}