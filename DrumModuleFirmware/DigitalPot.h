// DigitalPot.h

#ifndef _DIGITALPOT_h
#define _DIGITALPOT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>

class DigitalPot {
	byte address;
public:
	DigitalPot(byte address);
	uint16_t readFromPotentiometer(uint8_t channel);
	void writeToPotentiometer(uint8_t channel, uint16_t value);

private:
	uint8_t getAddress(uint8_t channel);
	uint8_t getMemoryAddress(uint8_t channel);

};

#endif

