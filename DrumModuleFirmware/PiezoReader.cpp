#include "piezoreader.h"
#include "hardware.h"
#include "helper.h"

PiezoReader::PiezoReader(byte channel, byte subChannel,	int thresholdMin, 
	int thresholdMax, int sensorScantime, int sensorMasktime, byte amplification) :
	thresholdMin_(thresholdMin), thresholdMax_(thresholdMax), sensorScantime_(sensorScantime),
	sensorMasktime_(sensorMasktime), amplification_(amplification), potentiometer_(channel, subChannel)
{
	lightHitMasktime_ = sensorMasktime * 4;
}

void PiezoReader::setup()
{
	potentiometer_.writeToPotentiometer(amplification_);
}

int PiezoReader::loop(int sensorValue)
{
	unsigned long currentMillis = millis();

	if (nextHitAllowed_)
	{
		if (!hitInProgress_ && sensorValue > thresholdMin_)
		{
			hitInProgress_ = true;
		}

		if (hitInProgress_)
		{
			return ProcessHit(sensorValue, currentMillis);
		}
	}
	else if ((currentMillis - previousHitMillis_) > sensorMasktime_)
	{
		nextHitAllowed_ = true;
	}
	return 0; //todo
}

int PiezoReader::ProcessHit(int sensorValue, unsigned long currentMillis)
{
	int result = 0;

	if (sensorValue > currentValue_)
	{
		lastIncreaseMillis_ = currentMillis;
		currentValue_ = sensorValue;
	}

	if ((currentMillis - lastIncreaseMillis_) > sensorScantime_)
	{
		int velocity = Helper::normalizeSensor(currentValue_, thresholdMin_, thresholdMax_);
		if (velocity > (previousHitValue_ >> 2) || (currentMillis - previousHitMillis_) > lightHitMasktime_)
		{
			result = velocity;
			previousHitValue_ = velocity;
			previousHitMillis_ = currentMillis;
			nextHitAllowed_ = false;
		}
		else
		{
			result = AfterShock;
		}
		currentValue_ = 0;
		hitInProgress_ = false;
	}
	return result;
}