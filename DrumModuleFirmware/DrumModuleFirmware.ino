#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "ThreeZoneCymbal.h"
#include "DigitalPot.h"
#include "SinglePiezoPad.h"
#include "DrumPad.h"
#include "hardware.h"
#include "ChannelSelector.h"
#include "HiHatController.h"
#include "EmptyDrumPad.h"
#include <EEPROM.h>

DigitalPot potentiometers = DigitalPot(0x28);
ChannelSelector channelSelector = ChannelSelector(S0, S1, S2, S3, DRAIN);

DrumPad *drums[12] = {
	new SinglePiezoPad(0, "Snare", &channelSelector, 38, 20, 1023, 3, 70),
	new ThreeZoneCymbal(1, "HiHat", &channelSelector, 20, 21, 18, 25, 500, 3, 70),
	new HiHatController(2, "HiHatController", &channelSelector, 4, 0, 650),
	new DualZoneCymbal(3, "Crash", &channelSelector, 15, 16, 20, 500, 3, 70),
	new EmptyDrumPad(4),
	new EmptyDrumPad(5),
	new EmptyDrumPad(6),
	new EmptyDrumPad(7),
	new EmptyDrumPad(8),
	new EmptyDrumPad(9),
	new EmptyDrumPad(10),
	new EmptyDrumPad(11),
};

void setup()
{
	channelSelector.Setup();

	Wire.begin();
	potentiometers.writeToPotentiometer(0, 20);//10
	potentiometers.writeToPotentiometer(1, 17);//17
	potentiometers.writeToPotentiometer(2, 20);//25
	potentiometers.writeToPotentiometer(3, 20);//20
	potentiometers.writeToPotentiometer(4, 20);//17
	potentiometers.writeToPotentiometer(5, 20);//10
	potentiometers.writeToPotentiometer(6, 20);//15
	potentiometers.writeToPotentiometer(7, 20);//10

	for (int i = 0; i < 12; i++) {
		DrumPad* pad = drums[i];
		if (!pad) continue;
		pad->setup();
	}
  
	Serial.begin(115200);
}

void loop()
{
	while (usbMIDI.read()) {}

	if (Serial.available())
		handleCommand();


	for (int i = 0; i < 12; i++) {
		DrumPad* pad = drums[i];
		if (!pad) continue;
		//todo: uncommant
		//pad->loop();
	}
}

void readPotentiometersSettings()
{
	int value;
	for (byte i = 0; i < 8; i++)
	{		
		value = potentiometers.readFromPotentiometer(i);
		Serial.println("Channel " + String(i) + ": " + String(value));
	}
	delay(3000);
}

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
		setDrumParameters(&response, commandId);
		break;
	case COMMAND_SaveSettings:
		saveSettings(commandId);
		break;
	case COMMAND_ReloadSettings:
		reloadSettings(commandId);
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
	const size_t capacity = JSON_ARRAY_SIZE(12) + JSON_OBJECT_SIZE(3) + 12 * JSON_OBJECT_SIZE(10) + 1780;
	DynamicJsonDocument response(capacity);

	response["CommandType"] = COMMAND_GetAllDrums;
	response["CommandId"] = commandId;

	JsonArray drumsArray = response.createNestedArray("Drums");

	getDrumsJsonArray(&drumsArray);

	serializeJsonPretty(response, Serial); //todo:
}

void getDrumsJsonArray(JsonArray *result)
{
	for (int i = 0; i < 12; i++) {
		DrumPad* pad = drums[i];
		JsonObject padJson = result->createNestedObject();		
		if (!pad) continue;
		pad->serializeParameters(&padJson);	
	}
}

void setDrumParameters(DynamicJsonDocument *request, int commandId)
{
	JsonObject paramsJson = (*request)["Drum"];
	DrumPad* newDrum = setDrumParameters(&paramsJson);
	sendSetDrumParametersResponse(newDrum, commandId);
	//const char* Drum_Name = paramsJson["Name"];
}

void sendSetDrumParametersResponse(DrumPad *pad, int commandId)
{
	const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(10);
	DynamicJsonDocument response(capacity);
	response["CommandType"] = COMMAND_SetDrumParameters;
	response["CommandId"] = commandId;

	JsonObject newDrumJson = response.createNestedObject("Drum");
	pad->serializeParameters(&newDrumJson);

	serializeJson(response, Serial);
}

DrumPad* createDrumPad(JsonObject* json)
{
	byte type = (*json)["Type"];
	switch (type) {
	case TYPE_EmptyDrumPad:
		return new EmptyDrumPad(json);
	case TYPE_SinglePiezoPad:
		return new SinglePiezoPad(json, &channelSelector);
	case TYPE_DualPiezoPad:
		return new SinglePiezoPad(json, &channelSelector); //todo
	case TYPE_DualZoneCymbal:
		return new DualZoneCymbal(json, &channelSelector);
	case TYPE_ThreeZoneCymbal:
		return new ThreeZoneCymbal(json, &channelSelector);
	case TYPE_HiHatController:
		return new HiHatController(json, &channelSelector);
	default:
		return 0;
	}
}

void sendSimpleCommandResponse(byte commandType, int commandId)
{
	const size_t capacity = JSON_OBJECT_SIZE(2);
	DynamicJsonDocument doc(capacity);

	doc["CommandType"] = commandType;
	doc["CommandId"] = commandId;

	serializeJson(doc, Serial);
}

void saveSettings(int commandId)
{
	const size_t capacity = JSON_ARRAY_SIZE(12) + JSON_OBJECT_SIZE(3) + 12 * JSON_OBJECT_SIZE(10) + 1780;
	DynamicJsonDocument result(capacity);
	JsonArray drumsArray = result.createNestedArray("Drums");
	getDrumsJsonArray(&drumsArray);
	
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
		setDrumParameters(&paramsJson);
	}
}

DrumPad* setDrumParameters(JsonObject* paramsJson)
{
	/*delay(300);
	serializeJsonPretty(*paramsJson, Serial);
	Serial.println();*/

	byte channel = (*paramsJson)["Channel"];
	byte type = (*paramsJson)["Type"];

	/*Serial.print("Channel ");
	Serial.println(channel);
	Serial.print("Type ");
	Serial.println(type);*/

	DrumPad* existing = drums[channel];
	if (existing)
	{
		if (existing->type == type)
		{
			existing->setParameters(paramsJson);
			return existing;
		}
		delete existing;
		existing = NULL;
	}

	DrumPad* newDrum = createDrumPad(paramsJson);
	drums[channel] = newDrum;
	return newDrum;
}
