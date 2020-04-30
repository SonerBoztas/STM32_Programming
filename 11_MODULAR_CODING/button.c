/*
 * button.c
 *
 *  Created on: 30 Nis 2020
 *      Author: soner.boztas
 */

#include "stm32f4xx.h"
#include "button.h"

#define button GPIO_Pin_13

void button_config()
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t button_read()
{
	GPIO_ReadInputDataBit(GPIOC, button);
}
