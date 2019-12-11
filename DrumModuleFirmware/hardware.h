#ifndef HARDWARE_H
#define HARDWARE_H

#define ANALOG_PIN A0
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define DRAIN 4

#define POTENTIOMETERS_ADDRESS 0x28


#define MIDICHANNEL 0

#define TYPE_EmptyDrumPad		0
#define TYPE_SinglePiezoPad		1
#define TYPE_DualPiezoPad		2
#define TYPE_DualZoneCymbal		3
#define TYPE_ThreeZoneCymbal	4
#define TYPE_HiHatController	5

#define COMMAND_Ping				1
#define COMMAND_GetAllDrums			2
#define COMMAND_SetDrumParameters	3
#define COMMAND_SaveSettings		4
#define COMMAND_ReloadSettings		5
#define COMMAND_EnableDrum			6
#define COMMAND_DisableDrum			7

#endif
