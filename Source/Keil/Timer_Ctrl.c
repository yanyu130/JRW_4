#include "Timer_Ctrl.h"

volatile uint32_t tick_counter = 0;
uint32_t frame_counter = 0;

void SysTick_Handler(void)	//每1ms一次
{
	tick_counter++;
//	if(tick_counter % 1000 == 0) get = 1;
}

void setup_system_tick(uint32_t sampleRate)
{
	uint32_t tickPeriod = SystemCoreClock/sampleRate;
	
	/* SysTick source to HCLK/2 */
  CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HCLK_DIV2);
	
	SysTick_Config(tickPeriod);
	//ChronographStart(ChronMain);
	printf("SystemCoreClock:%d\n",SystemCoreClock);
	printf("Tick Time: %d us\n",1000000/sampleRate);

}

uint32_t millis(void)	//返回系统时间，单位 ：毫秒
{
	return tick_counter;
}


void DelayMsec(uint8_t time)
{
	static uint8_t currentTime = 0;
	currentTime = tick_counter;
	while(tick_counter <= (currentTime + time));
	
}

void IncFrameCount(uint8_t inc)
{
	frame_counter+=inc;
}

uint32_t GetFrameCount(void)
{
	return frame_counter;
}

