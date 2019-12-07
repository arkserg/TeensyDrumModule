#include "SinglePiezoPad.h"
#include "hardware.h"

SinglePiezoPad::SinglePiezoPad(byte channel, String name, ChannelSelector *channelSelector, 
	byte padNote, int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime) :
	DrumPad(TYPE_SinglePiezoPad, channel, name, channelSelector), padNote(padNote), thresholdMin(thresholdMin),
	thresholdMax(thresholdMax), sensorScantime(sensorScantime), sensorMasktime(sensorMasktime)
{
	lightHitMasktime = sensorMasktime * 4;
}

SinglePiezoPad::SinglePiezoPad(byte type, byte channel, String name, ChannelSelector *channelSelector,
	byte padNote, int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime) :
	DrumPad(type, channel, name, channelSelector), padNote(padNote), thresholdMin(thresholdMin),
	thresholdMax(thresholdMax), sensorScantime(sensorScantime), sensorMasktime(sensorMasktime)
{
	lightHitMasktime = sensorMasktime * 4;
}

SinglePiezoPad::SinglePiezoPad(JsonObject* json, ChannelSelector* channelSelector)
	: DrumPad(channelSelector)
{
	setParameters(json);
}

SinglePiezoPad::SinglePiezoPad(ChannelSelector* channelSelector)
	: DrumPad(channelSelector)
{
}

void SinglePiezoPad::setup()
{
}

void SinglePiezoPad::loop()
{
	unsigned long currentMillis = millis();

	if (nextHitAllowed)
	{
		//todo: вернуть или убрать
		channelSelector->enableChannel(channel);
		int sensorValue = analogRead(0); //todo: sarkashin

		if (!hitInProgress && sensorValue > thresholdMin)
		{
			hitInProgress = true;
			hitStartMillis = currentMillis;
		}

		if (hitInProgress)
		{
			measurments++;

			if (sensorValue > currentValue)
			{
				lastIncreaseMillis = currentMillis;
				currentValue = sensorValue;			
			}

			if ((currentMillis - lastIncreaseMillis) > sensorScantime)
			{
				int velocity = normalizeSensor(currentValue, thresholdMin, thresholdMax);
				if (velocity > (previousHitValue >> 2) || (currentMillis - previousHitMillis) > lightHitMasktime)
				{
					sendNote(padNote, velocity);
					previousHitValue = velocity;
					previousHitMillis = currentMillis;
					nextHitAllowed = false;
				}
				resetCurrentValue();
				hitInProgress = false;
				channelSelector->drainCycle();
				//sendNote(1, measurments/(currentMillis - hitStartMillis));
				Serial.println(measurments / (currentMillis - hitStartMillis));
				measurments = 0;				
			}
		}
	}
	else if ((currentMillis - previousHitMillis) > sensorMasktime)
	{
		nextHitAllowed = true;
	}
}

void SinglePiezoPad::resetCurrentValue()
{
	currentValue = 0;
}

void SinglePiezoPad::serializeParameters(JsonObject* result)
{
	DrumPad::serializeParameters(result);
	(*result)["PadNote"] = padNote;
	(*result)["ThresholdMin"] = thresholdMin;
	(*result)["ThresholdMax"] = thresholdMax;
	(*result)["SensorScantime"] = sensorScantime;
	(*result)["SensorMasktime"] = sensorMasktime;
}

void SinglePiezoPad::setParameters(JsonObject* json)
{
	DrumPad::setParameters(json);
	padNote = (*json)["PadNote"];
	thresholdMin = (*json)["ThresholdMin"];
	thresholdMax = (*json)["ThresholdMax"];
	sensorScantime = (*json)["SensorScantime"];
	sensorMasktime = (*json)["SensorMasktime"];
}