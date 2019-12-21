#include "dualzonecymbal.h"
#include "hardware.h"
#include "helper.h"

DualZoneCymbal::DualZoneCymbal(byte channel, String name, bool enabled,	byte bowNote, 
	byte edgeNote, int thresholdMin, int thresholdMax, int sensorScantime, 
	int sensorMasktime,	byte amplification) :
	SinglePiezoPad(TYPE_DualZoneCymbal, channel, name, enabled, bowNote, thresholdMin,
		thresholdMax, sensorScantime, sensorMasktime, amplification), edgeNote_(edgeNote)
{
}

DualZoneCymbal::DualZoneCymbal(byte type, byte channel, String name, bool enabled,
	byte bowNote, byte edgeNote, int thresholdMin, int thresholdMax, int sensorScantime, 
	int sensorMasktime, byte amplification) :
	SinglePiezoPad(type, channel, name, enabled, bowNote, thresholdMin,
		thresholdMax, sensorScantime, sensorMasktime, amplification), edgeNote_(edgeNote)
{
}

DualZoneCymbal::DualZoneCymbal(JsonObject& json)
	: SinglePiezoPad(json)
{
	edgeNote_ = json["EdgeNote"];
}

void DualZoneCymbal::loopImplementation()
{
	int sensorValue = analogRead(ANALOG_IN0);
	int zoneSensorValue = analogRead(ANALOG_IN1);
	int velocity = piezoReader_->loop(sensorValue);

	if (piezoReader_->hitInProgress_)
	{
		if (zoneSensorValue < lastZoneSensorValue_)
			lastZoneSensorValue_ = zoneSensorValue;
	}
	if (velocity == PiezoReader::AfterShock)
	{
		ChannelSelector::drainCycle();
	}
	if (velocity != 0)
	{
		sendNote(padNote_, velocity);
		lastZoneSensorValue_ = 1023;
		ChannelSelector::drainCycle();
	}
}

void DualZoneCymbal::sendNote(byte pitch, byte velocity)
{
	if (lastZoneSensorValue_ > 1000)
		Helper::sendNoteOnOff(padNote_, velocity);
	else
		Helper::sendNoteOnOff(edgeNote_, velocity);
}

void DualZoneCymbal::serializeParameters(JsonObject& result)
{
	SinglePiezoPad::serializeParameters(result);
	result["EdgeNote"] = edgeNote_;
}