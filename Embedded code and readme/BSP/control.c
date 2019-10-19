#include "control.h"

//ʧ�ر���
void Get_SystemMode()	//ʹ��ʱ����int�ͷ�����ֵ
{

}

float Connected_Time = 0;
bool debug_flag = 0;



/**
  *@brief  ����װ�ÿ���
  *@param  void
  *@retval void
  *@attention
  */
void Spray_Control(void)
{
	if( VisionRecvData.identify_target == TRUE ) //�Ӿ�ʶ��Ŀ��Ϊޱ�ʾ�
	{
		Connected_Time++;
	}
	else
	{
		Connected_Time = 0;
	}
	
	if( Connected_Time >= 150 )//�ȶ�ʶ��150ms��ʼ����
	{
		OPEN_SPRAY;
		debug_flag = 1;
	}
	else
	{
		CLOSE_SPRAY;
		debug_flag = 0;
	}
}


/**
  *@brief  ����װ�ó�ʼ��
  *@param  void
  *@retval void
  *@attention
  */
void Spray_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //������pp
	  GPIO_Init(GPIOB,&GPIO_InitStructure); 
}

