#include "usart5.h"  
#include<string.h>

/* TX */
#define    GPIO_TX                   GPIOC
#define    GPIO_PIN_TX               GPIO_Pin_12
#define    GPIO_PINSOURCE_TX         GPIO_PinSource12
#define    RCC_AHB1PERIPH_GPIO_TX    RCC_AHB1Periph_GPIOC

/* RX */
#define    GPIO_RX                   GPIOD
#define    GPIO_PIN_RX               GPIO_Pin_2
#define    GPIO_PINSOURCE_RX         GPIO_PinSource2
#define    RCC_AHB1PERIPH_GPIO_RX    RCC_AHB1Periph_GPIOD

/* DMA */
#define    DMA1_Stream_RX            DMA1_Stream0


//接收到的视觉数据暂存在这里
uint8_t  Com5_Vision_Buffer[ VISION_BUFFER_LEN ] = {0};

int Usart5_Clean_IDLE_Flag = 0;

void usart5_Init(void)
{
	GPIO_InitTypeDef   gpio;
	USART_InitTypeDef  usart5;
	DMA_InitTypeDef    dma1;
	NVIC_InitTypeDef   nvic;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE );
	RCC_AHB1PeriphClockCmd( RCC_AHB1PERIPH_GPIO_TX | RCC_AHB1PERIPH_GPIO_RX, ENABLE );
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_DMA1, ENABLE );
	
	GPIO_PinAFConfig( GPIO_TX, GPIO_PINSOURCE_TX, GPIO_AF_UART5);
	GPIO_PinAFConfig( GPIO_RX, GPIO_PINSOURCE_RX, GPIO_AF_UART5);
	
	gpio.GPIO_Pin    =  GPIO_PIN_TX;
	gpio.GPIO_Mode   =  GPIO_Mode_AF;
	gpio.GPIO_OType  =  GPIO_OType_PP;
	gpio.GPIO_PuPd   =  GPIO_PuPd_UP;
	gpio.GPIO_Speed  =  GPIO_Speed_100MHz;
	GPIO_Init( GPIO_TX, &gpio );
	gpio.GPIO_Pin    =  GPIO_PIN_RX;
	GPIO_Init( GPIO_RX, &gpio );
	
	USART_DeInit( UART5 );
	usart5.USART_BaudRate            = 115200;
	usart5.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	usart5.USART_Parity              = USART_Parity_No;
	usart5.USART_StopBits            = USART_StopBits_1;
	usart5.USART_WordLength          = USART_WordLength_8b;
	usart5.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init( UART5, &usart5);
	USART_Cmd( UART5, ENABLE );
	
	USART_ITConfig( UART5, USART_IT_IDLE, ENABLE );
	
	USART_DMACmd( UART5, USART_DMAReq_Rx, ENABLE );
	USART_DMACmd( UART5, USART_DMAReq_Tx, ENABLE );
	
	nvic.NVIC_IRQChannel                   = UART5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority        = 0;
	nvic.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&nvic);
	
	DMA_DeInit( DMA1_Stream_RX );
	dma1.DMA_Channel            = DMA_Channel_4;
	dma1.DMA_DIR                = DMA_DIR_PeripheralToMemory;
	dma1.DMA_Mode               = DMA_Mode_Circular;
	dma1.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	dma1.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
	dma1.DMA_BufferSize         = 100;
	dma1.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	dma1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma1.DMA_Memory0BaseAddr    = (uint32_t)Com5_Vision_Buffer;
	dma1.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
	dma1.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	dma1.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	dma1.DMA_Priority           = DMA_Priority_VeryHigh;
	dma1.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
	dma1.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
	
	DMA_Init( DMA1_Stream_RX, &dma1 );
	DMA_Cmd( DMA1_Stream_RX, ENABLE );
}


void UART5_IRQHandler(void)
{
	if( USART_GetITStatus( UART5, USART_IT_IDLE ) != RESET )
	{
		Usart5_Clean_IDLE_Flag = UART5->SR;
		Usart5_Clean_IDLE_Flag = UART5->DR;

		DMA_Cmd( DMA1_Stream0, DISABLE );
		
		Usart5_Clean_IDLE_Flag = VISION_BUFFER_LEN - DMA_GetCurrDataCounter( DMA1_Stream0 );
		
	  Vision_Read_Data(Com5_Vision_Buffer);//读取视觉数据
		
		memset( Com5_Vision_Buffer, 0, VISION_BUFFER_LEN ); //没加会导致数据接收不完全

		DMA_Cmd( DMA1_Stream0, ENABLE );
	}
}


extVisionSendHeader_t    VisionSendHeader;  //头
extVisionRecvData_t      VisionRecvData;    //视觉接收结构体
extVisionSendData_t      VisionSendData;    //视觉发送结构体

/**
  * @brief  串口一次发送一个字节数据
  * @param  数据
  * @retval void
  * @attention  8位
  */
void UART5_SendChar(uint8_t cData)
{
	while (USART_GetFlagStatus( UART5, USART_FLAG_TC ) == RESET);
	
	USART_SendData( UART5, cData );   
}




//视觉是否发了新数据,FALSE没有,TRUE发了新的
uint8_t Vision_Get_New_Data = FALSE;

/**
  * @brief  读取视觉信息
  * @param  usart4缓存数据
  * @retval void
  * @attention  IRQ执行
  */
void Vision_Read_Data(uint8_t *ReadFromUsart)
{
	//判断帧头数据是否为0xA5
	if(ReadFromUsart[0] == VISION_SOF)
	{
		//帧头CRC8校验
		if(Verify_CRC8_Check_Sum( ReadFromUsart, VISION_LEN_HEADER ) == TRUE)
		{
			//帧尾CRC16校验
			if(Verify_CRC16_Check_Sum( ReadFromUsart, VISION_LEN_PACKED ) == TRUE)
			{
				//接收数据拷贝
				memcpy( &VisionRecvData, ReadFromUsart, VISION_LEN_PACKED);	
				Vision_Get_New_Data = TRUE;//标记视觉数据更新了
			}
		}
	}
}


/**
  * @brief  发送视觉指令
  * @param  CmdID
  * @retval void
  * @attention  按协议打包好数据发送
  */
uint8_t vision_send_pack[50] = {0};//大于22就行
void Vision_Send_Data( uint8_t CmdID )
{
//	uint8_t vision_send_pack[50] = {0};//大于22就行
	int i;    //循环发送次数

	VisionSendHeader.SOF = VISION_SOF;
	VisionSendHeader.CmdID = CmdID;//对视觉来说最重要的数据
	
	//写入帧头
	memcpy( vision_send_pack, &VisionSendHeader, VISION_LEN_HEADER );
	
	//帧头CRC8校验协议
	Append_CRC8_Check_Sum( vision_send_pack, VISION_LEN_HEADER );
		
	memcpy( vision_send_pack + VISION_LEN_HEADER, &VisionSendData, VISION_LEN_DATA);
	
	//帧尾CRC16校验协议
	Append_CRC16_Check_Sum( vision_send_pack, VISION_LEN_PACKED );
	
	//将打包好的数据通过串口移位发送到裁判系统
	for (i = 0; i < VISION_LEN_PACKED; i++)
	{
		UART5_SendChar( vision_send_pack[i] );
	}
}

/**
  * @brief  判断视觉数据更新了吗
  * @param  void
  * @retval TRUE更新了   FALSE没更新
  * @attention  为自瞄做准备,串口空闲中断每触发一次且通过校验,则Vision_Get_New_Data置TRUE
  */
bool Vision_If_Update(void)
{
	return Vision_Get_New_Data;
}

/**
  * @brief  视觉数据更新标志位手动置0(false)
  * @param  void
  * @retval void
  * @attention  记得要清零,在哪清零自己选,调用这个函数就行
  */
void Vision_Clean_Update_Flag(void)
{
	Vision_Get_New_Data = FALSE;
}


