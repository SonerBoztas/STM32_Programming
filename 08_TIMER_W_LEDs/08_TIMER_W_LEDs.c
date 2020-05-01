#include "stm32f4xx.h"

#define led_1 GPIO_Pin_5
#define led_2 GPIO_Pin_6
#define led_3 GPIO_Pin_8
#define led_4 GPIO_Pin_9

GPIO_InitTypeDef		GPIO_InitStructure;
TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
NVIC_InitTypeDef		NVIC_InitStructure;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	= 	GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= 	GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void TIM2_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/*
	 *******************************************************************
	 *        (Period + 1) x (Prescaler + 1)                     1	   *
	 *   T = --------------------------------        --->  F = -----   *
	 *               (Clock Freq)                                T     *
	 *******************************************************************


	 *******************************************************************
	 * NUCLE0 F401RE SYSTEM CLOCK FREQ = 84 MHZ
	 * APB1 BUSLINE FREQ = 42 MHz
	 *
	 * PLL_M = 16
	 * PLL_N = 336
	 * PLL_P = 4
	 * PLL_Q = 4
	 *
	 */

	TIM_TimeBaseInitStructure.TIM_Prescaler			= 4199;
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period			= 9999;
	TIM_TimeBaseInitStructure.TIM_ClockDivision		= TIM_CKD_DIV4;
	// TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_Cmd(TIM2, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel			= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;

	NVIC_Init(&NVIC_InitStructure);

}

void led()
{
	if(TIM_GetCounter(TIM2) < 2501)
		{
			GPIO_SetBits(GPIOC, led_1);
			GPIO_ResetBits(GPIOC, led_2);
			GPIO_ResetBits(GPIOC, led_3);
			GPIO_ResetBits(GPIOC, led_4);
		}

		else if(TIM_GetCounter(TIM2)>2500 && TIM_GetCounter(TIM2) < 5001)
		{
			GPIO_ResetBits(GPIOC, led_1);
			GPIO_SetBits(GPIOC, led_2);
			GPIO_ResetBits(GPIOC, led_3);
			GPIO_ResetBits(GPIOC, led_4);
		}
		else if(TIM_GetCounter(TIM2)>5000 && TIM_GetCounter(TIM2) < 7501)
		{
			GPIO_ResetBits(GPIOC, led_1);
			GPIO_ResetBits(GPIOC, led_2);
			GPIO_SetBits(GPIOC, led_3);
			GPIO_ResetBits(GPIOC, led_4);
		}
		else if(TIM_GetCounter(TIM2)>7500 && TIM_GetCounter(TIM2) < 10000)
		{
			GPIO_ResetBits(GPIOC, led_1);
			GPIO_ResetBits(GPIOC, led_2);
			GPIO_ResetBits(GPIOC, led_3);
			GPIO_SetBits(GPIOC, led_4);
		}
}

int main(void)
{
  GPIO_Config();
  TIM2_Config();

  while (1)
  {
	led();
  }
}
