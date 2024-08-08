#ifndef __Relay_H
#define __Relay_H

#ifdef	cplusplus
 extern "C"	{
#endif
			 
#include "stm32f10x.h"	
#include ".\AD9834\ad9834.h"  
	 

#define Relay_PORT				GPIOD
#define Relay_PIN				GPIO_Pin_4
#define Relay_CLK				RCC_APB2Periph_GPIOD
	 

void Relay_Init(void); 
void Relay_SelectWave(Waveform form);
	 
	 
#ifdef	cplusplus
 }
#endif			
 
#endif /* __Relay_H */


