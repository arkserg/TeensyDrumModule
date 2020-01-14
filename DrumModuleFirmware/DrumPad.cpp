#include "drumpad.h"
#include "hardware.h"

DrumPad::DrumPad(JsonObject& json)
{
	type_ = json["Type"];
	channel_ = json["Channel"];
	name_ = json["Name"].as<String>();
	enabled_ = json["Enabled"];
}

void DrumPad::serializeParameters(JsonObject& result)
{
	result["Type"] = type_;
	result["Channel"] = channel_;
	result["Name"] = name_;
	result["Enabled"] = enabled_;
}

void DrumPad::loop()
{
	ChannelSelector::enableChannel(channel_);
	loopImplementation();
}

void DrumPad::setup()
{
}