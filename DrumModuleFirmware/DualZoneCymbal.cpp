#include "dualzonecymbal.h"
#include "hardware.h"
#include "helper.h"
#include "sharedadc.h"

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
	ADC::Sync_result measurement = SharedADC::adc->analogSynchronizedRead(ANALOG_IN0, ANALOG_IN1);

	int velocity = piezoReader_->loop(measurement.result_adc1);

	if (piezoReader_->hitInProgress_)
	{
		if (measurement.result_adc0 < lastZoneSensorValue_)
			lastZoneSensorValue_ = measurement.result_adc0;
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