#include "DualZoneCymbal.h"
#include "hardware.h"

DualZoneCymbal::DualZoneCymbal(byte channel, String name, ChannelSelector *channelSelector, 
	byte bowNote, byte edgeNote, int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime) :
	SinglePiezoPad(TYPE_DualZoneCymbal, channel, name, channelSelector, bowNote, thresholdMin,
		thresholdMax, sensorScantime, sensorMasktime), edgeNote(edgeNote)
{
}

DualZoneCymbal::DualZoneCymbal(byte type, byte channel, String name, ChannelSelector *channelSelector, 
	byte bowNote, byte edgeNote, int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime) :
	SinglePiezoPad(type, channel, name, channelSelector, bowNote, thresholdMin,
		thresholdMax, sensorScantime, sensorMasktime), edgeNote(edgeNote)
{
}

DualZoneCymbal::DualZoneCymbal(JsonObject* json, ChannelSelector* channelSelector)
	: SinglePiezoPad(channelSelector)
{
	setParameters(json);
}

DualZoneCymbal::DualZoneCymbal(ChannelSelector* channelSelector)
	: SinglePiezoPad(channelSelector)
{
}

void DualZoneCymbal::setup()
{
}

void DualZoneCymbal::loop()
{
	SinglePiezoPad::loop();
	ZoneSensorLoop();
}

void DualZoneCymbal::resetCurrentValue()
{
	currentValue = 0;
	zoneSensorValue = 1023;	
}

void DualZoneCymbal::sendNote(byte pitch, byte velocity)
{
	if (zoneSensorValue > 1000)
		DrumPad::sendNote(padNote, velocity);
	else
		DrumPad::sendNote(edgeNote, velocity);
}

void DualZoneCymbal::ZoneSensorLoop()
{	
	if (hitInProgress)
	{
		int newValue = analogRead(1); //todo
		if (newValue < zoneSensorValue)
			zoneSensorValue = newValue;
	}
}

void DualZoneCymbal::serializeParameters(JsonObject* result)
{
	SinglePiezoPad::serializeParameters(result);
	(*result)["EdgeNote"] = edgeNote;
}

void DualZoneCymbal::setParameters(JsonObject* json)
{
	SinglePiezoPad::setParameters(json);
	edgeNote = (*json)["EdgeNote"];
}