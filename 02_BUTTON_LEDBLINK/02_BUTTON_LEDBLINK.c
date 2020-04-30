#include "stm32f4xx.h"

// GPIO A PORTU 5.  P�N = DAH�L� LED
// GPIO C PORTU 13. P�N	= DAH�L� BUTON

uint8_t button_input = 0;
uint16_t counter = 0;

void GPIO_Config()
{

	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// LED CONFIG

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// BUTTON CONFIG

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

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
	GPIO_Config();
	SysTick_Config(SystemCoreClock/1000);
  while (1)
  {

	  button_input = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
	  if(button_input == 0)
	  {
	    	while((button_input = 0));
	    	delay_ms(300);
	    	GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
	  }
  }
}
