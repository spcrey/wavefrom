#include ".\EEPROM\eeprom.h"		

uint8_t EEPROM_Buffer[256];

#define I2C_TIMEOUT		1000
static __IO uint16_t I2C_TIME;

static void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(EEPROM_SCL_CLK|EEPROM_SDA_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure; 
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	GPIO_InitStructure.GPIO_Pin = EEPROM_SCL_PIN;     
	GPIO_Init(EEPROM_SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EEPROM_SDA_PIN; 
	GPIO_Init(EEPROM_SDA_PORT, &GPIO_InitStructure);
}

static void I2C_Config(void)
{
	EEPROM_I2C_CLK_FUN(EEPROM_I2C_CLK, ENABLE);
	
	I2C_InitTypeDef I2C_InitStructure; 

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x0A; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C_SPEED;
	
	I2C_Init(EEPROM_I2C, &I2C_InitStructure);
	
	I2C_Cmd(EEPROM_I2C, ENABLE);   
}

void EEPROM_Init(void)
{
	GPIO_Config(); 
	I2C_Config();
	EEPROM_DownloadAll();	
}

static uint8_t EEPROM_WaitEepromStandbyState(void)      
{
	vu16 SR1_Tmp = 0;

	I2C_TIME = 0;
	do
	{
		I2C_GenerateSTART(EEPROM_I2C, ENABLE);
		SR1_Tmp = I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1);
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDRESS, I2C_Direction_Transmitter);
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}while(!(I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1)&0x0002));
  
	I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE); 
	return 1;
}

static uint8_t EEPROM_WritePage(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
	I2C_TIME = 0;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
  
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
  
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDRESS, I2C_Direction_Transmitter);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
	 
	I2C_SendData(EEPROM_I2C, WriteAddr);  
	I2C_TIME = 0;
	while(! I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}

	while(NumByteToWrite--)  
	{
		I2C_SendData(EEPROM_I2C, *pBuffer); 
		pBuffer++; 
		I2C_TIME = 0;
		while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if(I2C_TIME++==I2C_TIMEOUT)
			{
				I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
				return 0;
			}
		}
	}

	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
	return 1;
}

static void EEPROM_WriteBuffer(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	
	Addr = WriteAddr % EEPROM_PageSize;
	count = EEPROM_PageSize - Addr;
	NumOfPage =  NumByteToWrite / EEPROM_PageSize;
	NumOfSingle = NumByteToWrite % EEPROM_PageSize;
 
	if(Addr == 0) 
	{
		if(NumOfPage == 0) 
		{
			EEPROM_WritePage(pBuffer, WriteAddr, NumOfSingle);
			EEPROM_WaitEepromStandbyState();
		}
		else  
		{
			while(NumOfPage--)
			{
				EEPROM_WritePage(pBuffer, WriteAddr, EEPROM_PageSize); 
				EEPROM_WaitEepromStandbyState();
				WriteAddr +=  EEPROM_PageSize;
				pBuffer += EEPROM_PageSize;
			}

			if(NumOfSingle!=0)
			{
				EEPROM_WritePage(pBuffer, WriteAddr, NumOfSingle);
				EEPROM_WaitEepromStandbyState();
			}
		}
	}
	else 
	{
		if(NumOfPage== 0) 
		{
			EEPROM_WritePage(pBuffer, WriteAddr, NumOfSingle);
			EEPROM_WaitEepromStandbyState();
		}
		else
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / EEPROM_PageSize;
			NumOfSingle = NumByteToWrite % EEPROM_PageSize;	
      
			if(count != 0)
			{  
				EEPROM_WritePage(pBuffer, WriteAddr, count);
				EEPROM_WaitEepromStandbyState();
				WriteAddr += count;
				pBuffer += count;
			} 
      
			while(NumOfPage--)
			{
				EEPROM_WritePage(pBuffer, WriteAddr, EEPROM_PageSize);
				EEPROM_WaitEepromStandbyState();
				WriteAddr +=  EEPROM_PageSize;
				pBuffer += EEPROM_PageSize;  
			}
			if(NumOfSingle != 0)
			{
				EEPROM_WritePage(pBuffer, WriteAddr, NumOfSingle); 
				EEPROM_WaitEepromStandbyState();
			}
		}
	}  
}



static uint8_t EEPROM_ReadBuffer(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
	I2C_TIME = 0;
	while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
	I2C_TIME = 0;
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
	
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDRESS, I2C_Direction_Transmitter);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
	
	I2C_Cmd(EEPROM_I2C, ENABLE);
	
	I2C_SendData(EEPROM_I2C, ReadAddr);  
	I2C_TIME = 0;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
	
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
	
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_I2C_ADDRESS, I2C_Direction_Receiver);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if(I2C_TIME++==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
			return 0;
		}
	}
	
	while(NumByteToRead)  
	{
		if(NumByteToRead == 1)
		{
			I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
			
			I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
		}
		I2C_TIME = 0;
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
		{
			if(I2C_TIME++==I2C_TIMEOUT)
			{
				I2C_GenerateSTOP(EEPROM_I2C, ENABLE);  
				return 0;
			}
		}  
		*pBuffer = I2C_ReceiveData(EEPROM_I2C);
		pBuffer++; 
		NumByteToRead--;        
	}
	I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
	return 0;
}

void EEPROM_Upload(uint8_t startAddr, uint16_t endAddr)
{
	EEPROM_WriteBuffer(EEPROM_Buffer, startAddr, endAddr-startAddr+1);
}

void EEPROM_Download(uint8_t startAddr, uint16_t endAddr)
{
	EEPROM_ReadBuffer(EEPROM_Buffer, startAddr, endAddr-startAddr+1);
}


