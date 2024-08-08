#ifndef __VARCONVERS_H
#define __VARCONVERS_H

#ifdef	cplusplus
 extern "C"	{
#endif
			 
#include "stm32f10x.h"	
	 
	 
char *U32ToString(uint32_t num);
char *LongToString(uint32_t num);
void Ul64ToU8Array(unsigned long ul64, uint8_t ui8[8]);
void U8ArrayToUl64(uint8_t ui8[8], unsigned long *ul64);
void Ui32ToU8Array(uint32_t ui32, uint8_t ui8[4]);
void U8ArrayToUi32(uint8_t ui8[4], uint32_t *ui32);
void StringToString6(char *str);
	 
	 
#ifdef	cplusplus
 }
#endif			
 
#endif /* __VARCONVERS_H */


