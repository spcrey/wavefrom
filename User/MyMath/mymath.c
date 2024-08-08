#include ".\MyMath\mymath.h"

char *U32ToString(uint32_t num)
{
	static char temString[11];
	uint8_t numArray[11], n=0;
	if(num==0)
	{
		return "0\0";
	}
	while(num>0)
	{
		numArray[n] = num % 10;
		num /= 10;
		n++;
	}
	for(uint8_t i=0; i<n; i++)
	{
		temString[i] = numArray[n-i-1] + 48;
	}
	temString[n] = 0;
	return temString;
}

char *LongToString(uint32_t num)
{
	static char temString[22];
	uint8_t numArray[22], n=0;
	if(num==0)
	{
		return "0\0";
	}
	while(num>0)
	{
		numArray[n] = num % 10;
		num /= 10;
		n++;
	}
	for(uint8_t i=0; i<n; i++)
	{
		temString[i] = numArray[n-i-1] + 48;
	}
	temString[n] = 0;
	return temString;
}

void Ul64ToU8Array(unsigned long ul64, uint8_t ui8[8])
{
	for(uint8_t i=0; i<8; i++)
	{
		ui8[i] = (ul64&((unsigned long)0xff)<<(14-2*i)*4)>>(14-2*i)*4;
	}
}

void U8ArrayToUl64(uint8_t ui8[8], unsigned long *ul64)
{
	*ul64 = (unsigned long)0;
	for(uint8_t i=0; i<8; i++)
	{
		*ul64 += (unsigned long)ui8[i]<<((14-2*i)*4);
	}
}

void Ui32ToU8Array(uint32_t ui32, uint8_t ui8[4])
{
	for(uint8_t i=0; i<4; i++)
	{
		ui8[i] = (ui32&((uint32_t)0xff)<<(6-2*i)*4)>>(6-2*i)*4;
	}
}

void U8ArrayToUi32(uint8_t ui8[4], uint32_t *ui32)
{
	*ui32 = 0;
	for(uint8_t i=0; i<4; i++)
	{
		*ui32 += (uint32_t)ui8[i]<<((6-2*i)*4);
	}
}

void StringToString6(char *str)
{
	uint8_t num;
	for(num=0; num<7; num++)
	{
		if(*(str+num)=='\0')
		{
			break;
		}
	}
	for(uint8_t i=num; i<6; i++)
	{
		str[i] = ' ';
	}
	str[6]= '\0';
}
