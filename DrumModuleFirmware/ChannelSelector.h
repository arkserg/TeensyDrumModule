#ifndef _CHANNELSELECTOR_h
#define _CHANNELSELECTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class ChannelSelector {	
public:
	static void setup();
	static void enableChannel(byte num);
	static void enableDrain();
	static void disableDrain();
	static void drainCycle();
};

#endif