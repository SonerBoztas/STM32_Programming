/*
 * delay.h
 *
 *  Created on: 30 Nis 2020
 *      Author: soner.boztas
 */

#ifndef DELAY_H_
#define DELAY_H_

extern void SysTick_Handler(void);
extern void delay_ms(uint16_t time);
extern void clock_period_config();

#endif /* DELAY_H_ */
