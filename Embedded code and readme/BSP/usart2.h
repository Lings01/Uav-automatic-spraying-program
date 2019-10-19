#ifndef __USART2_H
#define __USART2_H

#include "system.h"

#define RC_MIN 	-300
#define RC_MAX 	300
/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)		//0x01
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)		//0x02
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)		//0x04
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)		//0x08
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<4)	//0x10
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<5)		//0x20
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<6)		//0x40
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<7)		//0x80
#define KEY_PRESSED_OFFSET_R ((uint16_t)0x01<<8)		//0x100
#define KEY_PRESSED_OFFSET_F ((uint16_t)0x01<<9)		//0x200
#define KEY_PRESSED_OFFSET_G ((uint16_t)0x01<<10)		//0x400
#define KEY_PRESSED_OFFSET_Z ((uint16_t)0x01<<11)		//0x800
#define KEY_PRESSED_OFFSET_X ((uint16_t)0x01<<12)		//0x1000
#define KEY_PRESSED_OFFSET_C ((uint16_t)0x01<<13)		//0x2000
#define KEY_PRESSED_OFFSET_V ((uint16_t)0x01<<14)		//0x4000
#define KEY_PRESSED_OFFSET_B ((uint16_t)0x01<<15)		//0x8000

typedef struct
{
	struct
	{
		uint16_t ch0;
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint8_t s1;
		uint8_t s2;
	}rc;
	
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	
	struct
	{
		uint16_t v;
	}key;
}RC_Ctl_t;

extern RC_Ctl_t RC_Ctl;
extern int16_t System_Num;

#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)	
void usart2_Init(void);
void Get_RC_Data(void);

#endif



