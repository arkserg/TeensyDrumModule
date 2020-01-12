#include "dualpiezopad.h"
#include "hardware.h"
#include "helper.h"

DualPiezoPad::DualPiezoPad(byte channel, String name, bool enabled, byte padNote,
	byte sideClickNote, byte rimShotNote, byte rimShotBoundLeft, byte rimShotBoundRight,
	int thresholdMin, int thresholdMax, int scanTime, int maskTime, byte amplification, byte scale, byte lift,
	int secondZoneThresholdMin, int secondZoneThresholdMax, int secondZoneScantime,
	int secondZoneMasktime, byte secondZoneAmplification) :
	SinglePiezoPad(TYPE_DualPiezoPad, channel, name, enabled, padNote, thresholdMin, 
		thresholdMax, scanTime, maskTime, amplification, scale, lift),
	sideClickNote_(sideClickNote),	rimShotNote_(rimShotNote),
	rimShotBoundLeft_(rimShotBoundLeft), rimShotBoundRight_(rimShotBoundRight),
	secondZonePiezoReader_(new PiezoReader(channel, 1, secondZoneThresholdMin, secondZoneThresholdMax, 
		secondZoneScantime, secondZoneMasktime, secondZoneAmplification, SCALE_Linear, 0))
{
}

DualPiezoPad::DualPiezoPad(JsonObject& json)
	: SinglePiezoPad(json)
{
	sideClickNote_ = json["SideClickNote"];
	rimShotNote_ = json["RimShotNote"];
	rimShotBoundLeft_ = json["RimShotBoundLeft"];
	rimShotBoundRight_ = json["RimShotBoundRight"];

	int thresholdMin = json["SecondZoneThresholdMin"];
	int thresholdMax = json["SecondZoneThresholdMax"];
	int sensorScantime = json["SecondZoneScanTime"];
	int sensorMasktime = json["SecondZoneMaskTime"];
	byte amplification = json["SecondZoneAmplification"];
	secondZonePiezoReader_ = new PiezoReader(channel_, 1, thresholdMin, thresholdMax, sensorScantime, sensorMasktime, amplification, SCALE_Linear, 0);
}

DualPiezoPad::~DualPiezoPad()
{
	delete secondZonePiezoReader_;
}

void DualPiezoPad::loopImplementation()
{
	int headValue = analogRead(ANALOG_IN0);
	int rimValue = analogRead(ANALOG_IN1);

	if (headVelocity_ <= 0)
		headVelocity_ = piezoReader_->loop(headValue);

	if (rimVelocity_ <= 0)
		rimVelocity_ = secondZonePiezoReader_->loop(rimValue);
	
	if (headVelocity_ > 0 && rimVelocity_ <= 0)
	{
		if (!secondZonePiezoReader_->hitInProgress_)
		{   // head shot
			Helper::sendNoteOnOff(padNote_, headVelocity_);
			reset();
		}
		return;			
	}

	if (rimVelocity_ > 0 && headVelocity_ <= 0)
	{
		if (!piezoReader_->hitInProgress_)
		{   // side click
			Helper::sendNoteOnOff(padNote_, rimVelocity_);
			reset();
		}
		return;		
	}

	if (headVelocity_ > 0 && rimVelocity_ > 0)
	{
		processRimShot(); 
		reset();
		return;
	}

	if (piezoReader_->hitInProgress_ || secondZonePiezoReader_->hitInProgress_)
		return;

	if (headVelocity_ == PiezoReader::AfterShock || rimVelocity_ == PiezoReader::AfterShock)	
		reset();	
}

void DualPiezoPad::processRimShot()
{
	byte rimshotValue = getRimShotValue();
	if (rimshotValue < rimShotBoundLeft_)
		Helper::sendNoteOnOff(padNote_, rimVelocity_);
	else if (rimshotValue > rimShotBoundRight_)
		Helper::sendNoteOnOff(sideClickNote_, rimVelocity_);
	else
		Helper::sendNoteOnOff(rimShotNote_, rimVelocity_);
}

byte DualPiezoPad::getRimShotValue()
{
	if (headVelocity_ > rimVelocity_)
		return rimVelocity_ * 100 / headVelocity_;

	return 200 - (headVelocity_ * 100 / rimVelocity_);
}

void DualPiezoPad::reset()
{
	headVelocity_ = 0;
	rimVelocity_ = 0;
	ChannelSelector::drainCycle();
}

void DualPiezoPad::setup()
{
	SinglePiezoPad::setup();
	secondZonePiezoReader_->setup();
}

void DualPiezoPad::serializeParameters(JsonObject& result)
{
	SinglePiezoPad::serializeParameters(result);
	result["SecondZoneThresholdMin"] = secondZonePiezoReader_->thresholdMin_;
	result["SecondZoneThresholdMax"] = secondZonePiezoReader_->thresholdMax_;
	result["SecondZoneScanTime"] = secondZonePiezoReader_->sensorScantime_;
	result["SecondZoneMaskTime"] = secondZonePiezoReader_->sensorMasktime_;
	result["SecondZoneAmplification"] = secondZonePiezoReader_->sensorMasktime_;

	result["SideClickNote"] = sideClickNote_;
	result["RimShotNote"] = rimShotNote_;
	result["RimShotBoundLeft"] = rimShotBoundLeft_;
	result["RimShotBoundRight"] = rimShotBoundRight_;
}