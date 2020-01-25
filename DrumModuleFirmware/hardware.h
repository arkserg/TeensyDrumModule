#ifndef HARDWARE_H
#define HARDWARE_H

#define ANALOG_IN0 A15
#define ANALOG_IN1 A20
#define S0 27
#define S1 26
#define S2 25
#define S3 24
#define DRAIN 28

#define POTENTIOMETERS_ADDRESS 0x28


#define MIDICHANNEL 10

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
#define COMMAND_Restart				8

#define SCALE_Linear	0
#define SCALE_Log		1

#define RESTART_ADDR 0xE000ED0C
#define READ_RESTART() (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

#endif
