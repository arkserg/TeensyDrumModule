#ifndef _EMPTYDRUMPAD_h
#define _EMPTYDRUMPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "DrumPad.h"
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

class EmptyDrumPad : public DrumPad {

public:
	EmptyDrumPad(byte channel);
	EmptyDrumPad(JsonObject* json);
	void setup();
	void loop();
};

#endif