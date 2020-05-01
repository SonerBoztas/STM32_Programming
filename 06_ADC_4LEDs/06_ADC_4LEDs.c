#include "stm32f4xx.h"

uint16_t led_1 =	GPIO_Pin_5;
uint16_t led_2 =	GPIO_Pin_6;
uint16_t led_3 =	GPIO_Pin_8;
uint16_t led_4 =	GPIO_Pin_9;

GPIO_InitTypeDef	GPIO_InitStructure;
ADC_InitTypeDef		ADC_InitStructure;
ADC_CommonInitTypeDef	ADC_CommonInitStructure;

uint16_t adc_value = 0;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin	= 	GPIO_Pin_0;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	= 	GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_OType	= 	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStructure.ADC_Mode 	=	ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler	=	ADC_Prescaler_Div4;

	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution	= 	ADC_Resolution_10b;

	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);
}

uint16_t read_adc()
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
	adc_value = read_adc();

	if(adc_value > 0 && adc_value < 251)
	{
		GPIO_SetBits(GPIOC, led_1);
		GPIO_ResetBits(GPIOC, led_2);
		GPIO_ResetBits(GPIOC, led_3);
		GPIO_ResetBits(GPIOC, led_4);
	}
	else if(adc_value > 250 && adc_value < 501)
	{
		GPIO_SetBits(GPIOC, led_1);
		GPIO_SetBits(GPIOC, led_2);
		GPIO_ResetBits(GPIOC, led_3);
		GPIO_ResetBits(GPIOC, led_4);
	}
	else if(adc_value > 500 && adc_value < 751)
	{
		GPIO_SetBits(GPIOC, led_1);
		GPIO_SetBits(GPIOC, led_2);
		GPIO_SetBits(GPIOC, led_3);
		GPIO_ResetBits(GPIOC, led_4);
	}
	else if(adc_value > 750 && adc_value < 1024)
	{
		GPIO_SetBits(GPIOC, led_1);
		GPIO_SetBits(GPIOC, led_2);
		GPIO_SetBits(GPIOC, led_3);
		GPIO_SetBits(GPIOC, led_4);
	}
  }
}
