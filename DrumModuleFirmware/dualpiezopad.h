#ifndef _DUALPIEZOPAD_h
#define _DUALPIEZOPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "piezoreader.h"
#include "singlepiezopad.h"
#include "channelselector.h"
#include <arduinojson.hpp>
#include <arduinojson.h>

class DualPiezoPad : public SinglePiezoPad {

public:
	DualPiezoPad(byte channel, String name, bool enabled, byte padNote, 
		byte sideClickNote, byte rimShotNote, byte rimShotBoundLeft, byte rimShotBoundRight,
		int thresholdMin, int thresholdMax, int scanTime, int maskTime, byte amplification, byte scale, byte lift,
		int secondZoneThresholdMin, int secondZoneThresholdMax, int secondZoneScantime, 
		int secondZoneMasktime, byte secondZoneAmplification);
	DualPiezoPad(JsonObject& json);
	~DualPiezoPad();

	void serializeParameters(JsonObject& result);
	void setup();

	byte sideClickNote_;
	byte rimShotNote_;
	byte rimShotBoundLeft_;
	byte rimShotBoundRight_;

protected:
	void loopImplementation();
	PiezoReader* secondZonePiezoReader_;
private:
	int headVelocity_;
	int rimVelocity_;
	void reset();
	void processRimShot();
	byte getRimShotValue();
};

#endif