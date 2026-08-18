/*
 * delay.c
 *
 *  Created on: May 6, 2026
 *      Author: GB Center
 */


#include "delay.h"
#define TIMER2 1
#define SYS_TICK 2
//#define DELAY_SRC TIMER2
#define DELAY_SRC TIMER2

#define TIM2_CLK_MHz 84

#define SYSCLK_MHz 168
#if DELAY_SRC != TIMER2 && DELAY_SRC != SYS_TICK
#error DELAY_SRC must be TIMER2 or SYS_TICK
#endif
u32 tim1_cnt = 0;

#if(DELAY_SRC == SYS_TICK)

static _vo u32 systick_ms = 0;
static u32 fac_us = 0;
#endif
void delay_init()
{
#if (DELAY_SRC == TIMER2)
	//set 1 SEC for timer
	//rcc-->16Mhz->>psc(16)->>1000000Hz/1cnt ->1us
	//ARR : 1000

	TIM2_PCLK_EN();

	u32* CR1 = (u32*)(TIM2_BASEADDR + 0x00);
	u32* ARR = (u32*)(TIM2_BASEADDR + 0x2C);
	u32* PSC = (u32*)(TIM2_BASEADDR + 0x28);
	//u32* DIER = (u32*)(TIM2_BASEADDR + 0X0C);
	u32* CNT = (u32*)(TIM2_BASEADDR + 0X24);
	u32* EGR = (u32*)(TIM2_BASEADDR + 0x14);
	*CR1 &= ~(1 << 0);

	*ARR = 0xffffffff;
	*PSC = TIM2_CLK_MHz - 1;
	*CNT = 0;

	*EGR = 1; // Update generation

	//*DIER |= 1 << 0;  // enable interrupt
	*CR1 |= 1 << 0;  //enable counter

	//u32 *ISER0 = (u32*)(0xe000e100);

	//*ISER0 |= 1 << 25;

#else
	u32 *CSR = (u32*)0xE000E010;
	u32 *RVR = (u32*)0xE000E014;
	u32 *CVR = (u32*)0xE000E018;
	//u32 *CVR = (u32*)0xE000E018;

	fac_us = SYSCLK_MHz;

	*CSR = 0; //
	*RVR = (SYSCLK_MHz * 1000U) - 1;


	*CVR = 0; // clean current counter
	*CSR = (1<<0) | (1<<1) | (1<<2);
#endif

}


/*
void TIM1_UP_TIM10_IRQHandler()
{

	u16 *SR =(u16*)(0x40010010);
	*SR &= ~(1 << 0);
	tim1_cnt++;
}
*/
/*
void tim1_delay_1sec()
{
	u16 *SR = (u16*)(0x40010010);
	while(((*SR << 0) & 1) != 1); //Set UIF flag is set to 1

	*SR &= ~(1<<0);   // clean UIF flag


}

*/
#if(DELAY_SRC == SYS_TICK)

void SysTick_Handler(void)
{
	systick_ms++;

}

#endif

void delay_us(u32 us)
{
#if (DELAY_SRC == TIMER2)
	u32 *CNT = (u32*)(TIM2_BASEADDR + 0x24);

	u32 start = *CNT;
	while((u32)(*CNT -start) < us);
#else
	uint32_t temp;
	uint32_t ticks;
	uint32_t *CSR = (uint32_t *)0xE000E010;
	uint32_t *RVR = (uint32_t *)0xE000E014;
	uint32_t *CVR = (uint32_t *)0xE000E018;

	if(us == 0)
	{
		return;
	}

	ticks = us * fac_us;
	*CSR = 0; // Tạm dừng ngắt để chuyển sang chế độ Polling

	
	while(ticks > 0xffffff)
	{
		*RVR = 0xffffff;
		*CVR = 0;
		*CSR = (1 << 0) | (1 << 2);

		do
		{
			temp = *CSR;
		}
		while((temp & 0x01) && !(temp & (1 << 16)));

		ticks -= 0xffffff; // ĐÃ SỬA: Trừ bớt ticks để không bị kẹt vòng lặp vô hạn
		*CSR = 0;
		*CVR = 0;
	}

	if(ticks > 0)
	{
		*RVR = ticks - 1U;
		*CVR = 0;
		*CSR = (1 << 0) | (1 << 2);

		do
		{
			temp = *CSR;
		}
		while((temp & 0x01) && !(temp & (1 << 16)));

		*CSR = 0;
		*CVR = 0;
	}
	/*
	 * Khôi phục lại SysTick interrupt mỗi 1ms.
	 */
	*RVR = (SYSCLK_MHz * 1000U) - 1U;
	*CVR = 0;
	*CSR = (1 << 0) | (1 << 1) | (1 << 2);
#endif
}
void delay_ms(u32 mssec)
{
#if (DELAY_SRC == TIMER2)

    while (mssec >= 1000U)
    {
        delay_us(1000000U);
        mssec -= 1000U;
    }

    if (mssec > 0U)
    {
        delay_us(mssec * 1000U);
    }

#else

	u32 current_cnt = systick_ms;

	while((u32)(systick_ms - current_cnt) < mssec);

#endif
}

u32 millis(void)
{
#if (DELAY_SRC == TIMER2)

	u32 *CNT = (u32 *)(TIM2_BASEADDR + 0x24);
	return (*CNT) / 1000U;

#else

	return systick_ms;

#endif
}

u32 micros(void)
{
#if (DELAY_SRC == TIMER2)

	u32 *CNT = (u32 *)(TIM2_BASEADDR + 0x24);
	return *CNT;

#else

	u32 *RVR = (u32 *)0xE000E014;
    u32 *CVR = (u32 *)0xE000E018;
    
    // Đọc giá trị đếm ngược của SysTick để tính chính xác tới từng Micro giây
    u32 ticks = *RVR - *CVR; 
    return (systick_ms * 1000U) + (ticks / SYSCLK_MHz);

#endif
}


