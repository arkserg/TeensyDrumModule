#include "threezonecymbal.h"
#include "hardware.h"
#include "helper.h"

ThreeZoneCymbal::ThreeZoneCymbal(byte channel, String name, bool enabled,
	byte bowNote, byte edgeNote, byte bellNote, int thresholdMin, int thresholdMax, 
	int sensorScantime, int sensorMasktime, byte amplification) :	
	DualZoneCymbal(TYPE_ThreeZoneCymbal, channel,name, enabled, bowNote, edgeNote, 
		thresholdMin, thresholdMax, sensorScantime, sensorMasktime, amplification), 
	bellNote_(bellNote)
{
}

ThreeZoneCymbal::ThreeZoneCymbal(JsonObject& json)
	: DualZoneCymbal(json)
{
	bellNote_ = json["BellNote"];
}

void ThreeZoneCymbal::sendNote(byte pitch, byte velocity)
{
	if (lastZoneSensorValue_ > 1000)
		Helper::sendNoteOnOff(padNote_, velocity);
	else if (lastZoneSensorValue_ > 100)
		Helper::sendNoteOnOff(edgeNote_, velocity);
	else
		Helper::sendNoteOnOff(bellNote_, velocity);
}

void ThreeZoneCymbal::serializeParameters(JsonObject& result)
{
	DualZoneCymbal::serializeParameters(result);
	result["BellNote"] = bellNote_;
}