#ifndef _SINGLEPIEZOPAD_h
#define _SINGLEPIEZOPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DrumPad.h"
#include "ChannelSelector.h"
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

class SinglePiezoPad : public DrumPad {	

public:
	SinglePiezoPad(byte channel, String name, ChannelSelector *channelSelector, byte padNote,
		int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime);
	SinglePiezoPad(JsonObject* json, ChannelSelector* channelSelector);
	void setup();
	void loop();
	void serializeParameters(JsonObject* result);
	void setParameters(JsonObject* json);

	int thresholdMin;
	int thresholdMax;
	int sensorScantime;
	int sensorMasktime;
	byte padNote;

protected:
	SinglePiezoPad(byte type, byte channel, String name, ChannelSelector* channelSelector, byte padNote,
		int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime);
	SinglePiezoPad(ChannelSelector* channelSelector);

	boolean nextHitAllowed = true;
	boolean hitInProgress = false;
	unsigned long previousHitMillis;
	unsigned long lastIncreaseMillis;
	int currentValue;
	int previousHitValue;
	int lightHitMasktime;
	int measurments;
	unsigned long hitStartMillis;

	virtual void resetCurrentValue();
};

#endif