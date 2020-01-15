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
	//todo: debug
	Serial.print("Gain: ");
	Serial.println(gain_);
	potentiometer_.writeToPotentiometer(gain_);
}

int PiezoReader::loop(int sensorValue)
{
	unsigned long currentMillis = millis();

	switch (state_)
	{
	case Wait:
		if (sensorValue > thresholdMin_)
		{
			//todo: debug
			Serial.println("Wait End");
			Serial.println("Scan Start");

			state_ = Scan;
			scanEndMillis_ = currentMillis + scan_;

			//todo: debug
			Serial.print("scan: ");
			Serial.println(scan_);

			Serial.print("current millis: ");
			Serial.println(currentMillis);

			Serial.print("scanEndMillis_: ");
			Serial.println(scanEndMillis_);


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
			//todo: debug
			Serial.println("Scan End");
			Serial.println("Hold Start");

			state_ = Hold;
			holdEndMillis_ = scanEndMillis_ + hold_;
		}
	case Hold:
		if (holdEndMillis_ <= currentMillis)
		{
			//todo: debug
			Serial.println("Hold End");
			Serial.println("Wait Start");

			ChannelSelector::drainCycle();
			int result = ProcessHit(sensorValue, currentMillis);

			state_ = Wait; //на самом деле Decay, но в этот период мы также принимаем сигнал

			return result;
		}
		break;
	}

	return 0; //todo
}

int PiezoReader::ProcessHit(int sensorValue, unsigned long currentMillis)
{
	//todo: debug
	Serial.println("ProcessHit Start");


	if (IsAfterShock(sensorValue))
	{
		state_ = Wait;

		//todo: debug
		Serial.println("ProcessHit End: AfterShock");

		return AfterShock;
	}

	CalculateDecayParameters();
	previousHitValue_ = maxValue_;
	previousHitMillis_ = currentMillis;
	decayEndMillis_ = holdEndMillis_ + decay_;

	if (XTalkHelper::checkNotCrossTalk(currentMillis, maxValue_))
	{
		byte velocity = Helper::normalizeSensor(maxValue_, thresholdMin_, thresholdMax_, scaleType_, lift_, scaleFactor_);
		//todo: debug
		Serial.print("ProcessHit End: ");
		Serial.println(velocity);

		return velocity;
	}
	else
	{
		//todo: debug
		Serial.println("ProcessHit End: CrossTalk");
		return CrossTalk;
	}
}

void PiezoReader::CalculateDecayParameters()
{
	//todo: debug
	Serial.println("CalculateDecayParameters Start");


	decayB_ = maxValue_;
	decayK_ = (thresholdMin_ - maxValue_) / decay_;


	//todo: debug
	Serial.println("CalculateDecayParameters End");
}

bool PiezoReader::IsAfterShock(unsigned long currentMillis)
{
	//todo: debug
	Serial.println("IsAfterShock Start");


	if (decayEndMillis_ <= currentMillis) return false;
	int threshold = decayK_ * (currentMillis - holdEndMillis_) + decayB_;

	//todo: debug
	Serial.print("IsAfterShock End: ");
	Serial.println(maxValue_ < threshold);

	return maxValue_ < threshold;
}