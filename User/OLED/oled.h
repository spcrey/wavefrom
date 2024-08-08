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


void OLED_Init(void);  //��ʼ��
void OLED_ON(void);  //��Ļ��������ʼ����Ĭ��Ϊ����
void OLED_OFF(void);  //��Ļ�رգ���ʼ����Ĭ��Ϊ����
void OLED_ShowChar(unsigned char row, unsigned char col, char ch);  //�ڵ�row�У���col����ʾ�ַ�ch
// ��������������0��ʼ���������Ϊ7���������Ϊ21����������ʾ���ַ���ͬ��
void OLED_ShowString(unsigned char row, unsigned char col, char *str);  //�Ե�row�У���col��Ϊ�����ʾ�ַ���str
void OLED_Cls(void);  //����
char *U32ToString(uint32_t num);  //��һ���޷���32λ����ת��Ϊһ���ַ���


#ifdef __cplusplus
 }
#endif

#endif /* __OLED_H */
