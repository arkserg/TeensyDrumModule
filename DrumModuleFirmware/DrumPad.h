// DrumPad.h

#ifndef _DRUMPAD_h
#define _DRUMPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ChannelSelector.h"
#include "MIDIUSB.h"
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

class DrumPad {
public:
	byte type;
	byte channel;
	String name;


	virtual void setup() = 0;
	virtual void loop() = 0;
	virtual void serializeParameters(JsonObject* result);
	virtual void setParameters(JsonObject* json);

protected:
	DrumPad(byte type, byte channel, String name, ChannelSelector* channelSelector);
	DrumPad(ChannelSelector* channelSelector);
	virtual void sendNote(byte pitch, byte velocity);
	void sendCC(byte control, byte value);
	int normalizeSensor(int sensorValue, int thresholdMin, int thresholdMax);

	ChannelSelector* channelSelector;

private:
	void noteOn(byte channel, byte pitch, byte velocity);
	void noteOff(byte channel, byte pitch, byte velocity);
	void controlChange(byte channel, byte control, byte value);
};

#endif