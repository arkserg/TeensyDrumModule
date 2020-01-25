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
	static bool isCrossTalk(unsigned long hitMillis, int hitValue, int xTalkFactor);
	static unsigned long lastHitMillis_;
	static int lastHitValue_;

private:
	static const byte timeWindow_ = 30;
};

#endif