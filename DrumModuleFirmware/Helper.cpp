#include "helper.h"
#include "hardware.h"
#include <cmath>

namespace Helper
{
	int normalizeSensor(int sensorValue, int thresholdMin, int thresholdMax)
	{
		sensorValue = map(sensorValue, thresholdMin, thresholdMax, 1, 127);
		if (sensorValue > 127) sensorValue = 127;
		return sensorValue;
	}

    int normalizeSensor(int sensorValue, int thresholdMin, int thresholdMax, byte scale, byte lift, float k)
    {
        sensorValue = map(sensorValue, thresholdMin, thresholdMax, 1, 127);

		if (scale == SCALE_Log)
		{
			sensorValue = ((std::log(0.0787 * sensorValue) / std::log(2)) + 6.68) * 12.7;
		}

		if (lift != 0)
		{
			sensorValue = k * sensorValue + lift;
		}

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