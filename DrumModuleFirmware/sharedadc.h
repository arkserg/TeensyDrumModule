#ifndef _SHAREDADC_h
#define _SHAREDADC_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "hardware.h"
#include <ADC.h>

class SharedADC {
public:
	static void setup();
	static ADC* adc;
};

#endif