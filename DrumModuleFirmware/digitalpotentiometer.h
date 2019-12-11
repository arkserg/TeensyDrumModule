#ifndef _DIGITALPOT_h
#define _DIGITALPOT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "hardware.h"
#include <wire.h>

class DigitalPotentiometer {
public:
	DigitalPotentiometer(byte channel, byte subChannel);
	static byte readFromPotentiometer(byte channel, byte subChannel);
	static void writeToPotentiometer(byte channel, byte subChannel, byte value);

	byte readFromPotentiometer();
	void writeToPotentiometer(byte value);

private:
	static uint8_t getAddress(byte channel, byte subChannel);
	static uint8_t getMemoryAddress(byte channel, byte subChannel);

	static byte readFromPotentiometerImplementation(uint8_t address, uint8_t memoryAddress);
	static void writeToPotentiometerImplementation(uint8_t address, uint8_t memoryAddress, byte value);

	static const byte baseAddress_ = POTENTIOMETERS_ADDRESS;

	uint8_t address_;
	uint8_t memoryAddress_;
};

#endif