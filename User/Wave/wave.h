#ifndef __WAVEFORM_H
#define __WAVEFORM_H

#ifdef	cplusplus
 extern "C"	{
#endif
		

/* Includes */
#include "stm32f10x.h"
#include <string.h>
#include ".\OLED\oled.h"
#include ".\KeyBoard\keyboard.h"
#include ".\EEPROM\eeprom.h"
#include ".\AD9834\ad9834.h" 
#include ".\MCP4725\mcp4725.h"	
#include ".\SysTick\systick.h"
#include ".\Relay\relay.h"
#include ".\MyMath\mymath.h"	


#define NEXT			1
#define LAST			(!NEXT)


/* the relative struction */
typedef enum {
	MENU_LINE_0=0, MENU_LINE_1, MENU_LINE_2, MENU_LINE_3,  STORE
}Section;
	
typedef struct{
	Waveform form;
	unsigned long freq;
	uint32_t pkpk;
}Wave;

typedef struct{
	char *title;
	char *text;
}Line;

typedef struct {
	char *headline;
	Line lineArray[4];
	uint8_t mark;
	char *markString;
	char *markStringNone;
	uint8_t markNum;
}Display;

typedef struct {
	Wave waveArray[10];
	uint8_t waveNum;
}WaveStore;

typedef struct {
	void (*fun)(void);
	char *name;
	uint16_t time;
} Spec;

typedef struct {
	Spec specArray[10];
	uint8_t specNum;
	uint8_t nowNum;
} SpecStore;

typedef struct {
	char menu_up;
	char menu_down;
	char value_inc;
	char value_dec;
	char input_goin;
	char input_goout;
	char store_add;
	char store_goin;
	char store_goout;
	char spec_start;
} KeyFunc;


/* Parameter */
extern Section section;
extern Wave wave;
extern Display display;
extern WaveStore wavestore;
extern Spec spec;
extern SpecStore specstore;
extern KeyFunc keyfunc;


/* Function */
void All_Init(void);
void All_Update(void);
void PkPk_Update(Wave *wave);
void Form_Next(Wave *wave);
void Form_Last(Wave *wave);
void Wave_Update(Wave wave);
void DisplayMarkUpdate(Display *display);
void WaveToDisPlay(Wave wave, Display *display);
void SectionToDisplay(Display *display, Section section);
void Display_Update(Display display);
void WaveToEEPROM(Wave wave);
void EEPROMToWave(Wave *wave);
void InputFreq(Wave *wave, Display display);
void InputPkPk(Wave *wave, Display display);
void AddWaveToStore(Wave wave, WaveStore *wavestore);
void GotoWaveStore(WaveStore wavestore, Display display);
void AddSpecToStore(Spec spec, SpecStore *specstore);
void SelectSpecFromStore(Spec *spec, SpecStore *specstore, uint8_t state);
void SpecToDisplay(Spec spec, Display *display);
void SpecTime(Spec spec);
	

#ifdef	cplusplus
 }
#endif			
 
#endif /* __WAVEFORM_H */


