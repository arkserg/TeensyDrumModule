// HiHatController.h

#ifndef _HIHATCONTROLLER_h
#define _HIHATCONTROLLER_h

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

class HiHatController : public DrumPad {
	int previousValue;
	long previousChangeMillis;
public:
	HiHatController(byte channel, String name, ChannelSelector *channelSelector, 
		byte ccControl, int thresholdMin, int thresholdMax);
	HiHatController(JsonObject* json, ChannelSelector* channelSelector);
	void setup();
	void loop();
	void serializeParameters(JsonObject* result);
	void setParameters(JsonObject* json);

	byte ccControl;
	int thresholdMin;
	int thresholdMax;
};

#endif