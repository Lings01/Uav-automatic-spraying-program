#ifndef __USART5_H
#define __USART5_H

#include "system.h"

/* ���ջ��� */
#define    VISION_BUFFER_LEN    100   //��΢�����

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
	
	
/*--------------------------------�ݶ�Э��-------------------------------------*/

#define    VISION_LENGTH        22     		 //�ݶ�22�ֽ�,ͷ3�ֽ�,����17�ֽ�,β2�ֽ�

//��ʼ�ֽ�,Э��̶�Ϊ0xA5
#define    VISION_SOF         (0xA5)

//���ȸ���Э�鶨��,���ݶγ���Ϊn��Ҫ����֡ͷ�ڶ��ֽ�����ȡ
#define    VISION_LEN_HEADER    3         //֡ͷ��
#define    VISION_LEN_DATA      17        //���ݶγ���,���Զ���
#define    VISION_LEN_TAIL      2	      //֡βCRC16
#define    VISION_LEN_PACKED    22        //���ݰ�����,���Զ���

#define    VISION_OFF           (0x00)


//�������պͷ���ָ������бȽ�,���պͷ���ָ������ͬʱ,���ж�Ϊ���ݿ���

//֡ͷ��CRC8У��,��֤���͵�ָ������ȷ��

//PC�շ���STM32�շ��ɾ����ϵ,���½ṹ��������STM32,PC�������޸�

typedef __packed struct
{
	/* ͷ */
	uint8_t   SOF;			//֡ͷ��ʼλ,�ݶ�0xA5
	uint8_t   CmdID;		//ָ��
	uint8_t   CRC8;			//֡ͷCRCУ��,��֤���͵�ָ������ȷ��
	
}extVisionSendHeader_t;


//STM32����,ֱ�ӽ����ڽ��յ������ݿ������ṹ��
typedef __packed struct
{
	/* ͷ */
	uint8_t   SOF;			//֡ͷ��ʼλ,�ݶ�0xA5
	uint8_t   CmdID;		//ָ��
	uint8_t   CRC8;			//֡ͷCRCУ��,��֤���͵�ָ������ȷ��
	
	/* ���� */
	float     pitch_angle;
	float     yaw_angle;
	float     distance;			
	uint8_t   blank_d;		
	uint8_t	  identify_target;  //�Ƿ�ʶ��Ŀ��
	
	uint8_t   blank_a;			//Ԥ��
	uint8_t	  blank_b;
	uint8_t	  blank_c;
	
	
	/* β */
	uint16_t  CRC16;       
	
}extVisionRecvData_t;



//STM32����,ֱ�ӽ�����õ�����һ���ֽ�һ���ֽڵط��ͳ�ȥ
typedef __packed struct
{
//	/* ͷ */
//	uint8_t   SOF;			//֡ͷ��ʼλ,�ݶ�0xA5
//	uint8_t   CmdID;		//ָ��
//	uint8_t   CRC8;			//֡ͷCRCУ��,��֤���͵�ָ������ȷ��
	
	/* ���� */  
	uint8_t    test_communicate;		 //�����ܷ�������

	
	/* β */
	uint16_t  CRC16;
	
}extVisionSendData_t;	
		
extern extVisionRecvData_t VisionRecvData;

#endif

