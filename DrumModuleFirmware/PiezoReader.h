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
		int sensorScantime, int sensorMasktime, byte amplification);

	static const int AfterShock = -1;
	int loop(int sensorValue);
	void setup();

	int thresholdMin_;
	int thresholdMax_;
	int sensorScantime_;
	int sensorMasktime_;
	byte amplification_;
	boolean hitInProgress_ = false;

private:
	int ProcessHit(int sensorValue, unsigned long currentMillis);

	boolean nextHitAllowed_ = true;
	unsigned long previousHitMillis_;
	unsigned long lastIncreaseMillis_;
	int currentValue_;
	int previousHitValue_;
	int lightHitMasktime_;
	DigitalPotentiometer potentiometer_;
};

#endif