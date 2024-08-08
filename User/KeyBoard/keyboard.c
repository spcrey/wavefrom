#include ".\KeyBoard\keyboard.h"
#include ".\SysTick\SysTick.h"

static GPIO_TypeDef *KeyBoard_ROW_PORTS[4] = {KeyBoard_R1_PORT, KeyBoard_R2_PORT, KeyBoard_R3_PORT, KeyBoard_R4_PORT};

static GPIO_TypeDef *KeyBoard_COL_PORTS[4] = {KeyBoard_C1_PORT, KeyBoard_C2_PORT, KeyBoard_C3_PORT, KeyBoard_C4_PORT};
	
static uint16_t KeyBoard_ROW_PINS[4] = {KeyBoard_R1_PIN, KeyBoard_R2_PIN, KeyBoard_R3_PIN, KeyBoard_R4_PIN};

static uint16_t KeyBoard_COL_PINS[4] = {KeyBoard_C1_PIN, KeyBoard_C2_PIN, KeyBoard_C3_PIN, KeyBoard_C4_PIN};

static char keyBoard_Map[4][4] = {{'1', '2', '3', 'A' },{'4', '5', '6', 'B' },{'7', '8', '9', 'C' },{'*', '0', '#', 'D' }};

void KeyBoard_Init(void)
{
	RCC_APB2PeriphClockCmd(KeyBoard_R1_CLK|KeyBoard_R2_CLK|KeyBoard_R3_CLK|KeyBoard_R4_CLK,ENABLE);	 
	RCC_APB2PeriphClockCmd(KeyBoard_C1_CLK|KeyBoard_C2_CLK|KeyBoard_C3_CLK|KeyBoard_C4_CLK,ENABLE);	 
}
  
char KeyBoard_Value(void)
{
	uint8_t rowNum, colNum;
	
	GPIO_InitTypeDef GPIO_InitStructure; 	 
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	for(uint8_t i=0; i<4; i++)
	{
		GPIO_InitStructure.GPIO_Pin = KeyBoard_ROW_PINS[i];
		GPIO_Init(KeyBoard_ROW_PORTS[i], &GPIO_InitStructure);
	}
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	for(uint8_t i=0; i<4; i++)
	{
		GPIO_InitStructure.GPIO_Pin = KeyBoard_COL_PINS[i];
		GPIO_Init(KeyBoard_COL_PORTS[i], &GPIO_InitStructure);
		GPIO_ResetBits(KeyBoard_COL_PORTS[i], KeyBoard_COL_PINS[i]); 
	}
	 
	for(uint8_t i=0; i<4; i++)
	{
		if(GPIO_ReadInputDataBit(KeyBoard_ROW_PORTS[i],KeyBoard_ROW_PINS[i])==RESET)
		{
			rowNum = i;
			break;
		}
		rowNum = 4;
	}
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	for(uint8_t i=0; i<4; i++)
	{
		GPIO_InitStructure.GPIO_Pin = KeyBoard_COL_PINS[i];
		GPIO_Init(KeyBoard_COL_PORTS[i], &GPIO_InitStructure);
	}
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	for(uint8_t i=0; i<4; i++)
	{
		GPIO_InitStructure.GPIO_Pin = KeyBoard_ROW_PINS[i];
		GPIO_Init(KeyBoard_ROW_PORTS[i], &GPIO_InitStructure);
		GPIO_ResetBits(KeyBoard_ROW_PORTS[i], KeyBoard_ROW_PINS[i]); 
	}
	 
	for(uint8_t i=0; i<4; i++)
	{
		if(GPIO_ReadInputDataBit(KeyBoard_COL_PORTS[i],KeyBoard_COL_PINS[i])==RESET)
		{
			colNum = i;
			break;
		}
		colNum = 4;
	}
	
	if(rowNum==4)
		return 'E';
	else
		return keyBoard_Map[rowNum][colNum];
}
 
char KeyBoard_Scan(void)
{
	static char last_value= 'E';
	char value = KeyBoard_Value();
	
	if(value!=last_value)
	{
		SysTick_DelayMs(KeyBoard_WaitingTime);
		value = KeyBoard_Value();
		last_value = value;
		return value;
	}
	else
	{
		return 'F';
	}
}

