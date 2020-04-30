// STM32F401RE İŞLEMCİSİNDE BU KOD ÇALIŞMAZ, İŞLEMCİNİN DAC BİRİMİ YOKTUR.

#include "stm32f4xx.h"

GPIO_InitTypeDef		GPIO_InitStructure;
ADC_InitTypeDef			ADC_InitStructure;
ADC_CommonInitTypeDef	ADC_CommonInitStructure;
DAC_InitTypeDef			DAC_InitStructure;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_4;		// DAC Channel 1
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;		// ADC Channel 0
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStructure.ADC_Mode		= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler	= ADC_Prescaler_Div4;

	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution		= ADC_Resolution_12b;

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);

}

void DAC1_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStructure.DAC_Trigger			= DAC_Trigger_None;
	DAC_InitStructure.DAC_OutputBuffer		= ENABLE;
	DAC_InitStructure.DAC_WaveGeneration	= DAC_WaveGeneration_None;

	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);

}

uint16_t ADC_Value = 0;

uint16_t Read_ADC()
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
	DAC1_Config();

  while (1)
  {

	  ADC_Value = Read_ADC();

	  DAC_SetChannel1Data(DAC_Align_12b_R, ADC_Value);
  }
}
