#include "stm32f4xx.h"

GPIO_InitTypeDef	GPIO_InitStructure;
EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef	NVIC_InitStructure;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void EXTI_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);

	EXTI_InitStructure.EXTI_Line	= EXTI_Line3;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_InitStructure.EXTI_Mode	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger	= EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel	= EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;

	NVIC_Init(&NVIC_InitStructure);
}

void delay(uint32_t time)
{
	while(time--);
}

void EXTI3_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		delay(840000);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		delay(840000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		delay(840000);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		delay(840000);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		delay(840000);

		EXTI_ClearITPendingBit(EXTI_Line3);  
	}
}
int main()
{
	GPIO_Config();
	EXTI_Config();

	while(1)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
}
