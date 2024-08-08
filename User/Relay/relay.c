#include ".\Relay\relay.h"
#include ".\SysTick\systick.h"

void Relay_Init(void)
{
	RCC_APB2PeriphClockCmd(Relay_CLK, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure; 	 
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Relay_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(Relay_PORT, &GPIO_InitStructure);
	
	Relay_SelectWave(SineWave);
}

void Relay_SelectWave(Waveform form)
{
	if(form==TriangleWave||form==SineWave)
	{
		GPIO_SetBits(Relay_PORT, Relay_PIN);
	}
	else
	{
		GPIO_ResetBits(Relay_PORT, Relay_PIN);
	}
	AD9834_SelectWave(form);
}


