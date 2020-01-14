#include "hihatcontroller.h"
#include "hardware.h"
#include "helper.h"
#include "sharedadc.h"

HiHatController::HiHatController(JsonObject& json)
	: DrumPad(json)
{
	ccControl_ = json["CcControl"];
	thresholdMin_ = json["ThresholdMin"];
	thresholdMax_ = json["ThresholdMax"];
}

void HiHatController::serializeParameters(JsonObject& result)
{
	DrumPad::serializeParameters(result);
	result["CcControl"] = ccControl_;
	result["ThresholdMin"] = thresholdMin_;
	result["ThresholdMax"] = thresholdMax_;
}

void HiHatController::loopImplementation()
{
	unsigned long currentMillis = millis();
	int newValue = SharedADC::adc->analogRead(ANALOG_IN0);
	newValue = 127 - Helper::normalizeSensor(newValue, thresholdMin_, thresholdMax_);
	if (newValue != previousValue_)
	{
		if (currentMillis - previousChangeMillis_ > 3)
		{
			int diff = newValue - previousValue_;
			if (diff > 2 || diff < -2) //todo
			{
				Helper::sendControlChange(ccControl_, newValue);
				previousValue_ = newValue;
			}
		}
	}
}