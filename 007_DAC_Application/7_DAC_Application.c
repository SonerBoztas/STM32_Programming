// BU UYGULAMA NUCLEO F401RE KARTINDA ÇALIŞMAZ, BU KART ÜZERİNDE DAC BİRİMİNİ BULUNDURMAZ !

#include "stm32f4xx.h"

GPIO_InitTypeDef	GPIO_InitStructure;
DAC_InitTypeDef		DAC_InitStructure;

/*
 * DAC1 = PA4 : DAC Channel 1
 * DAC2 = PA5 : DAC Channel 2 // PA5 INTERNAL LED
 */

uint16_t i = 0;

void delay(uint32_t time)
{
	while(time--);
}
void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin	    = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


void DAC1_Config()
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStructure.DAC_WaveGeneration	= DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_Trigger			= DAC_Trigger_None;
	DAC_InitStructure.DAC_OutputBuffer		= DAC_OutputBuffer_Disable; //"DAC_OutputBuffer" DAC pinindeki gürültüyü engellemek için kullanılır.

	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);			// Çevresel Birimleri Aktif etmek için CMD komutu kullanılmalıdır.

}

int main(void)
{
	GPIO_Config();
	DAC1_Config();

  while (1)
  {
	  /**
	    * @brief  Set the specified data holding register value for DAC channel1.
	    * @param  DAC_Align: Specifies the data alignment for DAC channel1.
	    *          This parameter can be one of the following values:
	    *            @arg DAC_Align_8b_R: 8bit right data alignment selected
	    *            @arg DAC_Align_12b_L: 12bit left data alignment selected
	    *            @arg DAC_Align_12b_R: 12bit right data alignment selected
	    * @param  Data: Data to be loaded in the selected data holding register.
	    * @retval None
	    */
	  for(i=0; i<255; i++)
	  {
		  DAC_SetChannel1Data(DAC_Align_8b_R, i);      // "DAC_SetChannel2Data()", "DAC_SetDualChannelData()"
		  delay(100000);
	  }

	  i=0;

  }
}
