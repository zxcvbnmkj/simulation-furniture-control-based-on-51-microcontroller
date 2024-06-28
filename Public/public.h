#ifndef _public_H
#define _public_H

#include "reg52.h" 
#include "intrins.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;


typedef enum 
{
	DISABLE = 0, ENABLE = !DISABLE
}FunctionalState;


void delay_10us(u16 us);
void delay_ms(u16 ms);


#endif
