#include "singlepiezopad.h"
#include "hardware.h"
#include "helper.h"
#include "sharedadc.h"

SinglePiezoPad::SinglePiezoPad(JsonObject& json)
	: DrumPad(json)
{
	padNote_ = json["PadNote"];
	int thresholdMin = json["ThresholdMin"];
	int thresholdMax = json["ThresholdMax"];
	int scan = json["Scan"];
	int hold = json["Hold"];
	int decay = json["Decay"];
	byte gain = json["Gain"];
	byte scale = json["Scale"];
	byte lift = json["Lift"];
	piezoReader_ = new PiezoReader(channel_, 0, thresholdMin, thresholdMax, scan, hold, decay, gain, scale, lift);
}

void SinglePiezoPad::serializeParameters(JsonObject& result)
{
	DrumPad::serializeParameters(result);
	result["PadNote"] = padNote_;
	result["ThresholdMin"] = piezoReader_->thresholdMin_;
	result["ThresholdMax"] = piezoReader_->thresholdMax_;
	result["Scan"] = piezoReader_->scan_;
	result["Hold"] = piezoReader_->hold_;
	result["Decay"] = piezoReader_->decay_;
	result["Gain"] = piezoReader_->gain_;
	result["Scale"] = piezoReader_->scale_;
	result["Lift"] = piezoReader_->lift_;
}

SinglePiezoPad::~SinglePiezoPad()
{
	delete piezoReader_;
}

void SinglePiezoPad::loopImplementation()
{
	ADC::Sync_result measurement = SharedADC::adc->analogSynchronizedRead(ANALOG_IN0, ANALOG_IN1);
	int value = channel_ < 4 ? measurement.result_adc0 : measurement.result_adc1;
	int velocity = piezoReader_->loop(value);

	if (velocity == PiezoReader::AfterShock || velocity == PiezoReader::CrossTalk)
	{
		ChannelSelector::drainCycle();
	}
	else if (velocity != 0)
	{
		Helper::sendNoteOnOff(padNote_, velocity);
		ChannelSelector::drainCycle();
	}
}

void SinglePiezoPad::setup()
{
	DrumPad::setup();
	piezoReader_->setup();
}