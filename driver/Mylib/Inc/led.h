#ifndef LED_H
#define LED_H

#include "stm32f407xx.h"
typedef enum
{
    A6,
    A7

}led_t;

void led_init();
void led_on(led_t state);
void led_off(led_t state);
void led_toggle(led_t state);


#endif