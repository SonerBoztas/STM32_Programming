/*
 * delay.c
 *
 *  Created on: 30 Nis 2020
 *      Author: soner.boztas
 */

#include "stm32f4xx.h"
#include "delay.h"

uint16_t counter = 0;

void SysTick_Handler(void)
{
	if(counter > 0)
		counter--;
}

void delay_ms(uint16_t time)
{
	counter = time;
	while(counter);
}

void clock_period_config()
{
	SysTick_Config(SystemCoreClock/1000);
}
