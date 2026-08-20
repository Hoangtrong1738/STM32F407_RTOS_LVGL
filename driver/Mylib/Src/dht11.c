#include "dht11.h"
#include "delay.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stddef.h"
#include <stdint.h>
#include "uart.h"
#define DHT_TIMEOUT_US 520U

static u8 dht_read_pin(dht_handle_t *dht)
{
    return ((dht->port->IDR & (1U << dht->pin)) != 0U) ? 1U : 0U;
}

static u8 dht_wait_level(dht_handle_t *dht, u8 level, u32 timeout_us)
{
    while(timeout_us--)
    {
        if(dht_read_pin(dht) == level)
        {
            return 1;
        }

        delay_us(1);
    }

    return 0;
}

static void dht_setout(dht_handle_t *dht)
{
    u8 p = dht->pin * 2;

    GPIO_PClkControl(dht->port, ENABLE);

    dht->port->MODER &= ~(0b11 << p);
    dht->port->MODER |=  (0b01 << p);
    dht->port->OTYPER |= (1U << dht->pin);
    dht->port->OSPEEDR &= ~(0b11 << p);
    dht->port->OSPEEDR |=  (0b10 << p);
    dht->port->PUPDR &= ~(0b11 << p);
    dht->port->PUPDR |=  (0b01 << p);
}

static void dht_setin(dht_handle_t *dht)
{
    u8 p = dht->pin * 2;

    GPIO_PClkControl(dht->port, ENABLE);

    dht->port->MODER &= ~(0b11 << p);
    dht->port->OSPEEDR &= ~(0b11 << p);
    dht->port->OSPEEDR |=  (0b10 << p);
    dht->port->PUPDR &= ~(0b11 << p);
    dht->port->PUPDR |=  (0b01 << p);
}

void dht_init(dht_handle_t *dht, GPIO_RegDef_t *port, u8 pin)
{
    dht->port = port;
    dht->pin = pin;
    dht->RH1 = 0;
    dht->RH2 = 0;
    dht->T1 = 0;
    dht->T2 = 0;
    dht->sum = 0;
    dht->RH = 0.0f;
    dht->T = 0.0f;

    dht_setin(dht);
}

u8 dht_start(dht_handle_t *dht)
{
    dht_setout(dht);
    dht->port->ODR &= ~(1U << dht->pin);
    vTaskDelay(pdMS_TO_TICKS(20));

    dht_setin(dht);
    delay_us(40);

    if(!dht_wait_level(dht, 0, DHT_TIMEOUT_US))
    {
        printlog("1");
        
        return 0;
    }

    if(!dht_wait_level(dht, 1, DHT_TIMEOUT_US))
    {
        printlog("2");
        return 0;
    }

    if(!dht_wait_level(dht, 0, DHT_TIMEOUT_US))
    {
        printlog("3");
        return 0;
    }

    return 1;
}

static u8 dht_read_byte(dht_handle_t *dht, u8 *value)
{
    u8 data = 0;

    for(u8 i = 0; i < 8; i++)
    {
        if(!dht_wait_level(dht, 1, DHT_TIMEOUT_US))
        {
            printlog("4");
            return 0;
        }

        delay_us(40);

        if(dht_read_pin(dht))
        {
            data |= (1U << (7U - i));
        }

        if(!dht_wait_level(dht, 0, DHT_TIMEOUT_US))
        {
            printlog("5");
            return 0;
        }
    }

    *value = data;
    return 1;
}

u8 dht_read_TempHum(dht_handle_t *dht)
{
    if(!dht_start(dht))
    {
        return 0;
    }

    if(!dht_read_byte(dht, &dht->RH1)) return 0;
    if(!dht_read_byte(dht, &dht->RH2)) return 0;
    if(!dht_read_byte(dht, &dht->T1)) return 0;
    if(!dht_read_byte(dht, &dht->T2)) return 0;
    if(!dht_read_byte(dht, &dht->sum)) return 0;

    if((u8)(dht->RH1 + dht->RH2 + dht->T1 + dht->T2) != dht->sum)
    {
        return 0;
    }

    dht->RH = (float)dht->RH1 + ((float)dht->RH2 / 10.0f);
    dht->T = (float)dht->T1 + ((float)dht->T2 / 10.0f);

    return 1;
}
