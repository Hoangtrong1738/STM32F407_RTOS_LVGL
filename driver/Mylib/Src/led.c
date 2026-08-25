
#include "led.h"

void led_init(GPIO_RegDef_t *port,u8 pin)
{
    GPIOA_PCLK_EN();

    GPIO_Handle_t gpio;
    gpio.pGPIOx = port;
    gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
    gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    gpio.GPIO_PinConfig.GPIO_PinNumber = pin;
    GPIO_Init(&gpio);

}
void led_on(GPIO_RegDef_t *port,u8 pin)
{
    GPIO_WriteToOutputPin(port,pin,SET);
}


void led_off(GPIO_RegDef_t *port,u8 pin)
{
    GPIO_WriteToOutputPin(port,pin,RESET);
}

void led_toggle(GPIO_RegDef_t *port,u8 pin)
{
    GPIO_ToggleOutputPin(port,pin);
}
