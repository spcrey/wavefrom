#ifndef __AD9834_H
#define __AD9834_H

#ifdef	cplusplus
 extern "C"	{
#endif
			 
#include "stm32f10x.h"	 
	 

#define AD9834_SYSCLK     				75000000UL 


#define AD9834_SCK_PORT					GPIOA
#define AD9834_SCK_PIN					GPIO_Pin_4
#define AD9834_SCK_CLK					RCC_APB2Periph_GPIOA

#define AD9834_SDA_PORT					GPIOA
#define AD9834_SDA_PIN					GPIO_Pin_5
#define AD9834_SDA_CLK					RCC_APB2Periph_GPIOA

#define AD9834_RST_PORT					GPIOA
#define AD9834_RST_PIN					GPIO_Pin_6
#define AD9834_RST_CLK					RCC_APB2Periph_GPIOA

#define AD9834_FSY_PORT					GPIOA
#define AD9834_FSY_PIN					GPIO_Pin_3
#define AD9834_FSY_CLK					RCC_APB2Periph_GPIOA

#define AD9834_FS_PORT					GPIOA
#define AD9834_FS_PIN					GPIO_Pin_0
#define AD9834_FS_CLK					RCC_APB2Periph_GPIOA

#define AD9834_PS_PORT					GPIOA
#define AD9834_PS_PIN					GPIO_Pin_1
#define AD9834_PS_CLK					RCC_APB2Periph_GPIOA

#define AD9834_SCK_HIGH()				GPIO_SetBits(AD9834_SCK_PORT ,AD9834_SCK_PIN) 
#define AD9834_SDA_HIGH()				GPIO_SetBits(AD9834_SDA_PORT ,AD9834_SDA_PIN) 
#define AD9834_RST_HIGH()				GPIO_SetBits(AD9834_RST_PORT ,AD9834_RST_PIN) 
#define AD9834_FSY_HIGH()				GPIO_SetBits(AD9834_FSY_PORT ,AD9834_FSY_PIN)
#define AD9834_FS_HIGH()				GPIO_SetBits(AD9834_FS_PORT ,AD9834_FS_PIN) 
#define AD9834_PS_HIGH()				GPIO_SetBits(AD9834_PS_PORT ,AD9834_PS_PIN)

#define AD9834_SCK_LOW()				GPIO_ResetBits(AD9834_SCK_PORT ,AD9834_SCK_PIN) 
#define AD9834_SDA_LOW()				GPIO_ResetBits(AD9834_SDA_PORT ,AD9834_SDA_PIN) 
#define AD9834_RST_LOW()				GPIO_ResetBits(AD9834_RST_PORT ,AD9834_RST_PIN) 
#define AD9834_FSY_LOW()				GPIO_ResetBits(AD9834_FSY_PORT ,AD9834_FSY_PIN)
#define AD9834_FS_LOW()					GPIO_ResetBits(AD9834_FS_PORT ,AD9834_FS_PIN) 
#define AD9834_PS_LOW()					GPIO_ResetBits(AD9834_PS_PORT ,AD9834_PS_PIN)


#define AD9834_Sine1				    0 
#define AD9834_Sine2				    1


typedef enum{
	TriangleWave=0x2002, SineWave=0x2008, SquareWave=0x2028
}Waveform;


void AD9834_Init(void);  					
void AD9834_SelectWave(Waveform form);
void AD9834_SetFreq(unsigned long freq); 
	 
#ifdef	cplusplus
 }
#endif			
 
#endif /* __AD9834_H */


