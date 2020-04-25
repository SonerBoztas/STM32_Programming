#include "stm32f4xx.h"

GPIO_InitTypeDef	GPIO_InitStructure;
EXTI_InitTypeDef	EXTI_InitStructure;
NVIC_InitTypeDef	NVIC_InitStructure;

void GPIO_Config()
{


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		// PC2 PC3 2 BUTTON

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= 	GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= 	GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &InitStructure);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= 	GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= 	GPIO_Speed_100MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin		= 	GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed	= 	GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void EXTI_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);			// EXTERNAL INTERRUPT Clock Hattı

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, GPIO_PinSource2); 	// EXTI Hattını Aktif Etme
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, GPIO_PinSource3); 	// EXTI Hattını Aktif Etme

	EXTI_InitStructure.EXTI_Line		=	EXTI_Line2 | EXTI_Line3;
	EXTI_InitStructure.EXTI_LineCmd		=	ENABLE;
	EXTI_InitStructure.EXTI_Mode		=	EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger		=	EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStructure);

	// NVIC: INTERRUPT YÖNETİCİSİ

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel		=	EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd	=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;

	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel		=	EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd	=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			=	0;

	NVIC_Init(&NVIC_InitStructure);

	/*
	 * "NVIC_IRQChannelPreemptionPriority" ve "NVIC_IRQChannelSubPriority" komutları ile öncelik ayarlaması yapılır.
	 * Burada öncelik 2 butona da aynı anda basıldığında hangi butonun öncelikli çalışacağını belirler.
	 * Priority işlemlerinde sayı değeri küçüldükçe, öncelik yükselmektedir.
	 * Bu uygulamada da görüceleği üzere PC2' ye bağlı butonun önceliği daha yüksektir.
	 */
}

void delay(uint32_t time)
{
	while(time--);
}

void EXTI2_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);

		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		delay(8400000);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		delay(8400000);

		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI3_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);

		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		delay(8400000);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
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
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
  }
}
