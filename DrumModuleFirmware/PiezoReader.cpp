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

	monitor_ = false;
	enableMonitor_ = false;
	disableMonitor_ = false;
}

void PiezoReader::setup()
{
	potentiometer_.writeToPotentiometer(gain_);
}

void PiezoReader::enableRealTimeMonitoring()
{
	enableMonitor_ = true;
}

void PiezoReader::disableRealTimeMonitoring()
{
	disableMonitor_ = true;
}

int PiezoReader::loop(int sensorValue)
{
	unsigned long currentMillis = millis();
	unsigned long currentMicros = monitor_ ? micros() : 0;

	if (monitor_)
	{
		measureIndex_++;
		monitorTimeStamps_[measureIndex_] = currentMicros;
		monitorData_[measureIndex_] = sensorValue;

		if (measureIndex_ == 99)
		{
			//todo: flush buffer
			measureIndex_ = 0;
		}
	}

	if (state_ == Decay && decayEndMillis_ <= currentMillis)
	{
		state_ = Wait;
		decayEndMicros_ = currentMicros;
		//todo: send data
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

			//monitor
			if (enableMonitor_)
			{
				monitor_ = true;
				enableMonitor_ = false;
			}
			scanStartMicros_ = currentMicros;
			measureIndex_ = 0;
			monitorTimeStamps_[measureIndex_] = currentMicros;
			monitorData_[measureIndex_] = sensorValue;
		}
		break;
	case Scan:
		if (sensorValue > maxValue_)
		{
			maxValue_ = sensorValue;
		}
		if (scanEndMillis_ <= currentMillis)
		{
			state_ = Hold;
			holdEndMillis_ = scanEndMillis_ + hold_;

			//monitor
			holdStartMicros_ = currentMicros;
		}
	case Hold:
		if (holdEndMillis_ <= currentMillis)
		{
			state_ = Decay;

			ChannelSelector::drainCycle();
			if (IsAfterShock(sensorValue))
			{
				return AfterShock;
			}
			return ProcessHit(sensorValue, currentMillis, currentMicros);
		}
		break;
	}

	return 0; //todo
}

int PiezoReader::ProcessHit(int sensorValue, unsigned long currentMillis, unsigned long currentMicros)
{
	CalculateDecayParameters();
	previousHitValue_ = maxValue_;
	previousHitMillis_ = currentMillis;
	decayEndMillis_ = holdEndMillis_ + decay_;

	//monitor
	decayStartMicros_ = currentMicros;

	if (XTalkHelper::checkNotCrossTalk(currentMillis, maxValue_))
	{
		hitMicros_ = currentMicros;
		hitValue_ = sensorValue;

		return Helper::normalizeSensor(maxValue_, thresholdMin_, thresholdMax_, scaleType_, lift_, scaleFactor_);
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
	if (decayEndMillis_ <= currentMillis) return false;
	int threshold = decayK_ * (currentMillis - holdEndMillis_) + decayB_;
	return maxValue_ < threshold;
}