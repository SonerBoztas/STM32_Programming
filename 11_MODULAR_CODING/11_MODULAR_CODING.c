#include "stm32f4xx.h"

#include "led.h"
#include "button.h"
#include "delay.h"

/*
 * PA5: INTERNAL LED
 *
 * led konfigürasyonları
 * led açık
 * led kapalı
 * led durum değiştir / toggle
 *
 *
 * PC13: USER BUTTON
 *
 * button konfigürasyonları
 * button okuma işlemi
 *
 * ms Cinsinden Delay
 *
 * sistem periyot ayarı
 * sistem timer interruptı
 * delay fonksiyonu
 *
 */

uint8_t button_level = 0;
uint8_t button_last_level = 0;

int main(void)
{
	led_config();
	button_config();
	clock_period_config();

  while (1)
  {
	  button_level = button_read();
	  while(button_read());
	  delay_ms(200);
	  button_last_level = button_last_level + button_level;

	  if(button_last_level % 2)
	  {
		  led_on();
	  }
	  else
	  {
		  led_off();
	  }
  }
}
