// DrumPad.h

#ifndef _DRUMPAD_h
#define _DRUMPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "channelselector.h"
#include "midiusb.h"
#include <arduinojson.hpp>
#include <arduinojson.h>

class DrumPad {
public:
	byte type_;
	byte channel_;
	String name_;
	bool enabled_;

	virtual void setup();
	virtual void loop();
	virtual void serializeParameters(JsonObject& result);

protected:
	DrumPad(byte type, byte channel, String name, bool enabled);
	DrumPad(JsonObject& json);

	virtual void loopImplementation() = 0;
};

#endif