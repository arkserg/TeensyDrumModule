#include "EEPROMStreamAdapter.h"

EEPROMStreamAdapter::EEPROMStreamAdapter(int startAddress)
	: pointer(startAddress)
{
}

uint8_t EEPROMStreamAdapter::readByte()
{
	return EEPROM.read(pointer++);
}

void EEPROMStreamAdapter::writeByte(uint8_t value)
{
	EEPROM.write(pointer++, value);
}

void EEPROMStreamAdapter::resetPosition()
{
	pointer = 0;
}