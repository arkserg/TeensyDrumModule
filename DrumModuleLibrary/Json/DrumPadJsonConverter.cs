using System;
using System.Collections.Generic;
using System.Linq;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Attributes;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.DrumModels;
using Arkserg.TeensyDrumModule.DrumModuleLibrary.Enums;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace Arkserg.TeensyDrumModule.DrumModuleLibrary.Json
{
    /// <inheritdoc />
    public class DrumPadJsonConverter : JsonConverter
    {
        public override bool CanWrite => false;

        private readonly JsonSerializer _basicSerializer;
        private readonly Dictionary<DrumType, Type> _drumTypes;

        public DrumPadJsonConverter()
        {
            _basicSerializer = new JsonSerializer();
            _drumTypes = typeof(DrumPad).GetInheritedClasses().ToDictionary(GetDrumType, t => t);
        }

        private DrumType GetDrumType(Type drumType)
        {
            return drumType.GetCustomAttributes(false).OfType<DrumTypeAttribute>().Single().DrumType;
        }

        /// <inheritdoc />
        public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            var result = _basicSerializer.Deserialize(reader);

            if (result == null) return null;

            if (!(result is JObject))
                throw new Exception($"Unexpected json token. Expected: 'JObject', actual: '{result.GetType().Name}', objectType={objectType?.Name ?? "null"}");

            var jObject = (JObject)result;

            if (!jObject.TryGetValue("type", StringComparison.InvariantCultureIgnoreCase, out var typeToken))
                throw new Exception($"Input json object doesn't contain 'type' field \r\n {jObject}");

            var typeCode = (DrumType) typeToken.Value<int>();
            if (!_drumTypes.TryGetValue(typeCode, out var type))
                throw new Exception($"Unknown drum type: {typeCode}");

            return jObject.ToObject(type, serializer);
        }

        /// <inheritdoc />
        public override bool CanConvert(Type objectType)
        {
            return objectType == typeof(DrumPad);
        }

        /// <inheritdoc />
        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }
    }    
}