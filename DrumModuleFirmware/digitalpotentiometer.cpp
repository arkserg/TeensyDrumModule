#include "digitalpotentiometer.h"

DigitalPotentiometer::DigitalPotentiometer(byte channel, byte subChannel)
{
	address_ = getAddress(channel, subChannel);
	memoryAddress_ = getMemoryAddress(channel, subChannel);
}

byte DigitalPotentiometer::readFromPotentiometer(byte channel, byte subChannel)
{
	uint8_t address = getAddress(channel, subChannel);
	uint8_t memoryAddress = getMemoryAddress(channel, subChannel);
	return readFromPotentiometerImplementation(address, memoryAddress);
}

void DigitalPotentiometer::writeToPotentiometer(byte channel, byte subChannel, byte value) 
{
	uint8_t address = getAddress(channel, subChannel);
	uint8_t memoryAddress = getMemoryAddress(channel, subChannel);
	writeToPotentiometerImplementation(address, memoryAddress, value);
}

byte DigitalPotentiometer::readFromPotentiometerImplementation(uint8_t address, uint8_t memoryAddress)
{
	Wire.beginTransmission(address);
	Wire.write(memoryAddress | 0x0C);   //Read from given memory address
	Wire.endTransmission(false);
	Wire.requestFrom(address, (uint8_t)2);
	byte result = (Wire.read() << 8) | Wire.read();
	Wire.endTransmission();
	return result;
}

void DigitalPotentiometer::writeToPotentiometerImplementation(uint8_t address, uint8_t memoryAddress, byte value)
{
	Wire.beginTransmission(address);
	Wire.write(memoryAddress | 0x00 | ((value >> 8) & 0x01)); //Write to the given memory address + bit 8 of message
	Wire.write(value & 0xFF); //Write bits 0 - 7 of message
	Wire.endTransmission();
}

byte DigitalPotentiometer::readFromPotentiometer()
{
	return readFromPotentiometerImplementation(address_, memoryAddress_);
}

void DigitalPotentiometer::writeToPotentiometer(byte value)
{
	writeToPotentiometerImplementation(address_, memoryAddress_, value);
}

uint8_t DigitalPotentiometer::getAddress(byte channel, byte subChannel) 
{
	if (channel < 4) 
	{
		channel = channel * 2 + subChannel;
	}
	else
	{
		channel += 4;
	}

	channel = channel >> 1;
	return baseAddress_ | ((channel >> 2) & 1) | ((channel << 2) & 4) | (channel & 2);
}

uint8_t DigitalPotentiometer::getMemoryAddress(byte channel, byte subChannel)
{
	if (channel < 4)
	{
		if (subChannel == 0)
			return 1 << 4; //todo
		else
			return 0 << 4; //todo
	}
	else
	{
		return (channel & 0x01) << 4;
	}
}