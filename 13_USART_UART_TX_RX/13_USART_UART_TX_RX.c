#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>

GPIO_InitTypeDef	GPIO_InitStructure;
USART_InitTypeDef	USART_InitStructure;
NVIC_InitTypeDef	NVIC_InitStructure;

char receive_data[50];
uint32_t i = 0;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2 | GPIO_Pin_3; //PA2: TX  && PA3: RX
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;		 
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

}

void USART_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitStructure.USART_BaudRate				= 115200;
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None; 	
	USART_InitStructure.USART_Mode					= USART_Mode_Tx | USART_Mode_Rx;	
	USART_InitStructure.USART_Parity				= USART_Parity_No;					
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;

	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;

	NVIC_Init(&NVIC_InitStructure);

}

void USART2_IRQHandler(void)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	char RX_Data = USART_ReceiveData(USART2);
	USART_SendData(USART2, RX_Data);
	receive_data[i] = RX_Data;
	i++;

		if(RX_Data == 'A')
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_5);
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			GPIO_ResetBits(GPIOC, GPIO_Pin_8);
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		}
		if(RX_Data == 'B')
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
			GPIO_SetBits(GPIOC, GPIO_Pin_6);
			GPIO_ResetBits(GPIOC, GPIO_Pin_8);
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		}
		if(RX_Data == 'C')
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			GPIO_SetBits(GPIOC, GPIO_Pin_8);
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		}
		if(RX_Data == 'D')
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_5);
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			GPIO_ResetBits(GPIOC, GPIO_Pin_8);
			GPIO_SetBits(GPIOC, GPIO_Pin_9);
		}
}

int main(void)
{
	GPIO_Config();
	USART_Config();

  while (1)
  {
	  USART2_IRQHandler();
  }
}
