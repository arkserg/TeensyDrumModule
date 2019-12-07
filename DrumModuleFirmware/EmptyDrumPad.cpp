#include "EmptyDrumPad.h"
#include "hardware.h"

EmptyDrumPad::EmptyDrumPad(byte channel) :
	DrumPad(TYPE_EmptyDrumPad, channel, 0, 0)
{
}

EmptyDrumPad::EmptyDrumPad(JsonObject* json)
	: DrumPad(0)
{
	DrumPad::setParameters(json);
}

void EmptyDrumPad::setup()
{
}

void EmptyDrumPad::loop()
{	
}