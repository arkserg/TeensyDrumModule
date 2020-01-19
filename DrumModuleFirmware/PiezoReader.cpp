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
	scanEndMillis_ = 0;
	holdEndMillis_ = 0;
	decayEndMillis_ = 0;
}

void PiezoReader::setup()
{
	potentiometer_.writeToPotentiometer(gain_);
}

int PiezoReader::loop(int sensorValue)
{
	unsigned long currentMillis = millis();

	if (state_ == Decay && decayEndMillis_ <= currentMillis)
	{
		state_ = Wait;
	}

	switch (state_)
	{
	case Decay:
	case Wait:
		if (sensorValue > thresholdMin_)
		{
			state_ = Scan;
			scanEndMillis_ = currentMillis + scan_;
			maxValue_ = sensorValue;
		}
		break;
	case Scan:
		if (sensorValue > maxValue_)
		{
			maxValue_ = sensorValue;
		}
		if (scanEndMillis_ <= currentMillis)
		{
			ChannelSelector::drainCycle();

			state_ = Hold;
			holdEndMillis_ = scanEndMillis_ + hold_;

			return ProcessHit(sensorValue, currentMillis);
		}
		break;
	case Hold:
		if (holdEndMillis_ <= currentMillis)
		{
			ChannelSelector::drainCycle();
			state_ = Decay;
		}
		break;
	}

	return 0; //todo
}

int PiezoReader::ProcessHit(int sensorValue, unsigned long currentMillis)
{
	if (maxValue_ > (previousHitValue_ >> 2) || decayEndMillis_ <= currentMillis)
	{
		previousHitValue_ = maxValue_;
		previousHitMillis_ = currentMillis;
		decayEndMillis_ = holdEndMillis_ + decay_;
		
		if (XTalkHelper::checkNotCrossTalk(currentMillis, maxValue_))
		{
			return Helper::normalizeSensor(maxValue_, thresholdMin_, thresholdMax_, scaleType_, lift_, scaleFactor_);
		}
		else
		{
			return CrossTalk;
		}
	}
	else
	{
		state_ = Decay;
		return AfterShock;
	}
}

void PiezoReader::CalculateDecayParameters()
{
	decayB_ = maxValue_;
	decayK_ = (thresholdMin_ - maxValue_) / decay_;
}

bool PiezoReader::IsAfterShock(unsigned long currentMillis)
{
	if (decayEndMillis_ <= currentMillis) return false;
	int threshold = decayK_ * (currentMillis - holdEndMillis_) + decayB_;
	return maxValue_ < threshold;
}