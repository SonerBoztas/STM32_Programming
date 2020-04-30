#include "stm32f4xx.h"

#define buffer_length 1

uint16_t adc_value = 0;

GPIO_InitTypeDef		GPIO_InitStructure;
ADC_InitTypeDef			ADC_InitStructure;
ADC_CommonInitTypeDef	ADC_CommonInitStructure;
DMA_InitTypeDef			DMA_InitStructure;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DMA_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_InitStructure.DMA_Channel				= DMA_Channel_0;

	DMA_InitStructure.DMA_PeripheralBaseAddr	= (uint32_t) &ADC1 -> DR;
	DMA_InitStructure.DMA_Memory0BaseAddr		= (uint32_t) &adc_value;

	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize			= buffer_length;

	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;  // PINC
	DMA_InitStructure.DMA_MemoryInc				= DMA_MemoryInc_Enable;		  // MINC

	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;  // Byte: 8Bit, HalfWord: 16 Bit, Word: 32Bit
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;

	DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;

	DMA_InitStructure.DMA_Priority				= DMA_Priority_High;

	DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream0, ENABLE);
}

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStructure.ADC_Mode			= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler		= ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;

	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution			= ADC_Resolution_12b;
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ContinuousConvMode	= ENABLE;

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);

	ADC_DMACmd(ADC1, ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
}

int main(void)
{
	GPIO_Config();
	ADC_Config();
	DMA_Config();

	ADC_SoftwareStartConv(ADC1);

  while (1)
  {
	  if(adc_value > 2000)
	  {
		  GPIO_SetBits(GPIOA, GPIO_Pin_5);
	  }
	  else
	  {
		  GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	  }
  }
}
