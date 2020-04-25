#include "stm32f4xx.h"

uint16_t counter = 0;
GPIO_InitTypeDef	GPIO_InitStructure;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= 	GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

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

int main(void)
{

	SysTick_Config(SystemCoreClock/1000);
	GPIO_Config();

  while (1)
  {
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	delay_ms(1000);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	delay_ms(1000);
  }
}

