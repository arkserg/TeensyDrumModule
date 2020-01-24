#include <arduinojson.hpp>
#include <arduinojson.h>
#include <eeprom.h>
#include "threezonecymbal.h"
#include "digitalpotentiometer.h"
#include "singlepiezopad.h"
#include "drumpad.h"
#include "hardware.h"
#include "channelselector.h"
#include "hihatcontroller.h"
#include "emptydrumpad.h"
#include "sharedadc.h"

DrumPad* drums[12];

//DrumPad* drums[12] =
//{
//	new SinglePiezoPad(0, "Snare", true, 38, 20, 750, 3, 70, 10),
//	new EmptyDrumPad(1),
//	new EmptyDrumPad(2),
//	new EmptyDrumPad(3),
//	new EmptyDrumPad(4),
//	new EmptyDrumPad(5),
//	new EmptyDrumPad(6),
//	new EmptyDrumPad(7),
//	new EmptyDrumPad(8),
//	new EmptyDrumPad(9),
//	new EmptyDrumPad(10),
//	new EmptyDrumPad(11),
//};

void setup()
{
	SharedADC::setup();
	ChannelSelector::setup();
	Wire.begin();
	Serial.begin(115200);
	loadSettingsFromEeprom();

	for (int i = 0; i < 12; i++) {
		DrumPad* pad = drums[i];
		if (!pad) continue;
		if (!pad->enabled_) continue;
		pad->setup();
	}

	//potentiometers.writeToPotentiometer(0, 20);//10
	//potentiometers.writeToPotentiometer(1, 17);//17
	//potentiometers.writeToPotentiometer(2, 20);//25
	//potentiometers.writeToPotentiometer(3, 20);//20
	//potentiometers.writeToPotentiometer(4, 20);//17
	//potentiometers.writeToPotentiometer(5, 20);//10
	//potentiometers.writeToPotentiometer(6, 20);//15
	//potentiometers.writeToPotentiometer(7, 20);//10
}

void loop()
{
	while (usbMIDI.read()) {}

	if (Serial.available())
		handleCommand();

	for (int i = 0; i < 12; i++) {
		DrumPad* pad = drums[i];
		if (!pad) continue;
		if (!pad->enabled_) continue;
		pad->loop();
	}
}

void potentiometersTest()
{
	Serial.println();
	Serial.println();
	Serial.println("Old values");

	for (int channel = 0; channel < 4; channel++)
	{
		byte oldValue0 = DigitalPotentiometer::readFromPotentiometer(channel, 0);
		byte oldValue1 = DigitalPotentiometer::readFromPotentiometer(channel, 1);
		Serial.print("Channel ");
		Serial.print(channel);
		Serial.print("| ");
		Serial.print(oldValue0);
		Serial.print(" : ");
		Serial.println(oldValue1);
	}

	DigitalPotentiometer::writeToPotentiometer(0, 0, 0);
	DigitalPotentiometer::writeToPotentiometer(0, 1, 1);
	DigitalPotentiometer::writeToPotentiometer(1, 0, 2);
	DigitalPotentiometer::writeToPotentiometer(1, 1, 3);
	DigitalPotentiometer::writeToPotentiometer(2, 0, 4);
	DigitalPotentiometer::writeToPotentiometer(2, 1, 5);
	DigitalPotentiometer::writeToPotentiometer(3, 0, 6);
	DigitalPotentiometer::writeToPotentiometer(3, 1, 7);

	Serial.println();
	Serial.println();
	Serial.println("New values");

	for (int channel = 0; channel < 4; channel++)
	{
		byte oldValue0 = DigitalPotentiometer::readFromPotentiometer(channel, 0);
		byte oldValue1 = DigitalPotentiometer::readFromPotentiometer(channel, 1);
		Serial.print("Channel ");
		Serial.print(channel);
		Serial.print("| ");
		Serial.print(oldValue0);
		Serial.print(" : ");
		Serial.println(oldValue1);
	}
}

#pragma region Serial commands

void handleCommand()
{
	const size_t capacity = JSON_ARRAY_SIZE(12) + JSON_OBJECT_SIZE(3) + 12 * JSON_OBJECT_SIZE(10);
	DynamicJsonDocument response(capacity);
	deserializeJson(response, Serial);

	int commandType = response["CommandType"];
	int commandId = response["CommandId"];

	switch (commandType) {
	case COMMAND_Ping:
		ping(commandId);
		break;
	case COMMAND_GetAllDrums:
		getAllDrums(commandId);
		break;
	case COMMAND_SetDrumParameters:
		setDrumParameters(response, commandId);
		break;
	case COMMAND_SaveSettings:
		saveSettings(commandId);
		break;
	case COMMAND_ReloadSettings:
		reloadSettings(commandId);
		break;
	case COMMAND_EnableDrum:
		enableDrum(response, commandId);
		break;
	case COMMAND_DisableDrum:
		disableDrum(response, commandId);
		break;
	case COMMAND_Restart:
		restart(commandId);
		break;
	default:
		break;
	}
}

void ping(int commandId)
{
	sendSimpleCommandResponse(COMMAND_Ping, commandId);
}

void getAllDrums(int commandId)
{
	const size_t capacity = JSON_ARRAY_SIZE(12) + JSON_OBJECT_SIZE(3) + 12 * JSON_OBJECT_SIZE(10) + 3780;
	DynamicJsonDocument response(capacity);

	response["CommandType"] = COMMAND_GetAllDrums;
	response["CommandId"] = commandId;

	JsonArray drumsArray = response.createNestedArray("Drums");

	getDrumsJsonArray(drumsArray);

	serializeJson(response, Serial);
}

void setDrumParameters(DynamicJsonDocument& request, int commandId)
{
	JsonObject paramsJson = request["Drum"];
	DrumPad* newDrum = setDrumParameters(paramsJson);

	const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(10) + 170;
	DynamicJsonDocument response(capacity);
	response["CommandType"] = COMMAND_SetDrumParameters;
	response["CommandId"] = commandId;

	JsonObject newDrumJson = response.createNestedObject("Drum");
	newDrum->serializeParameters(newDrumJson);

	serializeJson(response, Serial);
}

void saveSettings(int commandId)
{
	const size_t capacity = JSON_ARRAY_SIZE(12) + JSON_OBJECT_SIZE(3) + 12 * JSON_OBJECT_SIZE(10) + 3780;
	DynamicJsonDocument result(capacity);
	JsonArray drumsArray = result.createNestedArray("Drums");
	getDrumsJsonArray(drumsArray);

	String jsonStr;
	serializeJson(result, jsonStr);

	int length = jsonStr.length();
	byte byte1 = length & 0x00ff;
	byte byte2 = (length & 0xff00) >> 8;
	EEPROM.write(0, byte1);
	EEPROM.write(1, byte2);

	for (int i = 0; i < length; i++)
		EEPROM.write(i + 2, jsonStr[i]);

	sendSimpleCommandResponse(COMMAND_SaveSettings, commandId);
}

void reloadSettings(int commandId)
{
	loadSettingsFromEeprom();
	sendSimpleCommandResponse(COMMAND_ReloadSettings, commandId);
}

void enableDrum(DynamicJsonDocument& request, int commandId)
{
	byte index = request["ChannelId"];
	DrumPad* drum = drums[index];
	if (drum)
	{
		drum->enabled_ = true;
	}
	sendSimpleCommandResponse(COMMAND_EnableDrum, commandId);
}

void disableDrum(DynamicJsonDocument& request, int commandId)
{
	byte index = request["ChannelId"];
	DrumPad* drum = drums[index];
	if (drum)
	{
		drum->enabled_ = false;
	}
	sendSimpleCommandResponse(COMMAND_DisableDrum, commandId);
}

void restart(int commandId)
{
	sendSimpleCommandResponse(COMMAND_Restart, commandId);
	WRITE_RESTART(0x5FA0004);
}

void sendSimpleCommandResponse(byte commandType, int commandId)
{
	const size_t capacity = JSON_OBJECT_SIZE(2);
	DynamicJsonDocument doc(capacity);

	doc["CommandType"] = commandType;
	doc["CommandId"] = commandId;

	serializeJson(doc, Serial);
}

void getDrumsJsonArray(JsonArray& result)
{
	for (int i = 0; i < 12; i++) {
		DrumPad* pad = drums[i];
		JsonObject padJson = result.createNestedObject();
		if (!pad) continue;
		pad->serializeParameters(padJson);
	}
}

#pragma endregion

void loadSettingsFromEeprom()
{
	byte byte1 = EEPROM.read(0);
	byte byte2 = EEPROM.read(1);
	int length = byte1 | (byte2) << 8;

	char* a;
	a = new char[length];
	for (int i = 0; i < length; i++)
		a[i] = EEPROM.read(i + 2);

	const size_t capacity = JSON_ARRAY_SIZE(12) + JSON_OBJECT_SIZE(3) + 12 * JSON_OBJECT_SIZE(10) + 3780;
	DynamicJsonDocument settings(capacity);
	deserializeJson(settings, a);
	JsonArray drumsArray = settings["Drums"];

	for (int i = 0; i < 12; i++)
	{
		JsonObject paramsJson = drumsArray[i];
		setDrumParameters(paramsJson);
	}
}

DrumPad* setDrumParameters(JsonObject& paramsJson)
{
	byte channel = paramsJson["Channel"];

	DrumPad* existing = drums[channel];
	if (existing)
	{		
		delete existing;
		existing = NULL;
	}

	DrumPad* newDrum = createDrumPad(paramsJson);
	newDrum->setup();
	drums[channel] = newDrum;
	return newDrum;
}

DrumPad* createDrumPad(JsonObject& json)
{
	byte type = json["Type"];
	switch (type) {
	case TYPE_EmptyDrumPad:
		return new EmptyDrumPad(json);
	case TYPE_SinglePiezoPad:
		return new SinglePiezoPad(json);
	case TYPE_DualPiezoPad:
		return new SinglePiezoPad(json); //todo
	case TYPE_DualZoneCymbal:
		return new DualZoneCymbal(json);
	case TYPE_ThreeZoneCymbal:
		return new ThreeZoneCymbal(json);
	case TYPE_DualZoneHiHat:
		return new DualZoneHiHat(json);
	case TYPE_HiHatController:
		return new HiHatController(json);
	default:
		return 0;
	}
}
