// YamahaCymbal.h

#ifndef _THREEZONECYMBAL_h
#define _THREEZONECYMBAL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "drumpad.h"
#include "channelselector.h"
#include "singlepiezopad.h"
#include "dualzonecymbal.h"
#include <arduinojson.hpp>
#include <arduinojson.h>

class ThreeZoneCymbal : public DualZoneCymbal {
public:
	ThreeZoneCymbal(JsonObject& json);
	
	void serializeParameters(JsonObject& result);

	byte bellNote_;

protected:
	void sendNote(byte pitch, byte velocity);
	bool isChokeArea(int sensorValue);
};

#endif