/*
 * clock.h
 *
 *  Created on: May 12, 2026
 *      Author: GB Center
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include "stm32f407xx.h"

typedef enum
{
	GPIOA_peripheral = 0,
	GPIOB_peripheral,
	GPIOC_peripheral,
	GPIOD_peripheral,
	GPIOE_peripheral,
	GPIOF_peripheral,
	GPIOG_peripheral,
	GPIOH_peripheral = 7,
	CRC_peripheral = 12,
	DAM1_peripheral = 21,
	DMA2_peripheral

}AHB1_peripheral_t;

typedef enum
{
	DCMI_peripheral = 0,
	CRYP_peripheral = 4,
	HASH_peripheral,
	RNG_peripheral,
	OTGFS_peripheral
}AHB2_peripheral_t;

typedef enum
{
	TIM2_peripheral = 0,
	TIM3_peripheral,
	TIM4_peripheral,
	TIM5_peripheral,
	TIM6_peripheral,
	TIM7_peripheral,
	TIM12_peripheral,
	TIM13_peripheral,
	WWDG_peripheral = 11,
	SPI2_peripheral = 14,
	SPI3_peripheral,
	USART2_peripheral = 17,
	USART3_peripheral,
	USART4_peripheral,
	USART5_peripheral,
	I2C1_peripheral,
	I2C2_peripheral,
	I2C3_peripheral,
	CAN1_peripheral = 25,
	CAN2_peripheral,
	PWR_peripheral = 28,
	DAC_peripheral

}APB1_peripheral_t;

typedef enum
{
	TIM1_peripheral = 0,
	TIM8_peripheral,
	USART1_peripheral = 4,
	USART6_peripheral,
	ADC1_peripheral = 8,
	ADC2_peripheral,
	ADC3_peripheral,
	SDIO_peripheral,
	SPI1_peripheral,
	SYSCFG_peripheral = 14,
	TIM9_peripheral = 16,
	TIM10_peripheral,
	TIM11_peripheral


}APB2_peripheral_t;

void clock_enable_AHB1(AHB1_peripheral_t peripheral);
void clock_enable_AHB2(AHB2_peripheral_t peripheral);
void clock_enable_APB1(APB1_peripheral_t peripheral);
void clock_enable_APB2(APB2_peripheral_t peripheral);
void clock_init();
#endif /* INC_CLOCK_H_ */
