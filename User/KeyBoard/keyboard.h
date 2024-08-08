#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"


#define KeyBoard_WaitingTime				20


#define KeyBoard_R1_PORT					GPIOD
#define KeyBoard_R1_CLK						RCC_APB2Periph_GPIOD
#define KeyBoard_R1_PIN						GPIO_Pin_14
		
#define KeyBoard_R2_PORT					GPIOD
#define KeyBoard_R2_CLK						RCC_APB2Periph_GPIOD
#define KeyBoard_R2_PIN						GPIO_Pin_13
		
#define KeyBoard_R3_PORT					GPIOD
#define KeyBoard_R3_CLK						RCC_APB2Periph_GPIOD
#define KeyBoard_R3_PIN						GPIO_Pin_12
	
#define KeyBoard_R4_PORT					GPIOD
#define KeyBoard_R4_CLK						RCC_APB2Periph_GPIOD
#define KeyBoard_R4_PIN						GPIO_Pin_11

#define KeyBoard_C1_PORT					GPIOD
#define KeyBoard_C1_CLK						RCC_APB2Periph_GPIOD
#define KeyBoard_C1_PIN						GPIO_Pin_15
				 
#define KeyBoard_C2_PORT					GPIOG
#define KeyBoard_C2_CLK						RCC_APB2Periph_GPIOG
#define KeyBoard_C2_PIN						GPIO_Pin_2
				 
#define KeyBoard_C3_PORT					GPIOG
#define KeyBoard_C3_CLK						RCC_APB2Periph_GPIOG
#define KeyBoard_C3_PIN						GPIO_Pin_3
				 
#define KeyBoard_C4_PORT					GPIOG
#define KeyBoard_C4_CLK						RCC_APB2Periph_GPIOG
#define KeyBoard_C4_PIN						GPIO_Pin_4


void KeyBoard_Init(void);
char KeyBoard_Value(void);
char KeyBoard_Scan(void);


#ifdef __cplusplus
 }
#endif

#endif /* BB_KEYBOARD_H */

