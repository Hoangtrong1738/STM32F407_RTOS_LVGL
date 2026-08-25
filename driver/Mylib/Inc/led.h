#ifndef LED_H
#define LED_H

#include "stm32f407xx.h"
void led_init(GPIO_RegDef_t *port,u8 pin);
void led_on(GPIO_RegDef_t *port,u8 pin);
void led_off(GPIO_RegDef_t *port,u8 pin);
void led_toggle(GPIO_RegDef_t *port,u8 pin);


#endif