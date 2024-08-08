#include ".\Wave\wave.h"

/* Parameter */
Section section;
Wave wave;
Display display;
WaveStore wavestore;
Spec spec;
SpecStore specstore;
KeyFunc keyfunc;

void DAC0832_SetPKPK(uint16_t pkpk)
{
	
}

void All_Init(void)
{
	OLED_Init();
	KeyBoard_Init();
	EEPROM_Init();
	MCP4725_Init();
	AD9834_Init();
	SysTick_Init();
	Relay_Init();
	
	wavestore.waveNum = 0;
	specstore.specNum = 0;
	specstore.nowNum = 0;
}

void All_Update(void)
{
	PkPk_Update(&wave);
	Wave_Update(wave);
	WaveToDisPlay(wave, &display);
	SpecToDisplay(spec, &display);
	Display_Update(display);
	WaveToEEPROM(wave);
	SectionToDisplay(&display, section);
}

void Form_Next(Wave *wave)
{
	if(wave->form==TriangleWave)
	{
		wave->form = SquareWave;
	}
	else if(wave->form==SineWave)
	{
		wave->form = TriangleWave;
	}
	else
	{
		wave->form = SineWave;
	}
}

void Form_Last(Wave *wave)
{
	if(wave->form==TriangleWave)
	{
		wave->form = SineWave;
	}
	else if(wave->form==SineWave)
	{
		wave->form = SquareWave;
	}
	else
	{
		wave->form = TriangleWave;
	}
}

char *WaveformToString(Waveform form)
{
	static char *triangleString = "triangle";
	static char *sineString     = "sine    ";
	static char *sqrareString   = "square  ";
	if(form==TriangleWave)
	{
		return triangleString;
	}
	else if(form==SineWave)
	{
		return sineString;
	}
	else
	{
		return sqrareString;
	}
}

void PkPk_Update(Wave *wave)
{
	if(wave->form==TriangleWave||wave->form==SineWave)
	{
		if(wave->freq<33000)
		{
			wave->pkpk = 610;
		}
		else if(wave->freq<100000)
		{
			wave->pkpk = 600;
		}
		else if(wave->freq<150000)
		{
			wave->pkpk = 590;
		}
		else if(wave->freq<175000)
		{
			wave->pkpk = 580;
		}
		else
		{
			wave->pkpk = 570;
		}
	}
	else
	{
		wave->pkpk = 3400;
	}
}

void Wave_Update(Wave wave)
{
	Relay_SelectWave(wave.form);
	AD9834_SetFreq(wave.freq);
	DAC0832_SetPKPK(wave.pkpk);
}

void WaveToDisPlay(Wave wave, Display *display)
{
	char *freq, *pkpk;
	freq = LongToString(wave.freq);
	pkpk = U32ToString(wave.pkpk);
	StringToString6(freq);
	StringToString6(pkpk);
	display->lineArray[0].text = WaveformToString(wave.form);
	display->lineArray[1].text = freq;
	display->lineArray[2].text = pkpk;
}

void SectionToDisplay(Display *display, Section section)
{
	display->mark = (uint8_t)section;
}

void DisplayMarkUpdate(Display *display)
{
	uint8_t markNum=0;
	
	while(*(display->markString+markNum)!='\0')
	{
		markNum += 1;
	}
	display->markNum = markNum;
	if(markNum==1)
	{
		display->markStringNone = " ";
	}
	else if(markNum==2)
	{
		display->markStringNone = "  ";
	}
	else if(markNum==3)
	{
		display->markStringNone = "   ";
	}
}

void Display_Update(Display display)
{
	OLED_ShowString(0, display.markNum, display.headline);
	for(uint8_t i=0; i<=3; i++)
	{
		if(display.mark==i)
		{
			OLED_ShowString(i+2, 0, display.markString);
		}
		else
		{
			OLED_ShowString(i+2, 0, display.markStringNone);
		}
		OLED_ShowString(i+2, display.markNum, display.lineArray[i].title);
		OLED_ShowString(i+2, display.markNum+6, display.lineArray[i].text);
	}
	
}

void WaveToEEPROM(Wave wave)
{
	uint8_t formArray[4], freqArray[8], pkpkArray[4];
	
	Ui32ToU8Array((uint32_t)wave.form, formArray);
	Ul64ToU8Array(wave.freq, freqArray);
	Ui32ToU8Array(wave.pkpk, pkpkArray);
	
	for(uint8_t i=0; i<4; i++)
	{
		EEPROM_Buffer[i] = formArray[i];
	}
	
	for(uint8_t i=0; i<8; i++)
	{
		EEPROM_Buffer[i+4] = freqArray[i];
	}
	
	for(uint8_t i=0; i<4; i++)
	{
		EEPROM_Buffer[i+12] = pkpkArray[i];
	}
	
	EEPROM_Upload(0, 16);
}

void EEPROMToWave(Wave *wave)
{
	uint8_t formArray[4], freqArray[8], pkpkArray[4];
	
	for(uint8_t i=0; i<4; i++)
	{
		formArray[i] = EEPROM_Buffer[i];
	}
	
	for(uint8_t i=0; i<8; i++)
	{
		freqArray[i] = EEPROM_Buffer[i+4];
	}
	
	for(uint8_t i=0; i<4; i++)
	{
		pkpkArray[i] = EEPROM_Buffer[i+12];
	}
	
	U8ArrayToUi32(formArray, (u32*)(&wave->form));
	U8ArrayToUl64(freqArray, &wave->freq);
	U8ArrayToUi32(pkpkArray, &wave->pkpk);
}

void InputFreq(Wave *wave, Display display)
{
	uint8_t num=0;
	uint8_t array[6];
	char temChar[7] = "      ";
	display.lineArray[1].text = "      ";
	while(1)
	{
		char keyscan = KeyBoard_Scan();
		if(keyscan>='0'&&keyscan<='9'&&num<6)
		{
			array[num] = keyscan-48;
			temChar[num] = keyscan;
			display.lineArray[1].text = temChar;
			num += 1;
		}
		if(keyscan==keyfunc.input_goout)
		{
			wave->freq = 0;
			for(uint8_t i=0; i<num; i++)
				wave->freq += array[i]*pow(10, num-i-1);
			break;
		}
		Display_Update(display);
	}
}

void InputPkPk(Wave *wave, Display display)
{
	uint8_t num=0;
	uint8_t array[4];
	char temChar[7] = "      ";
	display.lineArray[2].text = "      ";
	while(1)
	{
		char keyscan = KeyBoard_Scan();
		if(keyscan>='0'&&keyscan<='9'&&num<4)
		{
			array[num] = keyscan-48;
			temChar[num] = keyscan;
			display.lineArray[2].text = temChar;
			num += 1;
		}
		if(keyscan==keyfunc.input_goout)
		{
			wave->pkpk = 0;
			for(uint8_t i=0; i<num; i++)
				wave->pkpk += array[i]*pow(10, num-i-1);
			break;
		}
		Display_Update(display);
	}
}

void AddWaveToStore(Wave wave, WaveStore *wavestore)
{
	wavestore->waveArray[wavestore->waveNum] = wave;
	wavestore->waveNum += 1;
}

void GotoWaveStore(WaveStore wavestore, Display display)
{
	uint8_t num=wavestore.waveNum-1;
	Wave wave;
	display.mark = 4;
	display.lineArray[3].title = "      ";
	display.lineArray[3].text  = "      ";
	while(1)
	{
		if(wavestore.waveNum==0)
		{
			break;
		}
		char keyscan = KeyBoard_Scan();
		if(keyscan==keyfunc.value_dec)
		{
			if(num>0)
			{
				num -= 1;
			}
		}
		else if(keyscan==keyfunc.value_inc)
		{
			if(num<wavestore.waveNum-1)
			{
				num += 1;
			}
		}
		if(keyscan==keyfunc.store_goin)
		{
			break;
		}
		wave = wavestore.waveArray[num];
		Wave_Update(wave);
		WaveToDisPlay(wave, &display);
		Display_Update(display);
	}
}

void AddSpecToStore(Spec spec, SpecStore *specstore)
{
	specstore->specArray[specstore->specNum] = spec;
	specstore->specNum += 1;
}

void SelectSpecFromStore(Spec *spec, SpecStore *specstore, uint8_t state)
{
	
	if(state==1)
	{
		if(specstore->nowNum<specstore->specNum-1)
		{
			specstore->nowNum += 1;
		}
	}
	else
	{
		if(specstore->nowNum>0)
		{
			specstore->nowNum -= 1;
		}
	}
	*spec = specstore->specArray[specstore->nowNum];
}

void SpecToDisplay(Spec spec, Display *display)
{
	display->lineArray[3].text = spec.name;
}

void SpecTime(Spec spec)
{
	for(uint16_t i=0; i<=spec.time; i++)
	{
		spec.fun();
	}
}
