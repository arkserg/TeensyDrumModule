// ChannelSelector.h

#ifndef _CHANNELSELECTOR_h
#define _CHANNELSELECTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class ChannelSelector {
	byte s0;
	byte s1;
	byte s2;
	byte s3;
	byte drain;
public:
	ChannelSelector(byte s0, byte s1, byte s2, byte s3, byte drain);
	void Setup();
	void enableChannel(byte num);
	void enableDrain();
	void disableDrain();
	void drainCycle();
};

#endif

