#include "dualzonehihat.h"
#include "hardware.h"
#include "helper.h"

DualZoneHiHat::DualZoneHiHat(JsonObject& json)
	: DualZoneCymbal(json)
{
	closedBowNote_ = json["ClosedBowNote"];
	closedEdgeNote_ = json["ClosedEdgeNote"];
}

void DualZoneHiHat::serializeParameters(JsonObject& result)
{
	DualZoneCymbal::serializeParameters(result);
	result["ClosedBowNote"] = closedBowNote_;
	result["ClosedEdgeNote"] = closedEdgeNote_;
}

void DualZoneHiHat::sendNote(byte pitch, byte velocity)
{
	// todo: select zone
	// HiHatController::currentValue_

	if (minZoneSensorValue_ > 1000)
		Helper::sendNoteOnOff(padNote_, velocity);
	else
		Helper::sendNoteOnOff(edgeNote_, velocity);
}