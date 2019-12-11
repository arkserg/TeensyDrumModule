#include "helper.h"
#include "hardware.h"

namespace Helper
{
    int normalizeSensor(int sensorValue, int thresholdMin, int thresholdMax)
    {
        sensorValue = map(sensorValue, thresholdMin, thresholdMax, 1, 127);
        if (sensorValue > 127) sensorValue = 127;
        return sensorValue;
    }

	void sendNoteOn(byte channel, byte pitch, byte velocity)
	{
		usbMIDI.sendNoteOn(pitch, velocity, channel);
		usbMIDI.send_now();
	}

	void sendNoteOff(byte channel, byte pitch, byte velocity)
	{
		usbMIDI.sendNoteOff(pitch, velocity, channel);
		usbMIDI.send_now();
	}

	void sendNoteOnOff(byte pitch, byte velocity)
	{
		usbMIDI.sendNoteOn(pitch, velocity, MIDICHANNEL);
		usbMIDI.send_now();
		usbMIDI.sendNoteOff(pitch, velocity, MIDICHANNEL);
		usbMIDI.send_now();
	}

	void sendControlChange(byte channel, byte control, byte value)
	{
		usbMIDI.sendControlChange(control, value, channel);
		usbMIDI.send_now();
	}

	void sendControlChange(byte control, byte value)
	{
		usbMIDI.sendControlChange(control, value, MIDICHANNEL);
		usbMIDI.send_now();
	}
}