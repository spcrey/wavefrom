#include ".\MCP4725\mcp4725.h"

#define I2C_TIMEOUT		1000
static __IO uint16_t I2C_TIME;

static void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(MCP4725_SCL_CLK|MCP4725_SDA_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure; 
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	GPIO_InitStructure.GPIO_Pin = MCP4725_SCL_PIN;     
	GPIO_Init(MCP4725_SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MCP4725_SDA_PIN; 
	GPIO_Init(MCP4725_SDA_PORT, &GPIO_InitStructure);
}

static void I2C_Config(void)
{
	MCP4725_I2C_CLK_FUN(MCP4725_I2C_CLK, ENABLE);
	
	I2C_InitTypeDef I2C_InitStructure; 

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = MCP4725_I2C_ADDRESS; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = MCP4725_I2C_SPEED;
	
	I2C_Init(MCP4725_I2C, &I2C_InitStructure);
	
	I2C_Cmd(MCP4725_I2C, ENABLE);   
}

void MCP4725_Init(void)
{
	GPIO_Config(); 
	I2C_Config();
}

uint8_t MCP4725_WriteDigital(uint16_t data) 
{
	uint8_t data_H = (0x0F00&data) >> 8;
	uint8_t data_L = 0X00FF&data ;
		
	I2C_TIME = 0;
	while(I2C_GetFlagStatus(MCP4725_I2C, I2C_FLAG_BUSY))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(MCP4725_I2C, ENABLE);  
			return 0;
		}
	}
	
	
	I2C_GenerateSTART(MCP4725_I2C, ENABLE);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(MCP4725_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(MCP4725_I2C, ENABLE);  
			return 0;
		}
	}
	   
	I2C_Send7bitAddress(MCP4725_I2C, MCP4725_I2C_ADDRESS, I2C_Direction_Transmitter);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(MCP4725_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(MCP4725_I2C, ENABLE);  
			return 0;
		}
	}
	    
	I2C_SendData(MCP4725_I2C, data_H);
	I2C_TIME = 0;
	while (!I2C_CheckEvent(MCP4725_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(MCP4725_I2C, ENABLE);  
			return 0;
		}
	}

	I2C_SendData(MCP4725_I2C, data_L);
	I2C_TIME = 0;
	while (!I2C_CheckEvent(MCP4725_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(MCP4725_I2C, ENABLE);  
			return 0;
		}
	}
	
	I2C_GenerateSTOP(MCP4725_I2C, ENABLE);  
	return 1;
}

void MCP4725_None(void)
{

}

void MCP4725_DrawCircle(void)
{
	for(uint16_t i=0; i<4096; i++)
	{
		if(i%2==0)
			MCP4725_WriteDigital(2047+sqrt(2047*2047-(i-2047)*(i-2047)));
		else
			MCP4725_WriteDigital(2048-sqrt(2047*2047-(i-2047)*(i-2047)));
	}
}


