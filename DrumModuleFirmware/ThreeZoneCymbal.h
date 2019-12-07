// YamahaCymbal.h

#ifndef _THREEZONECYMBAL_h
#define _THREEZONECYMBAL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "DrumPad.h"
#include "ChannelSelector.h"
#include "SinglePiezoPad.h"
#include "DualZoneCymbal.h"
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

class ThreeZoneCymbal : public DualZoneCymbal {
public:
	ThreeZoneCymbal(byte channel, String name, ChannelSelector* channelSelector, 
		byte bowNote, byte bellNote, byte edgeNote,
		int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime);
	ThreeZoneCymbal(JsonObject* json, ChannelSelector* channelSelector);
	
	void serializeParameters(JsonObject* result);
	void setParameters(JsonObject* json);

	byte bellNote;

protected:
	void sendNote(byte pitch, byte velocity);
};

#endif