/*
 * led.c
 *
 *  Created on: 30 Nis 2020
 *      Author: soner.boztas
 */

#include "stm32f4xx.h"
#include "led.h"

#define led GPIO_Pin_5

void led_config()
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void led_on()
{
	GPIO_SetBits(GPIOA, led);
}

void led_off()
{
	GPIO_ResetBits(GPIOA, led);
}

void led_toggle()
{
	GPIO_ToggleBits(GPIOA, led);
}
