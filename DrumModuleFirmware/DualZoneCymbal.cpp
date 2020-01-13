#include "dualzonecymbal.h"
#include "hardware.h"
#include "helper.h"
#include "sharedadc.h"

DualZoneCymbal::DualZoneCymbal(byte channel, String name, bool enabled,	byte bowNote, 
	byte edgeNote, int thresholdMin, int thresholdMax, int sensorScantime, 
	int sensorMasktime,	byte amplification, byte scale, byte lift) :
	SinglePiezoPad(TYPE_DualZoneCymbal, channel, name, enabled, bowNote, thresholdMin,
		thresholdMax, sensorScantime, sensorMasktime, amplification, scale, lift), edgeNote_(edgeNote)
{
}

DualZoneCymbal::DualZoneCymbal(byte type, byte channel, String name, bool enabled,
	byte bowNote, byte edgeNote, int thresholdMin, int thresholdMax, int sensorScantime, 
	int sensorMasktime, byte amplification, byte scale, byte lift) :
	SinglePiezoPad(type, channel, name, enabled, bowNote, thresholdMin,
		thresholdMax, sensorScantime, sensorMasktime, amplification, scale, lift), edgeNote_(edgeNote)
{
}

DualZoneCymbal::DualZoneCymbal(JsonObject& json)
	: SinglePiezoPad(json)
{
	edgeNote_ = json["EdgeNote"];
	chokeValueThreshold_ = json["ChokeValueThreshold"];
	chokeTimeThreshold_ = json["ChokeTimeThreshold"];
	chokeEnabled_ = json["ChokeEnabled"];
}

void DualZoneCymbal::loopImplementation()
{
	ADC::Sync_result measurement = SharedADC::adc->analogSynchronizedRead(ANALOG_IN0, ANALOG_IN1);

	int velocity = piezoReader_->loop(measurement.result_adc1);

	if (piezoReader_->hitInProgress_)
	{
		if (measurement.result_adc0 < minZoneSensorValue_)
			minZoneSensorValue_ = measurement.result_adc0;
	}
	if (velocity == PiezoReader::AfterShock || velocity == PiezoReader::CrossTalk)
	{
		ChannelSelector::drainCycle();
	}
	else if (velocity != 0)
	{
		sendNote(padNote_, velocity);
		minZoneSensorValue_ = 1023;
		ChannelSelector::drainCycle();
	}

	if(chokeEnabled_)
		processChoke(measurement.result_adc0);
}

void DualZoneCymbal::processChoke(int sensorValue)
{
	if (!chokeInProgress_ && sensorValue <= chokeValueThreshold_)
	{
		chokeStartTime_ = millis();
		chokeInProgress_ = true;
	}
	else if (chokeInProgress_ && !chokeNoteSent_)
	{
		if ((millis() - chokeStartTime_) >= chokeTimeThreshold_)
		{
			chokeNoteSent_ = true;
			Helper::sendAfterTouch(edgeNote_, 127); //todo: может быть нужно выбирать правильную ноту
		}
	}
	else if (chokeInProgress_ && sensorValue > chokeValueThreshold_)
	{
		chokeInProgress_ = false;
		chokeNoteSent_ = false;
	}
}

void DualZoneCymbal::sendNote(byte pitch, byte velocity)
{
	if (minZoneSensorValue_ > 1000)
		Helper::sendNoteOnOff(padNote_, velocity);
	else
		Helper::sendNoteOnOff(edgeNote_, velocity);
}

void DualZoneCymbal::serializeParameters(JsonObject& result)
{
	SinglePiezoPad::serializeParameters(result);
	result["EdgeNote"] = edgeNote_;
	result["ChokeValueThreshold"] = chokeValueThreshold_;
	result["ChokeTimeTreshold"] = chokeTimeThreshold_;
	result["ChokeEnabled"] = chokeEnabled_;
	result["ChokeNote"] = chokeNote_;
}