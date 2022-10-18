#include "delay.h"


static void delay_us_core(void)
{
	unsigned int i;
	for(i=0;i<7;i++) __asm { nop };
}


void delay_ms(uint16_t ms)
{
	uint16_t i;
	for(i=0;i<ms;i++)
	{
		delay_us(980);
	}
}


void delay_s(uint16_t s)
{
	uint16_t i;
	for(i=0;i<s;i++)
	{
		delay_ms(1000);
	}
}


void delay_us(uint16_t us)
{
	uint16_t i;
	for(i=0;i<us;i++)
	{
		delay_us_core();
	}
}


