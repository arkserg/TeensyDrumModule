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
	SinglePiezoPad(byte channel, String name, bool enabled, byte padNote,
		int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime, 
		byte amplification);
	SinglePiezoPad(JsonObject& json);
	~SinglePiezoPad();

	void serializeParameters(JsonObject& result);
	void setup();

	byte padNote_;

protected:
	SinglePiezoPad(byte type, byte channel, String name, bool enabled, byte padNote,
		int thresholdMin, int thresholdMax, int sensorScantime, int sensorMasktime,
		byte amplification);

	void loopImplementation();
	PiezoReader *piezoReader_;
};

#endif