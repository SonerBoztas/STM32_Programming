#include "stm32f4xx.h"

GPIO_InitTypeDef		GPIO_InitStructure;
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
TIM_OCInitTypeDef		TIM_OCInitStructure;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
 *******************************************************************
 *        (Period + 1) x (Prescaler + 1)                     1	   *
 *   T = --------------------------------        --->  F = -----   *
 *            (Clock Freq / DivX)                            T     *
 *******************************************************************

* NUCLE0 F401RE SYSTEM CLOCK FREQ = 84 MHZ
* FOR APB1 BUSLINE, CLOCK DIVIDER = 2
* APB1 BUSLINE FREQ = 42 MHZ
*
* PLL_M = 16
* PLL_N = 336
* PLL_P = 4
* PLL_Q = 4
*
*/
void PWM_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Prescaler		= 41;
	TIM_TimeBaseInitStructure.TIM_Period		= 999;
	TIM_TimeBaseInitStructure.TIM_ClockDivision	= TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode	= TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode				= TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNPolarity			= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState			= TIM_OutputState_Enable;

	TIM_Cmd(TIM3, ENABLE);

}

void delay(uint32_t time)
{
	while(time--);
}

/*
 * PULSE LENGTH = [ ((TIM_PERIOD + 1) x DUTY_CYCLE) / 100 ] - 1
 *
 * %25 DC 	- PULSE LENGTH = [((999 + 1) x 25) 	/ 100 ] - 1 = 249
 * %50 DC 	- PULSE LENGTH = [((999 + 1) x 50)	/ 100 ] - 1 = 449
 * %75 DC 	- PULSE LENGTH = [((999 + 1) x 75)	/ 100 ] - 1 = 749
 * %100 DC 	- PULSE LENGTH = [((999 + 1) x 100)	/ 100 ] - 1 = 999
 */

int main(void)
{
	GPIO_Config();
	PWM_Config();

	TIM_OCInitStructure.TIM_Pulse = 0;

  while (1)
  {

	for(int i=0; i<1000; i++)
	{
		TIM_OCInitStructure.TIM_Pulse = i;
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		delay(42000);
	}
	for(int i=999; i>0; i--)
	{
		TIM_OCInitStructure.TIM_Pulse = i;
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		delay(42000);
	}
  }
}
