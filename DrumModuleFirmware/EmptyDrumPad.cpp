#include "emptydrumpad.h"
#include "hardware.h"

EmptyDrumPad::EmptyDrumPad(byte channel) :
	DrumPad(TYPE_EmptyDrumPad, channel, 0, false)
{
}

EmptyDrumPad::EmptyDrumPad(JsonObject& json)
	: DrumPad(json)
{
}

void EmptyDrumPad::loopImplementation()
{	
}