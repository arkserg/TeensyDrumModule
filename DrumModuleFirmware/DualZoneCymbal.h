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
	DualZoneCymbal(byte channel, String name, bool enabled,
		byte bowNote, byte edgeNote,  int thresholdMin, int thresholdMax,
		int sensorScantime, int sensorMasktime, byte amplification);
	DualZoneCymbal(JsonObject& json);

	void serializeParameters(JsonObject& result);

	byte edgeNote_;
	int chokeValueThreshold_;
	byte chokeTimeThreshold_;
	bool chokeEnabled_;
	byte chokeNote_;

protected:
	DualZoneCymbal(byte type, byte channel, String name, bool enabled,
		byte bowNote, byte edgeNote, int thresholdMin, int thresholdMax,
		int sensorScantime, int sensorMasktime, byte amplification);

	void sendNote(byte pitch, byte velocity);
	void loopImplementation();
	void processChoke(int sensorValue);

	int minZoneSensorValue_;
	bool chokeInProgress_;
	bool chokeNoteSent_;
	unsigned long chokeStartTime_;

};

#endif