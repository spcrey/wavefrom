#ifndef __MCP4725_H
#define __MCP4725_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
#include <math.h>


#define MCP4725_I2C_SPEED              		400000 
#define MCP4725_I2C_ADDRESS       			0XC0


#define MCP4725_I2C                        	I2C1
#define MCP4725_I2C_CLK_FUN                 RCC_APB1PeriphClockCmd
#define MCP4725_I2C_CLK                     RCC_APB1Periph_I2C1

#define MCP4725_SCL_CLK          	       	RCC_APB2Periph_GPIOB     
#define MCP4725_SCL_PORT         	        GPIOB   
#define MCP4725_SCL_PIN          	        GPIO_Pin_6
	
#define MCP4725_SDA_CLK          	       	RCC_APB2Periph_GPIOB 
#define MCP4725_SDA_PORT         	        GPIOB 
#define MCP4725_SDA_PIN          	        GPIO_Pin_7


void MCP4725_Init(void);
uint8_t MCP4725_WriteDigital(uint16_t data);
void MCP4725_None(void);
void MCP4725_DrawCircle(void);
void MCP4725_DrawHeart(void);


#ifdef __cplusplus
 }
#endif	 
	 
#endif /* __MCP4725_H */

