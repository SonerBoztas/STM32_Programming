#include "stm32f4xx.h"

GPIO_InitTypeDef		GPIO_InitStructure;
TIM_TimeBaseInitTypeDef		TIM_TimeBaseInitStructure;
NVIC_InitTypeDef		NVIC_InitStructure;

#define led GPIO_Pin_5

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
 *******************************************************************
 *        (Period + 1) x (Prescaler + 1)                     1	   *
 *   T = --------------------------------        --->  F = -----   *
 *                   (Clock Freq)                            T     *
 *******************************************************************
 *								   *
 *	       (Period + 1) x (Prescaler + 1) x (RC + 1)     	   *
 *   T = ---------------------------------------------             *
 *                    (Clock Freq )                                *
 * 	                                                           *
 *                 FOR ADVANCED TIMERS ONLY                        *
 *******************************************************************
 *
 * NUCLE0 F401RE SYSTEM CLOCK FREQ = 84 MHZ
 * FOR APB1 BUSLINE, CLOCK DIVIDER = 2
 * APB1 BUSLINE FREQ = 42 MHZ
 *
 * HSI = 16 MHz
 * PLL_M = 16
 * PLL_N = 336
 * PLL_P = 4
 * PLL_Q = 4
 *
 */

void TIM2_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitStructure.TIM_Period 		= 9999;
	TIM_TimeBaseInitStructure.TIM_CounterMode	= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler		= 4199;
	TIM_TimeBaseInitStructure.TIM_ClockDivision	= TIM_CKD_DIV1;

	// TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_Cmd(TIM2, ENABLE);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel	= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	 = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority	   	 = 0;

	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		GPIO_ToggleBits(GPIOA, led);
	}
}

int main(void)
{
	GPIO_Config();
    TIM2_Config();
  while (1)
  {

  }
}
