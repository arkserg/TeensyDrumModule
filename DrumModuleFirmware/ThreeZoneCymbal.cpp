#include "threezonecymbal.h"
#include "hardware.h"
#include "helper.h"

ThreeZoneCymbal::ThreeZoneCymbal(JsonObject& json)
	: DualZoneCymbal(json)
{
	bellNote_ = json["BellNote"];
}

void ThreeZoneCymbal::serializeParameters(JsonObject& result)
{
	DualZoneCymbal::serializeParameters(result);
	result["BellNote"] = bellNote_;
}

bool ThreeZoneCymbal::isChokeArea(int sensorValue)
{
	return sensorValue >= 100 && sensorValue <= 1000;
}

void ThreeZoneCymbal::sendNote(byte pitch, byte velocity)
{
	if (minZoneSensorValue_ > 1000)
		Helper::sendNoteOnOff(padNote_, velocity);
	else if (minZoneSensorValue_ > 100)
		Helper::sendNoteOnOff(edgeNote_, velocity);
	else
		Helper::sendNoteOnOff(bellNote_, velocity);
}