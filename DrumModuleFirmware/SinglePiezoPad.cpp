#include "singlepiezopad.h"
#include "hardware.h"
#include "helper.h"
#include "sharedadc.h"

SinglePiezoPad::SinglePiezoPad(byte channel, String name, bool enabled,
	byte padNote, int thresholdMin, int thresholdMax, int sensorScantime, 
	int sensorMasktime,	byte amplification) :
	DrumPad(TYPE_SinglePiezoPad, channel, name, enabled), padNote_(padNote),
	piezoReader_(new PiezoReader(channel, 0, thresholdMin, thresholdMax, sensorScantime, sensorMasktime, amplification))
{
}

SinglePiezoPad::SinglePiezoPad(byte type, byte channel, String name, bool enabled,
	byte padNote, int thresholdMin, int thresholdMax, int sensorScantime, 
	int sensorMasktime,	byte amplification) :
	DrumPad(type, channel, name, enabled), padNote_(padNote),
	piezoReader_(new PiezoReader(channel, 0, thresholdMin, thresholdMax, sensorScantime, sensorMasktime, amplification))
{
}

SinglePiezoPad::SinglePiezoPad(JsonObject& json)
	: DrumPad(json)
{
	padNote_ = json["PadNote"];
	int thresholdMin = json["ThresholdMin"];
	int thresholdMax = json["ThresholdMax"];
	int sensorScantime = json["SensorScantime"];
	int sensorMasktime = json["SensorMasktime"];
	byte amplification = json["Amplification"];
	piezoReader_ = new PiezoReader(channel_, 0, thresholdMin, thresholdMax, sensorScantime, sensorMasktime, amplification);
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

	if (velocity == PiezoReader::AfterShock)
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

void SinglePiezoPad::serializeParameters(JsonObject& result)
{
	DrumPad::serializeParameters(result);
	result["PadNote"] = padNote_;
	result["ThresholdMin"] = piezoReader_->thresholdMin_;
	result["ThresholdMax"] = piezoReader_->thresholdMax_;
	result["SensorScantime"] = piezoReader_->sensorScantime_;
	result["SensorMasktime"] = piezoReader_->sensorMasktime_;
	result["Amplification"] = piezoReader_->amplification_;
}