#include "system.h"

extern int SystemMode;
static volatile uint32_t usTicks = 0;
extern int16_t Angle_Target;

uint32_t currentTime = 0;
uint32_t loopTime_1ms=0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0; 

PID_t PID;

//�޷�
float constrain(float amt, float low, float high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}
int32_t constrain_int32(int32_t amt, int32_t low, int32_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int16_t constrain_int16(int16_t amt, int16_t low, int16_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int constrain_int(int amt,int low,int high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

//��������ʼ��
static void cycleCounterInit(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	usTicks = clocks.SYSCLK_Frequency / 1000000; 
}

//��΢��Ϊ��λ����ϵͳʱ��
uint32_t micros(void)
{
	register uint32_t ms, cycle_cnt;
	do {
			ms = sysTickUptime;
			cycle_cnt = SysTick->VAL;
	} while (ms != sysTickUptime);
	return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

//΢�뼶��ʱ
void delay_us(uint32_t us)
{
	uint32_t now = micros();
	while (micros() - now < us);
}

//���뼶��ʱ
void delay_ms(uint32_t ms)
{
	while (ms--)
			delay_us(1000);
}

//�Ժ���Ϊ��λ����ϵͳʱ��
uint32_t millis(void)
{
	return sysTickUptime;
}

//ϵͳ��ʼ��
void systemInit(void)
{
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);	//�δ�ʱ�����ã�1ms
}

int SystemMonitor=Normal_Mode;
void Stop()
{
	CLOSE_SPRAY;
}

void Parameter_Init(void)
{      
	Spray_Init();
}

void System_Init(void)
{	
	CRC_init();	
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);
	
	Parameter_Init();
	usart2_Init();
	usart5_Init();
	Spray_Init();
	Led_Init();
}

//��ѭ��
void Loop(void)
{	
	static uint32_t currentTime = 0;
	static uint32_t loopTime_1ms = 0;
	
	currentTime = micros();	//��ȡ��ǰϵͳʱ��
	
	if((int32_t)(currentTime - loopTime_1ms) >= 0)  
	{	
		loopTime_1ms = currentTime + 1000;	//1ms		
		if(SystemMonitor == Normal_Mode)
			{
				Spray_Control();
			}
	}
}








