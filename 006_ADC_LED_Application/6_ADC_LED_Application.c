#include "stm32f4xx.h"

GPIO_InitTypeDef		GPIO_InitStructure;
ADC_InitTypeDef			ADC_InitStructure;
ADC_CommonInitTypeDef	ADC_CommonInitStructure;

uint8_t ADC_Value = 0;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin	    = 	GPIO_Pin_0;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	    = 	GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStructure.ADC_Mode 		=	ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler	=	ADC_Prescaler_Div4;

	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution		= 	ADC_Resolution_8b;

	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);
}

uint8_t Read_ADC()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	return ADC_GetConversionValue(ADC1);
}


int main(void)
{
	GPIO_Config();
	ADC_Config();

  while (1)
  {
	ADC_Value = Read_ADC();

	if(ADC_Value < 51)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
	else if(ADC_Value > 50 && ADC_Value < 101)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
	else if(ADC_Value > 100 && ADC_Value < 151)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
	else if(ADC_Value > 150 && ADC_Value < 201)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	}
	else if(ADC_Value > 200 && ADC_Value < 251)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_5);
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
	}
  }
}
