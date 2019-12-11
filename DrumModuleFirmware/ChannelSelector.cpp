#include "channelselector.h"
#include "hardware.h"

void ChannelSelector::setup()
{
	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(S3, OUTPUT);

	pinMode(DRAIN, OUTPUT);
	disableDrain();
}

void ChannelSelector::enableChannel(byte num)
{
	byte l = num % 2 == 0 ? (15 - (num / 2)) : (num / 2);

	digitalWriteFast(S0, (l & 1) != 0);
	digitalWriteFast(S1, (l & 2) != 0);
	digitalWriteFast(S2, (l & 4) != 0);
	digitalWriteFast(S3, (l & 8) != 0);
}

void ChannelSelector::enableDrain()
{
	digitalWriteFast(DRAIN, LOW);
}

void ChannelSelector::disableDrain()
{
	digitalWriteFast(DRAIN, HIGH);
}

void ChannelSelector::drainCycle()
{
	digitalWriteFast(DRAIN, LOW);
	digitalWriteFast(DRAIN, HIGH);
}