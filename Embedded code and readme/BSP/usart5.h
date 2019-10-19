#ifndef __USART5_H
#define __USART5_H

#include "system.h"

/* 接收缓存 */
#define    VISION_BUFFER_LEN    100   //稍微给大点

#define  RED   1
#define  BlUE  0

#define  FALSE 0
#define  TRUE  1

void usart5_Init(void);
void UART5_SendChar(uint8_t cData);


void Vision_Read_Data(uint8_t *ReadFromUsart);
void Vision_Send_Data( uint8_t CmdID );
bool Vision_If_Update(void);
void Vision_Clean_Update_Flag(void);
	
	
/*--------------------------------暂定协议-------------------------------------*/

#define    VISION_LENGTH        22     		 //暂定22字节,头3字节,数据17字节,尾2字节

//起始字节,协议固定为0xA5
#define    VISION_SOF         (0xA5)

//长度根据协议定义,数据段长度为n需要根据帧头第二字节来获取
#define    VISION_LEN_HEADER    3         //帧头长
#define    VISION_LEN_DATA      17        //数据段长度,可自定义
#define    VISION_LEN_TAIL      2	      //帧尾CRC16
#define    VISION_LEN_PACKED    22        //数据包长度,可自定义

#define    VISION_OFF           (0x00)


//可利用收和发的指令码进行比较,当收和发的指令码相同时,可判定为数据可用

//帧头加CRC8校验,保证发送的指令是正确的

//PC收发与STM32收发成镜像关系,以下结构体适用于STM32,PC需稍作修改

typedef __packed struct
{
	/* 头 */
	uint8_t   SOF;			//帧头起始位,暂定0xA5
	uint8_t   CmdID;		//指令
	uint8_t   CRC8;			//帧头CRC校验,保证发送的指令是正确的
	
}extVisionSendHeader_t;


//STM32接收,直接将串口接收到的数据拷贝进结构体
typedef __packed struct
{
	/* 头 */
	uint8_t   SOF;			//帧头起始位,暂定0xA5
	uint8_t   CmdID;		//指令
	uint8_t   CRC8;			//帧头CRC校验,保证发送的指令是正确的
	
	/* 数据 */
	float     pitch_angle;
	float     yaw_angle;
	float     distance;			
	uint8_t   blank_d;		
	uint8_t	  identify_target;  //是否识别到目标
	
	uint8_t   blank_a;			//预留
	uint8_t	  blank_b;
	uint8_t	  blank_c;
	
	
	/* 尾 */
	uint16_t  CRC16;       
	
}extVisionRecvData_t;



//STM32发送,直接将打包好的数据一个字节一个字节地发送出去
typedef __packed struct
{
//	/* 头 */
//	uint8_t   SOF;			//帧头起始位,暂定0xA5
//	uint8_t   CmdID;		//指令
//	uint8_t   CRC8;			//帧头CRC校验,保证发送的指令是正确的
	
	/* 数据 */  
	uint8_t    test_communicate;		 //测试能否发送数据

	
	/* 尾 */
	uint16_t  CRC16;
	
}extVisionSendData_t;	
		
extern extVisionRecvData_t VisionRecvData;

#endif

