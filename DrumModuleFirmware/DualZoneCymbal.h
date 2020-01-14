// YamahaCymbal.h

#ifndef _DUALZONECYMBAL_h
#define _DUALZONECYMBAL_h

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

class DualZoneCymbal : public SinglePiezoPad {
public:
	DualZoneCymbal(JsonObject& json);

	void serializeParameters(JsonObject& result);

	byte edgeNote_;
	byte chokeTimeThreshold_;
	bool chokeEnabled_;

protected:
	virtual bool isChokeArea(int sensorValue);
	virtual void sendNote(byte pitch, byte velocity);
	void loopImplementation();
	void processChoke(int sensorValue);

	int minZoneSensorValue_;
	bool chokeInProgress_;
	bool chokeNoteSent_;
	unsigned long chokeStartTime_;

};

#endif