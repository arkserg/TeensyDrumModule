#include "hihatcontroller.h"
#include "hardware.h"
#include "helper.h"

HiHatController::HiHatController(byte channel, String name, bool enabled,
	byte ccControl,	int thresholdMin, int thresholdMax) 
	: DrumPad(TYPE_HiHatController, channel, name, enabled),
	ccControl_(ccControl), thresholdMin_(thresholdMin), thresholdMax_(thresholdMax)
{
}

HiHatController::HiHatController(JsonObject& json)
	: DrumPad(json)
{
	ccControl_ = json["CcControl"];
	thresholdMin_ = json["ThresholdMin"];
	thresholdMax_ = json["ThresholdMax"];
}

void HiHatController::loopImplementation()
{
	unsigned long currentMillis = millis();
	int newValue = analogRead(2); //todo
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

void HiHatController::serializeParameters(JsonObject& result)
{
	DrumPad::serializeParameters(result);
	result["CcControl"] = ccControl_;
	result["ThresholdMin"] = thresholdMin_;
	result["ThresholdMax"] = thresholdMax_;
}