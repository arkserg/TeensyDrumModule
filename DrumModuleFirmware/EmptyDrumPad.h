#ifndef _EMPTYDRUMPAD_h
#define _EMPTYDRUMPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "drumpad.h"
#include <arduinojson.hpp>
#include <arduinojson.h>

class EmptyDrumPad : public DrumPad {

public:
	EmptyDrumPad(JsonObject& json);

protected:
	void loopImplementation();
};

#endif