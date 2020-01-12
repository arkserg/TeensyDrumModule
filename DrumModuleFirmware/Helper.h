#ifndef _HELPER_h
#define _HELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Helper
{
    int normalizeSensor(int sensorValue, int thresholdMin, int thresholdMax);
    int normalizeSensor(int sensorValue, int thresholdMin, int thresholdMax, byte scale, byte lift, float k);
	void sendNoteOn(byte channel, byte pitch, byte velocity);
	void sendNoteOff(byte channel, byte pitch, byte velocity);
	void sendNoteOnOff(byte pitch, byte velocity);
	void sendControlChange(byte channel, byte control, byte value);
	void sendControlChange(byte control, byte value);
}

#endif