#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>

GPIO_InitTypeDef	GPIO_InitStructure;
USART_InitTypeDef	USART_InitStructure;

char str[50];           

uint16_t counter = 0;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;		 
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
}

void USART_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	

	USART_InitStructure.USART_BaudRate				= 115200;
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None; 	
	USART_InitStructure.USART_Mode					= USART_Mode_Tx;				  	
	USART_InitStructure.USART_Parity				= USART_Parity_No;					
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;

	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);

}

void USART_Puts(USART_TypeDef* USARTx, volatile char *s)  
{
	while(*s)	
	{
	  
		while(!(USARTx -> SR & 0x00000040));
		USART_SendData(USARTx, *s);              
		*s++;									 
	}
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
	USART_Config();
	SysTick_Config(SystemCoreClock/1000);

  while (1)
  {
	  sprintf(str, "Hello World \nSTM32F401RE\n");
	  USART_Puts(USART2, str);
	  delay_ms(1000);

  }
}