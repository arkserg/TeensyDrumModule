#include "HiHatController.h"
#include "hardware.h"

HiHatController::HiHatController(byte channel, String name, ChannelSelector *channelSelector, 
	byte ccControl,	int thresholdMin, int thresholdMax) 
	: DrumPad(TYPE_HiHatController, channel, name, channelSelector),
	ccControl(ccControl), thresholdMin(thresholdMin), thresholdMax(thresholdMax)
{
}

HiHatController::HiHatController(JsonObject* json, ChannelSelector* channelSelector)
	: DrumPad(channelSelector)
{
	setParameters(json);
}

void HiHatController::setup()
{
}

void HiHatController::loop()
{
	//todo: sarkashin всё здесь

	//channelSelector.enableChannel(zoneSensorChannel);

	unsigned long currentMillis = millis();
	int newValue = analogRead(2); //todo
	newValue = 127 - DrumPad::normalizeSensor(newValue, thresholdMin, thresholdMax);
	if (newValue != previousValue)
	{
		if (currentMillis - previousChangeMillis > 3)
		{
			int diff = newValue - previousValue;
			if (diff > 2 || diff < -2) //todo
			{
				DrumPad::sendCC(ccControl, newValue);
				previousValue = newValue;
			}
		}
	}
}

void HiHatController::serializeParameters(JsonObject* result)
{
	DrumPad::serializeParameters(result);
	(*result)["CcControl"] = ccControl;
	(*result)["ThresholdMin"] = thresholdMin;
	(*result)["ThresholdMax"] = thresholdMax;
}

void HiHatController::setParameters(JsonObject* json)
{
	DrumPad::setParameters(json);
	ccControl = (*json)["CcControl"];
	thresholdMin = (*json)["ThresholdMin"];
	thresholdMax = (*json)["ThresholdMax"];
}