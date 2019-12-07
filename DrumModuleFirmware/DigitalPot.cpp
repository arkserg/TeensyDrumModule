// 
// 
// 

#include "DigitalPot.h"

DigitalPot::DigitalPot(byte address):
	address(address)
{
}

uint16_t DigitalPot::readFromPotentiometer(uint8_t channel) {
	uint8_t address = getAddress(channel);
	uint8_t memoryAddress = getMemoryAddress(channel);

	Wire.beginTransmission(address);
	Wire.write(memoryAddress | 0x0C);   //Read from given memory address
	Wire.endTransmission(false);
	Wire.requestFrom(address, (uint8_t)2);
	uint16_t result = (Wire.read() << 8) | Wire.read();
	Wire.endTransmission();
	return result;
}

void DigitalPot::writeToPotentiometer(uint8_t channel, uint16_t value) {
	uint8_t address = getAddress(channel);
	uint8_t memoryAddress = getMemoryAddress(channel);

	Wire.beginTransmission(address);
	Wire.write(memoryAddress | 0x00 | ((value >> 8) & 0x01)); //Write to the given memory address + bit 8 of message
	Wire.write(value & 0xFF); //Write bits 0 - 7 of message
	Wire.endTransmission();
}

uint8_t DigitalPot::getAddress(uint8_t channel) {
	channel = channel >> 1;
	return address | ((channel >> 2) & 1) | ((channel << 2) & 4) | (channel & 2);
}

uint8_t DigitalPot::getMemoryAddress(uint8_t channel) {
	return (channel & 0x01) << 4;
}