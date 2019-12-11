#ifndef _PIEZOREADER_h
#define _PIEZOREADER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class PiezoReader {

public:
	PiezoReader(int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime);
	int loop(int sensorValue);

	int thresholdMin;
	int thresholdMax;
	int sensorScantime;
	int sensorMasktime;

private:
	int ProcessHit(int sensorValue, unsigned long currentMillis);

	boolean nextHitAllowed = true;
	boolean hitInProgress = false;
	unsigned long previousHitMillis;
	unsigned long lastIncreaseMillis;
	int currentValue;
	int previousHitValue;
	int lightHitMasktime;
};

#endif