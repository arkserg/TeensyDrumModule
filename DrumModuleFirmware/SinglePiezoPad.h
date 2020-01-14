#ifndef _SINGLEPIEZOPAD_h
#define _SINGLEPIEZOPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "piezoreader.h"
#include "drumpad.h"
#include "channelselector.h"
#include <arduinojson.hpp>
#include <arduinojson.h>

class SinglePiezoPad : public DrumPad {	

public:
	SinglePiezoPad(JsonObject& json);
	~SinglePiezoPad();

	void serializeParameters(JsonObject& result);
	void setup();

	byte padNote_;

protected:
	void loopImplementation();
	PiezoReader *piezoReader_; //todo: точно нужен указатель?
};

#endif