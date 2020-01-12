#ifndef _XTALKHELPER_h
#define _XTALKHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "hardware.h"

class XTalkHelper {
public:
	static void setup();
	static bool checkNotCrossTalk(unsigned long hitMillis, int hitValue);
	static unsigned long lastHitMillis_;
	static int lastHitValue_;

private:
	static const byte timeWindow_ = 3;
	static const byte factor_ = 3;
};

#endif