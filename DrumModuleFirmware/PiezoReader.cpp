#include "piezoreader.h"
#include "hardware.h"
#include "helper.h"
#include "xtalkhelper.h"

PiezoReader::PiezoReader(byte channel, byte subChannel,	int thresholdMin, 
	int thresholdMax, int sensorScantime, int sensorMasktime, byte amplification, byte scale, byte lift) :
	thresholdMin_(thresholdMin), thresholdMax_(thresholdMax), sensorScantime_(sensorScantime),
	sensorMasktime_(sensorMasktime), amplification_(amplification), scale_(scale), lift_(lift),
	potentiometer_(channel, subChannel)
{
	lightHitMasktime_ = sensorMasktime * 4;
	k_ = (127 - lift) / 127.0f;
}

void PiezoReader::setup()
{
	potentiometer_.writeToPotentiometer(amplification_);
}

int PiezoReader::loop(int sensorValue)
{
	unsigned long currentMillis = millis();

	if (!nextHitAllowed_ && (currentMillis - previousHitMillis_) > sensorMasktime_)
	{
		nextHitAllowed_ = true;
	}

	if (nextHitAllowed_ && !hitInProgress_ && sensorValue > thresholdMin_)
	{
		hitInProgress_ = true;
		hitStartMillis_ = currentMillis;
	}
	
	if (hitInProgress_)
	{
		return ProcessHit(sensorValue, currentMillis);
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

	if ((currentMillis - hitStartMillis_) >= sensorScantime_)
	{
		if (currentValue_ > (previousHitValue_ >> 2) || (currentMillis - previousHitMillis_) > lightHitMasktime_)
		{
			previousHitValue_ = currentValue_;
			previousHitMillis_ = currentMillis;
			nextHitAllowed_ = false;

			if (XTalkHelper::checkNotCrossTalk(currentMillis, currentValue_))
			{
				result = Helper::normalizeSensor(currentValue_, thresholdMin_, thresholdMax_, scale_, lift_, k_);
			}
			else
			{
				result = CrossTalk;
			}
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