#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"


void SysTick_Init(void);
void SysTick_DelayUs(__IO uint32_t us);
void SysTick_DelayMs(__IO uint32_t ms);
#define SysTick_DelayS(x)			SysTick_DelayMs(1000*x)

	 
#ifdef __cplusplus
}
#endif 

#endif /* __SYSTICK_H */
