#ifndef _EEPROMSTREAMADAPTER_h
#define _EEPROMSTREAMADAPTER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <EEPROM.h>
#include "StreamAdapter.h"

class EEPROMStreamAdapter : public StreamAdapter {
	int pointer = 0;
public:
	EEPROMStreamAdapter(int startAddress);
	uint8_t readByte();
	void writeByte(uint8_t value);
	void resetPosition();
};

#endif