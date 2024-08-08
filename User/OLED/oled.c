#include ".\OLED\oled.h"
#include ".\SysTick\systick.h"
#include "codetab.h"

#define I2C_TIMEOUT		1000
static __IO uint16_t I2C_TIME;

static void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(OLED_SCL_CLK|OLED_SDA_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure; 

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	
	GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN;
	GPIO_Init(OLED_SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
	GPIO_Init(OLED_SDA_PORT, &GPIO_InitStructure);	
}

static void I2C_Config(void)
{
	
	OLED_I2C_CLK_FUN(OLED_I2C_CLK, ENABLE);

	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x0A;  
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = OLED_I2C_SPEED;	                          
  
	I2C_Init(OLED_I2C, &I2C_InitStructure);	                                  
	
	I2C_Cmd(OLED_I2C, ENABLE);  	                                        
}

static uint8_t OLED_WriteByte(uint8_t addr,uint8_t data)
{
	I2C_TIME = 0;
  	while(I2C_GetFlagStatus(OLED_I2C, I2C_FLAG_BUSY))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(OLED_I2C, ENABLE);  
			return 0;
		}
	}
	
	I2C_GenerateSTART(OLED_I2C, ENABLE);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(OLED_I2C, ENABLE);  
			return 0;
		}
	}

	I2C_Send7bitAddress(OLED_I2C, OLED_I2C_ADDRESS, I2C_Direction_Transmitter);
	I2C_TIME = 0;
	while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(OLED_I2C, ENABLE);  
			return 0;
		}
	}

	I2C_SendData(OLED_I2C, addr);
	I2C_TIME = 0;
	while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(OLED_I2C, ENABLE);  
			return 0;
		}
	}

	I2C_SendData(OLED_I2C, data);
	I2C_TIME = 0;
	while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		I2C_TIME += 1;
		if(I2C_TIME==I2C_TIMEOUT)
		{
			I2C_GenerateSTOP(OLED_I2C, ENABLE);  
			return 0;
		}
	}
	
	I2C_GenerateSTOP(OLED_I2C, ENABLE);
	return 1;
}

/**
  * @brief  Write command
  * @param  I2C_Command: the command writing to I2C
  */
static void OLED_WriteCmd(unsigned char I2C_Command)
{
	OLED_WriteByte(0x00, I2C_Command);
}

static void OLED_WriteDat(unsigned char I2C_Data)
{
	OLED_WriteByte(0x40, I2C_Data);
}


 /**
  * @brief  Make OLED work
  */

void OLED_ON(void)
{
	OLED_WriteCmd(0X8D);  //设置电荷泵
	OLED_WriteCmd(0X14);  //开启电荷泵
	OLED_WriteCmd(0XAF);  //OLED唤醒
}


 /**
  * @brief  Make OLED sleep
  */
void OLED_OFF(void)
{
	OLED_WriteCmd(0X8D);  //设置电荷泵
	OLED_WriteCmd(0X10);  //关闭电荷泵
	OLED_WriteCmd(0XAE);  //OLED休眠
}

static void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd((x&0x0f)|0x01);
}

static void OLED_Fill(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WriteCmd(0xb0+m);		//page0-page1
		OLED_WriteCmd(0x00);		//low column start address
		OLED_WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WriteDat(fill_Data);
		}
	}
}


void OLED_Init(void)
{
	GPIO_Config();
	I2C_Config();
	SysTick_DelayS(1);		// 1s,这里的延时很重要,上电后延时，没有错误的冗余设计
	
	OLED_WriteCmd(0xAE); //display off
	OLED_WriteCmd(0x20);	//Set Memory Addressing Mode	
	OLED_WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_WriteCmd(0xc8);	//Set COM Output Scan Direction
	OLED_WriteCmd(0x00); //---set low column address
	OLED_WriteCmd(0x10); //---set high column address
	OLED_WriteCmd(0x40); //--set start line address
	OLED_WriteCmd(0x81); //--set contrast control register
	OLED_WriteCmd(0xff); //亮度调节 0x00~0xff
	OLED_WriteCmd(0xa1); //--set segment re-map 0 to 127
	OLED_WriteCmd(0xa6); //--set normal display
	OLED_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	OLED_WriteCmd(0x3F); //
	OLED_WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WriteCmd(0xd3); //-set display offset
	OLED_WriteCmd(0x00); //-not offset
	OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0xf0); //--set divide ratio
	OLED_WriteCmd(0xd9); //--set pre-charge period
	OLED_WriteCmd(0x22); //
	OLED_WriteCmd(0xda); //--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb); //--set vcomh
	OLED_WriteCmd(0x20); //0x20,0.77xVcc
	OLED_WriteCmd(0x8d); //--set DC-DC enable
	OLED_WriteCmd(0x14); //
	OLED_WriteCmd(0xaf); //--turn on OLED panel
	OLED_Cls();
}

/**
  * @brief  Make OLED show character;
  * @param 	row: the row
  * @param 	col: the col
  * @param 	ch: the character to show
  * @retval None
  */

void OLED_ShowChar(unsigned char row, unsigned char col, char ch)
{	
	if(col<=20 && row<=7)
	{
		OLED_SetPos(6*col, row);
		for(unsigned char i=0;i<6;i++)
		{
			OLED_WriteDat(OLED_F6x8[ch-32][i]);
		}
	}
}

/**
  * @brief  Make OLED show character string;
  * @param 	row: the row
  * @param 	col: the col
  * @param 	str: the character string to show
  * @retval None
  */
void OLED_ShowString(unsigned char row, unsigned char col, char *str)
{
	unsigned int i=0;
	while(str[i]!=0)
	{
		OLED_ShowChar(row, col+i, str[i]);
		i++;
	}
}


/**
  * @brief  Clear the OLED;
  * @param  None
  * @retval None
  */
void OLED_Cls(void)
{
	OLED_Fill(0x00);
}

