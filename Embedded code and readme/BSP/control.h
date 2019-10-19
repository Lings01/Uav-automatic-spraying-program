#ifndef __CONTROL_H
#define __CONTROL_H

#include "system.h"

#define  OPEN_SPRAY    GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define  CLOSE_SPRAY   GPIO_ResetBits(GPIOB, GPIO_Pin_8)

void Get_SystemMode(void);

enum system_mode
{	
	Error_Mode=0,
	Normal_Mode
};

void Spray_Control(void);
void Spray_Init(void);

#endif
