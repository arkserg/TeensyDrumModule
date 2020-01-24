// HiHatController.h

#ifndef _HIHATCONTROLLER_h
#define _HIHATCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "drumpad.h"
#include "channelselector.h"
#include "singlepiezopad.h"
#include <arduinojson.hpp>
#include <arduinojson.h>

class HiHatController : public DrumPad {
	long previousChangeMillis_;
public:
	HiHatController(JsonObject& json);

	void serializeParameters(JsonObject& result);

	byte ccControl_;
	byte pedalNote_;
	byte splashNote_;
	int thresholdMin_;
	int thresholdMax_;

	static int currentValue_;

protected:
	void loopImplementation();
};

#endif