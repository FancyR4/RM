#ifndef _SYSCTRL_H_
#define _SYSCTRL_H_

#include "main.h"

#define FirstInit 0
#define AlreadyInit 1

typedef enum
{
	RC,
	PC,
} ControlWay_en;

typedef enum
{
	Online,
	Offline,
} ControlState;

void AllInit(void);
void InitCallback(void);
ControlState CheckControlOnline(ControlWay_en SelectControlWay);

#endif




