#ifndef __OLED_H
#define	__OLED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"


#define OLED_I2C_ADDRESS				0x78  
#define OLED_I2C_SPEED            		400000  


#define OLED_I2C                        I2C1
#define OLED_I2C_CLK                    RCC_APB1Periph_I2C1
#define OLED_I2C_CLK_FUN				RCC_APB1PeriphClockCmd

#define OLED_SCL_PIN                	GPIO_Pin_6                 
#define OLED_SCL_PORT          			GPIOB                       
#define OLED_SCL_CLK           			RCC_APB2Periph_GPIOB

#define OLED_SDA_PIN                	GPIO_Pin_7                  
#define OLED_SDA_PORT          			GPIOB                       
#define OLED_SDA_CLK          	 		RCC_APB2Periph_GPIOB


void OLED_Init(void);  //初始化
void OLED_ON(void);  //屏幕开启，初始化后默认为开启
void OLED_OFF(void);  //屏幕关闭，初始化后默认为开启
void OLED_ShowChar(unsigned char row, unsigned char col, char ch);  //在第row行，第col列显示字符ch
// 行数，列数均从0开始，行数最大为7，列数最大为21，超出则不显示，字符串同理
void OLED_ShowString(unsigned char row, unsigned char col, char *str);  //以第row行，第col列为起点显示字符串str
void OLED_Cls(void);  //清屏
char *U32ToString(uint32_t num);  //将一个无符号32位整型转化为一个字符串


#ifdef __cplusplus
 }
#endif

#endif /* __OLED_H */
