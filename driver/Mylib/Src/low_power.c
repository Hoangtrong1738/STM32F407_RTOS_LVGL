/*
 * low_power.c
 *
 *  Created on: May 15, 2026
 *      Author: GB Center
 */
#include "low_power.h"

void goto_low_power_mode()
{
	u32* SCR = (u32*)(0xE000ED10);
	*SCR |= 1<<2;// DEEPSLEE[ enable
	__asm("WFI");
}

void enable_iwdg()
{
	// set watchdog 3 sec wakeup MCU
}
