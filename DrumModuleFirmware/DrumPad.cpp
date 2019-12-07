#include "DrumPad.h"
#include "hardware.h"

DrumPad::DrumPad(byte type, byte channel, String name, ChannelSelector *channelSelector) :
	type(type), channel(channel), name(name), channelSelector(channelSelector)
{
}

DrumPad::DrumPad(ChannelSelector *channelSelector) :
	channelSelector(channelSelector)
{
}

void DrumPad::sendNote(byte pitch, byte velocity)
{
	usbMIDI.sendNoteOn(pitch, velocity, MIDICHANNEL);
	usbMIDI.send_now();
	usbMIDI.sendNoteOff(pitch, velocity, MIDICHANNEL);	
	usbMIDI.send_now();

	//Serial.println("Note: " + String(pitch) + " Velocity: " + String(velocity));

	/*Serial.write((byte)5);
	String message = "Note: " + (String)pitch + " Velocity: " + (String)velocity;
	Serial.write(message.length());
	Serial.print(message);*/
}

void DrumPad::sendCC(byte control, byte value)
{
	usbMIDI.sendControlChange(control, value, MIDICHANNEL);
	usbMIDI.send_now();
}

void DrumPad::noteOn(byte channel, byte pitch, byte velocity)
{
	usbMIDI.sendNoteOn(pitch, velocity, channel);
	usbMIDI.send_now();
}

void DrumPad::noteOff(byte channel, byte pitch, byte velocity)
{
	usbMIDI.sendNoteOff(pitch, velocity, channel);
	usbMIDI.send_now();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void DrumPad::controlChange(byte channel, byte control, byte value)
{
	usbMIDI.sendControlChange(control, value, channel);
	usbMIDI.send_now();
}

int DrumPad::normalizeSensor(int sensorValue, int thresholdMin, int thresholdMax)
{
	sensorValue = map(sensorValue, thresholdMin, thresholdMax, 1, 127);
	if (sensorValue > 127) sensorValue = 127;
	return sensorValue;
}

void DrumPad::serializeParameters(JsonObject* result)
{
	(*result)["Type"] = type;
	(*result)["Channel"] = channel;
	(*result)["Name"] = name;
}

void DrumPad::setParameters(JsonObject* json)
{
	type = (*json)["Type"];
	channel = (*json)["Channel"];
	name = (*json)["Name"].as<String>();
}