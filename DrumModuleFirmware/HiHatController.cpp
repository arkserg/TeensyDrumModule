#include "hihatcontroller.h"
#include "hardware.h"
#include "helper.h"
#include "sharedadc.h"

int HiHatController::currentValue_ = 0;

HiHatController::HiHatController(JsonObject& json)
	: DrumPad(json)
{
	ccControl_ = json["CcControl"];
	pedalNote_ = json["PedalNote"];
	splashNote_ = json["SplashNote"];
	thresholdMin_ = json["ThresholdMin"];
	thresholdMax_ = json["ThresholdMax"];
}

void HiHatController::serializeParameters(JsonObject& result)
{
	DrumPad::serializeParameters(result);
	result["CcControl"] = ccControl_;
	result["PedalNote"] = pedalNote_;
	result["SplashNote"] = splashNote_;
	result["ThresholdMin"] = thresholdMin_;
	result["ThresholdMax"] = thresholdMax_;
}

void HiHatController::loopImplementation()
{
	unsigned long currentMillis = millis();
	int newValue = SharedADC::adc->analogRead(ANALOG_IN0);
	newValue = 127 - Helper::normalizeSensor(newValue, thresholdMin_, thresholdMax_);
	if (newValue != currentValue_)
	{
		if (currentMillis - previousChangeMillis_ > 3)
		{
			int diff = newValue - currentValue_;
			if (diff > 2 || diff < -2) //todo
			{
				Helper::sendControlChange(ccControl_, newValue);
				currentValue_ = newValue;
			}
		}
	}
}