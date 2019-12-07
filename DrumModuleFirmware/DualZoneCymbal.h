// YamahaCymbal.h

#ifndef _DUALZONECYMBAL_h
#define _DUALZONECYMBAL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DrumPad.h"
#include "ChannelSelector.h"
#include "SinglePiezoPad.h"
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

class DualZoneCymbal : public SinglePiezoPad {
	void ZoneSensorLoop();

public:
	DualZoneCymbal(byte channel, String name, ChannelSelector *channelSelector, 
		byte bowNote, byte edgeNote,  int thresholdMin, int thresholdMax,
		int sensorScantime, int sensorMasktime);
	DualZoneCymbal(JsonObject* json, ChannelSelector* channelSelector);

	void setup();
	void loop();
	void serializeParameters(JsonObject* result);
	void setParameters(JsonObject* json);

	byte edgeNote;

protected:
	DualZoneCymbal(byte type, byte channel, String name, ChannelSelector* channelSelector, 
		byte bowNote, byte edgeNote, int thresholdMin, int thresholdMax,
		int sensorScantime, int sensorMasktime);
	DualZoneCymbal(ChannelSelector* channelSelector);

	void resetCurrentValue();
	void sendNote(byte pitch, byte velocity);

	int zoneSensorValue;
};

#endif