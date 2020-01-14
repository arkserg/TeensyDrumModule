#include "piezoreader.h"
#include "hardware.h"
#include "helper.h"
#include "xtalkhelper.h"

PiezoReader::PiezoReader(byte channel, byte subChannel,	int thresholdMin, 
	int thresholdMax, int scan, int hold, int decay, byte gain, byte scaleType, byte lift) :
	thresholdMin_(thresholdMin), thresholdMax_(thresholdMax), scan_(scan), hold_(hold),
	decay_(decay), gain_(gain), scaleType_(scaleType), lift_(lift),
	potentiometer_(channel, subChannel)
{
	scaleFactor_ = (127 - lift) / 127.0f;

	state_ = Wait;
	maxValue_ = 0;
	previousHitValue_ = 0;
	previousHitMillis_ = 0;
	holdStartMillis_ = 0;
	decayStartMillis_ = 0;
	waitStartMillis_ = 0;


}

void PiezoReader::setup()
{
	potentiometer_.writeToPotentiometer(gain_);
}

byte PiezoReader::loop(int sensorValue)
{
	unsigned long currentMillis = millis();

	switch (state_)
	{
	case Wait:
		if (sensorValue > thresholdMin_)
		{
			state_ = Scan;
			holdStartMillis_ = currentMillis + scan_;
			maxValue_ = sensorValue;
		}
		break;
	case Scan:
		if (sensorValue > maxValue_)
		{
			maxValue_ = sensorValue;
		}
		if (holdStartMillis_ >= currentMillis)
		{
			state_ = Hold;
			decayStartMillis_ = holdStartMillis_ + hold_;
			ChannelSelector::drainCycle();
			return ProcessHit(sensorValue, currentMillis);
		}
	case Hold:
		if (decayStartMillis_ >= currentMillis)
		{
			CalculateDecayParameters();
			state_ = Wait;
			waitStartMillis_ = decayStartMillis_ + decay_;
		}
		break;
	}

	return 0; //todo
}

byte PiezoReader::ProcessHit(int sensorValue, unsigned long currentMillis)
{
	if (IsAfterShock(sensorValue))
	{
		state_ = Wait;
		return AfterShock;
	}

	previousHitValue_ = maxValue_;
	previousHitMillis_ = currentMillis;
	
	if (XTalkHelper::checkNotCrossTalk(currentMillis, maxValue_))
	{
		byte velocity = Helper::normalizeSensor(maxValue_, thresholdMin_, thresholdMax_, scaleType_, lift_, scaleFactor_);
		//todo
		Serial.println(velocity);
		return velocity;
	}
	else
	{
		return CrossTalk;
	}	
}

void PiezoReader::CalculateDecayParameters()
{
	decayB_ = maxValue_;
	decayK_ = (thresholdMin_ - maxValue_) / decay_;
}

bool PiezoReader::IsAfterShock(unsigned long currentMillis)
{
	if (currentMillis >= waitStartMillis_) return false;
	int threshold = decayK_ * (currentMillis - decayStartMillis_) + decayB_;
	return maxValue_ < threshold;
}