#include ".\SysTick\systick.h"

void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/100000)==1)
	{ 
		while (1);
	}
}

void SysTick_DelayUs(__IO uint32_t us)
{
	SysTick_Config(SystemCoreClock/1000000);
	
	for(uint32_t i=0; i<us; i++)
	{
		while(!((SysTick->CTRL)&(1<<16)));
	}
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_DelayMs(__IO uint32_t ms)
{
	SysTick_Config(SystemCoreClock/1000);
	
	for(uint32_t i=0; i<ms; i++)
	{
		while(!((SysTick->CTRL)&(1<<16)));
	}
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

