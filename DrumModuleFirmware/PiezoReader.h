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

	static const int Wait = 0;
	static const int Scan = 1;
	static const int Hold = 2;
	static const int Decay = 3;

	int loop(int sensorValue);
	void setup();
	void enableRealTimeMonitoring();
	void disableRealTimeMonitoring();

	int thresholdMin_;
	int thresholdMax_;
	int scan_;
	int hold_;
	int decay_;
	byte gain_;
	byte scaleType_;
	byte lift_;

	int state_;

private:
	int ProcessHit(int sensorValue, unsigned long currentMillis, unsigned long currentMicros);
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

	//todo: в хэлпер
	bool monitor_;
	bool enableMonitor_;
	bool disableMonitor_;
	unsigned long scanStartMicros_;
	unsigned long holdStartMicros_;
	unsigned long decayStartMicros_;
	unsigned long decayEndMicros_;
	unsigned long hitMicros_;
	unsigned long hitValue_;
	unsigned long monitorTimeStamps_[100];
	int monitorData_[100];
	long measureIndex_;
};

#endif