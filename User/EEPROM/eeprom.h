#ifndef __EEPROM_H
#define	__EEPROM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"


#define EEPROM_I2C_SPEED              		400000 
#define EEPROM_I2C_ADDRESS       			0xA0  
#define EEPROM_PageSize        				8 // AT24C01/02 has 8 byte in a page


#define EEPROM_I2C                          I2C1
#define EEPROM_I2C_CLK_FUN                  RCC_APB1PeriphClockCmd
#define EEPROM_I2C_CLK                      RCC_APB1Periph_I2C1

#define EEPROM_SCL_CLK          	       	RCC_APB2Periph_GPIOB     
#define EEPROM_SCL_PORT         	        GPIOB   
#define EEPROM_SCL_PIN          	        GPIO_Pin_6
	
#define EEPROM_SDA_CLK          	       	RCC_APB2Periph_GPIOB 
#define EEPROM_SDA_PORT         	        GPIOB 
#define EEPROM_SDA_PIN          	        GPIO_Pin_7


extern uint8_t EEPROM_Buffer[256];

void EEPROM_Init(void);  
void EEPROM_Upload(uint8_t startAddr, uint16_t endAddr);
void EEPROM_Download(uint8_t startAddr, uint16_t endAddr); 


#define EEPROM_UploadAll()				EEPROM_Upload(0, 255)    
#define EEPROM_DownloadAll()			EEPROM_Download(0, 255)  


#ifdef __cplusplus
 }
#endif

#endif /* __EEPROM_H */
