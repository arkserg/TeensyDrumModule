// 
// 
// 

#include "ChannelSelector.h"

ChannelSelector::ChannelSelector(byte s0, byte s1, byte s2, byte s3, byte drain) :
	s0(s0), s1(s1), s2(s2), s3(s3), drain(drain)
{		
}

void ChannelSelector::Setup()
{
	pinMode(s0, OUTPUT);
	pinMode(s1, OUTPUT);
	pinMode(s2, OUTPUT);
	pinMode(s3, OUTPUT);

	pinMode(drain, OUTPUT);
	disableDrain();
}

void ChannelSelector::enableChannel(byte num)
{
	byte l = num % 2 == 0 ? (15 - (num / 2)) : (num / 2);

	digitalWrite(s0, (l & 1) != 0);
	digitalWrite(s1, (l & 2) != 0);
	digitalWrite(s2, (l & 4) != 0);
	digitalWrite(s3, (l & 8) != 0);	
}

void ChannelSelector::enableDrain()
{
	digitalWrite(drain, LOW);
}

void ChannelSelector::disableDrain()
{
	digitalWrite(drain, HIGH);
}

void ChannelSelector::drainCycle()
{
	digitalWrite(drain, LOW);
	digitalWrite(drain, HIGH);
}