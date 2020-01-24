#ifndef _DUALZONEHIHAT_h
#define _DUALZONEHIHAT_h

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

class DualZoneHiHat : public DualZoneCymbal {
public:
	DualZoneHiHat(JsonObject& json);

	void serializeParameters(JsonObject& result);

	byte closedBowNote_;
	byte closedEdgeNote_;

protected:
	void sendNote(byte pitch, byte velocity);
};

#endif