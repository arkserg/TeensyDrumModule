#include "PiezoReader.h"
#include "hardware.h"

PiezoReader::PiezoReader(int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime) : 
	thresholdMin(thresholdMin), thresholdMax(thresholdMax), 
	sensorScantime(sensorScantime), sensorMasktime(sensorMasktime)
{
	lightHitMasktime = sensorMasktime * 4;
}

int PiezoReader::loop(int sensorValue)
{
	unsigned long currentMillis = millis();

	if (nextHitAllowed)
	{
		if (!hitInProgress && sensorValue > thresholdMin)
		{
			hitInProgress = true;
		}

		if (hitInProgress)
		{
			return ProcessHit(sensorValue, currentMillis);
		}
	}
	else if ((currentMillis - previousHitMillis) > sensorMasktime)
	{
		nextHitAllowed = true;
	}
	return 0; //todo
}

int PiezoReader::ProcessHit(int sensorValue, unsigned long currentMillis)
{
	int result = 0;

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
			result = velocity;
			previousHitValue = velocity;
			previousHitMillis = currentMillis;
			nextHitAllowed = false;
		}
		else
		{
			result = -1; //todo: aftershock
		}
		currentValue = 0;
		hitInProgress = false;

		//channelSelector->drainCycle(); todo: выше по стеку
	}
	return result;
}