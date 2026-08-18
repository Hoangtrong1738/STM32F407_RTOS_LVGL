
#include "led.h"

void led_init()
{
    GPIOA_PCLK_EN();

    GPIO_Handle_t A;
    A.pGPIOx = GPIOA;
    A.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    A.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    A.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    A.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

    A.GPIO_PinConfig.GPIO_PinNumber = PIN_6;
    GPIO_Init(&A);
    A.GPIO_PinConfig.GPIO_PinNumber = PIN_7;
    GPIO_Init(&A);

}
void led_on(led_t state)
{
    switch(state)
    {
        case A6:
        {
            GPIO_WriteToOutputPin(GPIOA,PIN_6,SET);
            break;

        }
        case A7:
        {
            GPIO_WriteToOutputPin(GPIOA,PIN_7,SET);
            break;

        }
    }

}


void led_off(led_t state)
{
    switch(state)
    {
        case A6:
        {
            GPIO_WriteToOutputPin(GPIOA,PIN_6,RESET);
            break;

        }
        case A7:
        {
            GPIO_WriteToOutputPin(GPIOA,PIN_7,RESET);
            break;

        }
    }

}

void led_toggle(led_t state)
{
    switch(state)
    {
        case A6:
        {
            GPIO_WriteToOutputPin(GPIOA,PIN_6,SET);
            break;

        }
        case A7:
        {
            GPIO_WriteToOutputPin(GPIOA,PIN_7,SET);
            break;

        }
    }

}
