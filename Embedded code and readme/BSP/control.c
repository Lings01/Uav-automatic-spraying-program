#include "control.h"

//失控保护
void Get_SystemMode()	//使用时换成int型返回数值
{

}

float Connected_Time = 0;
bool debug_flag = 0;



/**
  *@brief  喷洒装置控制
  *@param  void
  *@retval void
  *@attention
  */
void Spray_Control(void)
{
	if( VisionRecvData.identify_target == TRUE ) //视觉识别到目标为薇甘菊
	{
		Connected_Time++;
	}
	else
	{
		Connected_Time = 0;
	}
	
	if( Connected_Time >= 150 )//稳定识别150ms后开始喷洒
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
  *@brief  喷洒装置初始化
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
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //自制是pp
	  GPIO_Init(GPIOB,&GPIO_InitStructure); 
}

