#include "stm32f4xx.h"

GPIO_InitTypeDef	GPIO_InitStructure;
EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef	NVIC_InitStructure;

#define button_1	GPIO_Pin_2
#define button_2	GPIO_Pin_3

#define led		GPIO_Pin_5
#define led_1		GPIO_Pin_8
#define led_2		GPIO_Pin_9

// PC2 ve PC3 2 Harici Buton (button_1, button_2)
// PA5 Dahili Led (led)
// PB8 ve PB9 2 Harici Led	(led_1, led_2)
void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB , ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void EXTI_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, GPIO_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, GPIO_PinSource3);

	EXTI_InitStructure.EXTI_Line	= EXTI_Line2 | EXTI_Line3;
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_InitStructure.EXTI_Mode	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger	= EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel	= EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;

    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel      	= EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;

    NVIC_Init(&NVIC_InitStructure);

}

void delay(uint32_t time)
{
	while(time--);
}

void EXTI2_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		GPIO_ResetBits(GPIOB, led_1 | led_2);

		GPIO_SetBits(GPIOB, led_1);
		delay(8400000);
		GPIO_SetBits(GPIOB, led_2);
		delay(8400000);

		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI3_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		GPIO_ResetBits(GPIOB, led_1 | led_2);

		GPIO_SetBits(GPIOB, led_2);
		delay(8400000);
		GPIO_SetBits(GPIOB, led_1);
		delay(8400000);

		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}
int main(void)
{

	GPIO_Config();
	EXTI_Config();

  while (1)
  {
	GPIO_SetBits(GPIOA, led);
	GPIO_ResetBits(GPIOB, led_1 | led_2);
  }
}
