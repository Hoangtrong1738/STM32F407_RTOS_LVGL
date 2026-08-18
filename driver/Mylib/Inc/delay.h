/*
 * delay.h
 *
 *  Created on: May 6, 2026
 *      Author: GB Center
 */

#ifndef MYLIB_INC_DELAY_H_
#define MYLIB_INC_DELAY_H_
#include "stm32f407xx.h"

void delay_init();
void delay_ms(u32 mssec);
void delay_us(u32 us);
u32 millis(void);
u32 micros(void);

#endif /* MYLIB_INC_DELAY_H_ */
