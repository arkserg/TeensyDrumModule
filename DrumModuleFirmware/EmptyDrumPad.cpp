#include "emptydrumpad.h"
#include "hardware.h"

EmptyDrumPad::EmptyDrumPad(JsonObject& json)
	: DrumPad(json)
{
}

void EmptyDrumPad::loopImplementation()
{	
}