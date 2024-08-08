#include ".\Wave\wave.h"

static void MCP4725_DrawHeartUp(float x)
{
	x -= 2047;
	x /= 1000;
	x = fabs(x);
	x -= 1;
	x = pow(x, 2);
	x = 1 - x;
	x = sqrt(x);
	x = 1000 * x;
	x += 3071;
	MCP4725_WriteDigital(x);
}

static void MCP4725_DrawHeartDown(float x)
{
	x -= 2047;
	x /= 1000;
	x = fabs(x);
	x /= 2;
	x = 1 - x;
	x = sqrt(x);
	x *= -3;
	x *= 1000;
	x += 3071;
	MCP4725_WriteDigital(x);
}

void MCP4725_DrawHeart(void)
{
	for(uint16_t i=0; i<4096; i++)
	{
		if(i%2==0)
			MCP4725_DrawHeartUp((float)i);
		else
			MCP4725_DrawHeartDown((float)i);
	}
	for(uint32_t i=0; i<0x3FF; i++)
	{
		MCP4725_WriteDigital(3072);
	}
}

void Juchi(void)
{
	
	for(uint16_t i=0; i<4096; i++)
	{
		MCP4725_WriteDigital(i);
	}
}

void Shanpo(void)
{
	for(uint16_t i=0; i<2048; i++)
	{
		MCP4725_WriteDigital(2048-sqrt(2047*2047-(i)*(i)));
	}
	for(uint16_t i=2048; i<4095; i++)
	{
		MCP4725_WriteDigital(sqrt(2047*2047-(i-2047)*(i-2047)));
	}
}

void Xingyo(void)
{
	for(uint16_t i=0; i<2048; i++)
	{
		if(i%2==0)MCP4725_WriteDigital(4095+sqrt(2047*2047-(i)*(i)));
		else MCP4725_WriteDigital(-sqrt(2047*2047-(i)*(i))+2047);
	}
	for(uint16_t i=2048; i<4095; i++)
	{
		if(i%2==0)MCP4725_WriteDigital(4095+sqrt(2047*2047-(i-4095)*(i-4095)));
		else MCP4725_WriteDigital(-sqrt(2047*2047-(i-4095)*(i-4095))+2047);
	}
}

void Xing(void)
{
	for(uint16_t i=0; i<2048; i++)
	{
		if(i%2==0)MCP4725_WriteDigital(4095+sqrt(2047*2047-(i)*(i)));
		else MCP4725_WriteDigital(-sqrt(2047*2047-(i)*(i))+4095);
	}
	for(uint16_t i=2048; i<4095; i++)
	{
		if(i%2==0)MCP4725_WriteDigital(4095+sqrt(2047*2047-(i-4095)*(i-4095)));
		else MCP4725_WriteDigital(-sqrt(2047*2047-(i-4095)*(i-4095))+4095);
	}
}

void M(void)
{	
		for(uint16_t i=0; i<512; i++)
		if(i%2==0)MCP4725_WriteDigital(4095);
		else MCP4725_WriteDigital(0);
		for(uint16_t i=512; i<2048;i++)
			if(i%2==0)MCP4725_WriteDigital(5119-2*i);
		else MCP4725_WriteDigital(4095-2*i);
		for(uint16_t i=2048; i<3584;i++)
		 if(i%2==0)MCP4725_WriteDigital(-3076+2*i);
		else MCP4725_WriteDigital(-4095+2*i);
	for(uint16_t i=3584; i<4095; i++)
		if(i%2==0)MCP4725_WriteDigital(4095);
		else MCP4725_WriteDigital(0);
	MCP4725_WriteDigital(2047);	
	SysTick_DelayMs(60);
}

void KeyFunc_Init(void)
{
	keyfunc.menu_up = 'A';
	keyfunc.menu_down = 'B';
	keyfunc.value_dec = '4';
	keyfunc.value_inc = '6';
	keyfunc.input_goin = 'D';
	keyfunc.input_goout = 'D';
	keyfunc.spec_start = 'D';
	keyfunc.store_add = '*';
	keyfunc.store_goin = '#';
	keyfunc.store_goout = '#';
}

void Para_Init(void)
{
	KeyFunc_Init();
	
	/* the position of the first mark */
	section = MENU_LINE_0;
	
	/* the title of the menu */
	display.headline = "Waveform";
	display.lineArray[0].title = "Form: ";
	display.lineArray[1].title = "Freq: ";
	display.lineArray[2].title = "PkPk: ";
	display.lineArray[3].title = "Spec: ";
	
	/* the character string of mark */
	display.markString = "=>";
	DisplayMarkUpdate(&display);
	
	/* The times of special wave */
	spec.time = 0;
	
	/* add special wave */
	spec.fun = MCP4725_None;
	spec.name  = "None  ";
	AddSpecToStore(spec, &specstore);
	
	spec.fun = Juchi;
	spec.name = "Juchi ";
	AddSpecToStore(spec, &specstore);
	
	spec.fun = Shanpo;
	spec.name = "Shanpo";
	AddSpecToStore(spec, &specstore);
	
	spec.fun = Xingyo;
	spec.name = "Xingyo";
	AddSpecToStore(spec, &specstore);
	
	spec.fun = Xing;
	spec.name = "Xing  ";
	AddSpecToStore(spec, &specstore);
	
	spec.fun = M;
	spec.name = "M     ";
	AddSpecToStore(spec, &specstore);
	
	spec.fun = MCP4725_DrawHeart;
	spec.name = "Heart ";
	AddSpecToStore(spec, &specstore);
	
	
	/* the first special wave */
	spec = specstore.specArray[0];
}

int main(void)
{
	All_Init();
	Para_Init();
	//OLED_ShowString(0, 0, "fsdf");
	//while(1);
	EEPROMToWave(&wave);
	while(1)
	{
		char keyscan = KeyBoard_Scan();
		switch(section)
		{
			case MENU_LINE_0:
				if(keyscan==keyfunc.value_dec)
				{
					Form_Last(&wave);
				}
				else if(keyscan==keyfunc.value_inc)
				{
					Form_Next(&wave);
				}
				else if(keyscan==keyfunc.menu_down)
				{
					section = MENU_LINE_1;
				}
				else if(keyscan==keyfunc.store_add)
				{
					AddWaveToStore(wave, &wavestore);
				}
				else if(keyscan==keyfunc.store_goin)
				{
					GotoWaveStore(wavestore, display);	
				}
				break;
			case MENU_LINE_1:
				if(keyscan==keyfunc.value_dec)
				{
					if(wave.freq>100)
					{
						wave.freq -= 100;
					}
				}
				else if(keyscan==keyfunc.value_inc)
				{
					wave.freq += 100;
				}
				else if(keyscan==keyfunc.menu_up)
				{
					section = MENU_LINE_0;
				}
				else if(keyscan==keyfunc.menu_down)
				{
					section = MENU_LINE_2;
				}
				else if(keyscan==keyfunc.input_goin)
				{
					InputFreq(&wave, display);
				}
				else if(keyscan==keyfunc.store_add)
				{
					AddWaveToStore(wave, &wavestore);
				}
				else if(keyscan==keyfunc.store_goin)
				{
					GotoWaveStore(wavestore, display);	
				}
				break;
			case MENU_LINE_2:
				if(keyscan==keyfunc.value_dec)
				{
					if(wave.pkpk>100)
					{
						wave.pkpk -= 100;
					}
				}
				else if(keyscan==keyfunc.value_inc)
				{
					wave.pkpk += 100;
				}
				else if(keyscan==keyfunc.menu_up)
				{
					section = MENU_LINE_1;
				}
				else if(keyscan==keyfunc.menu_down)
				{
					section = MENU_LINE_3;
				}
				else if(keyscan==keyfunc.input_goin)
				{
					InputPkPk(&wave, display);
				}
				else if(keyscan==keyfunc.store_add)
				{
					AddWaveToStore(wave, &wavestore);
				}
				else if(keyscan==keyfunc.store_goin)
				{
					GotoWaveStore(wavestore, display);				
				}
				break;
			case MENU_LINE_3:
				if(keyscan==keyfunc.value_dec)
				{
					SelectSpecFromStore(&spec, &specstore, LAST);
				}
				else if(keyscan==keyfunc.value_inc)
				{
					SelectSpecFromStore(&spec, &specstore, NEXT);
				}
				else if(keyscan==keyfunc.menu_up)
				{
					section = MENU_LINE_2;
				}
				else if(keyscan==keyfunc.spec_start)
				{
					SpecTime(spec);
				}
				else if(keyscan==keyfunc.store_add)
				{
					AddWaveToStore(wave, &wavestore);
				}
				else if(keyscan==keyfunc.store_goin)
				{
					GotoWaveStore(wavestore, display);				
				}
				break;
		}
		All_Update();
	}
}
