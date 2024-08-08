#include ".\AD9834\ad9834.h" 

static void AD9834_Write16Bits(unsigned int data) 
{ 
    AD9834_SCK_HIGH();
    AD9834_FSY_LOW();

    for(uint8_t i=0 ;i<16 ;i++)      
	{ 
		if(data & 0x8000) 
			AD9834_SDA_HIGH(); 
        else 
			AD9834_SDA_LOW(); 

        AD9834_SCK_LOW();
        data <<= 1; 
        AD9834_SCK_HIGH();
    } 
    AD9834_SDA_LOW(); 
    AD9834_FSY_HIGH();
} 

void AD9834_SelectWave(Waveform form)
{ 
	AD9834_FSY_HIGH();
	AD9834_SCK_HIGH();
	AD9834_RST_HIGH();
	AD9834_RST_HIGH();
	AD9834_RST_LOW();
	
    AD9834_Write16Bits((unsigned int)form); 
} 

static void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(AD9834_SCK_CLK|AD9834_SDA_CLK|AD9834_RST_CLK|AD9834_FSY_CLK|AD9834_FS_CLK|AD9834_PS_CLK, ENABLE);	
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	
	GPIO_InitStructure.GPIO_Pin = AD9834_SCK_PIN;
	GPIO_Init(AD9834_SCK_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD9834_SDA_PIN;
	GPIO_Init(AD9834_SDA_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD9834_RST_PIN;
	GPIO_Init(AD9834_RST_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD9834_FSY_PIN;
	GPIO_Init(AD9834_FSY_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD9834_FS_PIN;
	GPIO_Init(AD9834_FS_PORT, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = AD9834_PS_PIN;
	GPIO_Init(AD9834_PS_PORT, &GPIO_InitStructure); 
}

void AD9834_Init()
{
	GPIO_Config();
	
	AD9834_Write16Bits(0x2100);
	AD9834_Write16Bits(0x2038);
	AD9834_Write16Bits(0XC000);
	AD9834_Write16Bits(0x2100);
	
	AD9834_SetFreq(100);
} 

static void AD9834_SetFreqN(unsigned char number, unsigned long freq)
{
    unsigned long FREQREG = (unsigned long)(268435456.0/AD9834_SYSCLK*freq); 
    unsigned int FREQREG_LSB_14BIT = (unsigned int)FREQREG; 
    unsigned int FREQREG_MSB_14BIT = (unsigned int)(FREQREG>>14); 
	
    if(number==AD9834_Sine1) 
    { 
        FREQREG_LSB_14BIT &= ~(1U<<15); 
        FREQREG_LSB_14BIT |= 1<<14; 
        FREQREG_MSB_14BIT &= ~(1U<<15); 
        FREQREG_MSB_14BIT |= 1<<14; 
    }
    else
    {
        FREQREG_LSB_14BIT &= ~(1<<14); 
        FREQREG_LSB_14BIT |= 1U<<15; 
        FREQREG_MSB_14BIT &= ~(1<<14); 
        FREQREG_MSB_14BIT |= 1U<<15; 
    } 
    AD9834_Write16Bits(FREQREG_LSB_14BIT); 
    AD9834_Write16Bits(FREQREG_MSB_14BIT);      
} 

void AD9834_SetFreq(unsigned long freq)
{
	AD9834_SetFreqN(0, freq);
}

