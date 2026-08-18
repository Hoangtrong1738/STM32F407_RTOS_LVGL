/*
 * clock.c
 *
 *  Created on: May 12, 2026
 *      Author: GB Center
 */

#include "clock.h"


/*
 *  setup RCC to generate a clock (168 MHZ) max
 */

#define FLASH_REG_BASEADDR    0x40023C00U
void clock_init()
{
	/*
	 * use clock  M = /8; N=x336 ; P = /2
	 * PLL  CLOCK (source from HSE)
	 */
	u32 *RCC_CR = (u32* )(RCC_BASEADDR + 0X00);
	u32 *RCC_PLLCFGR = (u32* )(RCC_BASEADDR + 0X04);
	u32 *RCC_CFGR = (u32* )(RCC_BASEADDR + 0X08);


	*RCC_CR |= 1 << 16; // enable HSE
	while(((*RCC_CR >> 17) & 1) == 0); // wait HSE ready

	u32 *FLASH_ACR = (u32* )(FLASH_REG_BASEADDR + 0X00);
	*FLASH_ACR |= (5 << 0);

    /*
     * Enable Prefetch, Instruction cache, Data cache
     */
    *FLASH_ACR |= (1 << 8);               // PRFTEN
    *FLASH_ACR |= (1 << 9);               // ICEN
    *FLASH_ACR |= (1 << 10);              // DCEN


	//set M DIV
	*RCC_PLLCFGR &= ~(0b111111 << 0);
	*RCC_PLLCFGR |= (8 << 0);

	// SET N
	*RCC_PLLCFGR &= ~(0b111111111 << 6);
	*RCC_PLLCFGR |= (336 << 6);

	// SET P
	*RCC_PLLCFGR &= ~(0b11 << 16);
	*RCC_PLLCFGR |= (0b00 << 16);

    // PLLQ = 7
	*RCC_PLLCFGR &= ~(0b1111 << 24);
    *RCC_PLLCFGR |= (7 << 24);
	//set PLL source is HSE

	*RCC_PLLCFGR |= (1 << 22);

	*RCC_CR |= 1<<24;  // enable PLL
	while(((*RCC_CR >> 25)&1) == 0); // WAIT PLL ready
	/*
	 * AHB <= 168 MHZ
	 * APB1 <= 42 MHZ
	 * APB2 <=84 MHZ
	 *
	 */

	*RCC_CFGR &= ~(0xF << 4);
	// set APB1 pre-scaler = /4
	*RCC_CFGR &= ~(0b111 << 10);
	*RCC_CFGR |= (0b101 << 10);

	// set APB2 per_scaler = /2;

	*RCC_CFGR &= ~(0b111 << 13);
	*RCC_CFGR |= (0b100 << 13);



    *RCC_CFGR &= ~(0b11 << 0);
    *RCC_CFGR |=  (0b10 << 0); // select system clock in PLL

    while(((*RCC_CFGR >> 2) & 0b11) != 0b10);

}

void clock_enable_AHB1(AHB1_peripheral_t peripheral)
{
	 u32 *RCC_AHB1ENR = (u32*)(RCC_BASEADDR + 0x30 );
	 *RCC_AHB1ENR |= (1 << peripheral);
}

void clock_enable_AHB2(AHB2_peripheral_t peripheral)
{
	 u32 *RCC_AHB2ENR = (u32*)(RCC_BASEADDR + 0x34 );
	 *RCC_AHB2ENR |= (1 << peripheral);
}

void clock_enable_APB1(APB1_peripheral_t peripheral)
{
	 u32 *RCC_APB1ENR = (u32*)(RCC_BASEADDR + 0x40 );
	 *RCC_APB1ENR |= (1 << peripheral);
}
void clock_enable_APB2(APB2_peripheral_t peripheral)
{
	 u32 *RCC_APB2ENR = (u32*)(RCC_BASEADDR + 0x44 );
	 *RCC_APB2ENR |= (1 << peripheral);
}















