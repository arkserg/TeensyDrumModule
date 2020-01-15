#ifndef _PIEZOREADER_h
#define _PIEZOREADER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "channelselector.h"
#include "digitalpotentiometer.h"

class PiezoReader {

public:
	PiezoReader(byte channel, byte subChannel, int thresholdMin, int thresholdMax, 
		int scan, int hold, int decay, byte gain, byte scaleType, byte lift);

	static const int AfterShock = -1;
	static const int CrossTalk = -2;

	static const byte Wait = 0;
	static const byte Scan = 1;
	static const byte Hold = 2;

	int loop(int sensorValue);
	void setup();

	int thresholdMin_;
	int thresholdMax_;
	int scan_;
	int hold_;
	int decay_;
	byte gain_;
	byte scaleType_;
	byte lift_;

	byte state_;

private:
	int ProcessHit(int sensorValue, unsigned long currentMillis);
	void CalculateDecayParameters();
	bool IsAfterShock(unsigned long currentMillis);

	DigitalPotentiometer potentiometer_;

	int previousHitValue_;
	unsigned long previousHitMillis_;

	unsigned long scanEndMillis_;
	unsigned long holdEndMillis_;
	unsigned long decayEndMillis_;

	float scaleFactor_;

	int maxValue_;

	float decayK_;
	int decayB_;
};

#endif