#include "ThreeZoneCymbal.h"
#include "hardware.h"

ThreeZoneCymbal::ThreeZoneCymbal(byte channel, String name, ChannelSelector* channelSelector,
	byte bowNote, byte edgeNote, byte bellNote, 
	int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime) :
	DualZoneCymbal(TYPE_ThreeZoneCymbal, channel, name, channelSelector, 
		bowNote, edgeNote, thresholdMin, thresholdMax, sensorScantime, sensorMasktime),
	bellNote(bellNote)
{
}

ThreeZoneCymbal::ThreeZoneCymbal(JsonObject* json, ChannelSelector* channelSelector)
	: DualZoneCymbal(channelSelector)
{
	setParameters(json);
}

void ThreeZoneCymbal::sendNote(byte pitch, byte velocity)
{
	if (zoneSensorValue > 1000)
		DrumPad::sendNote(padNote, velocity);
	else if (zoneSensorValue > 100)
		DrumPad::sendNote(edgeNote, velocity);
	else
		DrumPad::sendNote(bellNote, velocity);
}

void ThreeZoneCymbal::serializeParameters(JsonObject* result)
{
	DualZoneCymbal::serializeParameters(result);
	(*result)["BellNote"] = bellNote;
}

void ThreeZoneCymbal::setParameters(JsonObject* json)
{
	DualZoneCymbal::setParameters(json);
	bellNote = (*json)["BellNote"];
}