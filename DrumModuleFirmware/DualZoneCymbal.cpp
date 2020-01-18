#include "dualzonecymbal.h"
#include "hardware.h"
#include "helper.h"
#include "sharedadc.h"

DualZoneCymbal::DualZoneCymbal(JsonObject& json)
	: SinglePiezoPad(json)
{
	edgeNote_ = json["EdgeNote"];
	chokeTimeThreshold_ = json["ChokeTimeThreshold"];
	chokeEnabled_ = json["ChokeEnabled"];
}

void DualZoneCymbal::serializeParameters(JsonObject& result)
{
	SinglePiezoPad::serializeParameters(result);
	result["EdgeNote"] = edgeNote_;
	result["ChokeTimeThreshold"] = chokeTimeThreshold_;
	result["ChokeEnabled"] = chokeEnabled_;
}

void DualZoneCymbal::loopImplementation()
{
	ADC::Sync_result measurement = SharedADC::adc->analogSynchronizedRead(ANALOG_IN0, ANALOG_IN1);

	int velocity = piezoReader_->loop(measurement.result_adc1);

	if (piezoReader_->state_ == piezoReader_->Scan)
	{
		if (measurement.result_adc0 < minZoneSensorValue_)
			minZoneSensorValue_ = measurement.result_adc0;
	}
	if (velocity > 0)
	{
		sendNote(padNote_, velocity);
		minZoneSensorValue_ = 1023;
	}

	if(chokeEnabled_)
		processChoke(measurement.result_adc0);
}

void DualZoneCymbal::processChoke(int sensorValue)
{
	if (chokeInProgress_ && !isChokeArea(sensorValue))
	{
		chokeInProgress_ = false;
		chokeNoteSent_ = false;
	}
	else if (!chokeInProgress_ && isChokeArea(sensorValue))
	{
		chokeStartTime_ = millis();
		chokeInProgress_ = true;
	}

	if (chokeInProgress_ && !chokeNoteSent_
		&& (millis() - chokeStartTime_) >= chokeTimeThreshold_)
	{
		chokeNoteSent_ = true;
		Helper::sendAfterTouch(edgeNote_); //todo: нужно выбирать правильную ноту
	}
}

bool DualZoneCymbal::isChokeArea(int sensorValue)
{
	return sensorValue <= 1000;
}

void DualZoneCymbal::sendNote(byte pitch, byte velocity)
{
	if (minZoneSensorValue_ > 1000)
		Helper::sendNoteOnOff(padNote_, velocity);
	else
		Helper::sendNoteOnOff(edgeNote_, velocity);
}