#include "dht11.h"
#include "delay.h"
#include "FREERTOS.h"
#include "task.h"
#include "stddef.h"
#include <stdint.h>
dht_data_t dht_data;
#define dht_read (((GPIOD->IDR) & (1<<5)) ?1:0)

void dht_setout()
{
    //GPIOD5
    GPIOD->MODER &= ~(0b11 << 10);
    GPIOD->MODER |= 0 << 10;
    GPIOD->OTYPER &= ~(0b1 << 5);
    GPIOD->OTYPER |= 0 << 5;
}
void dht_setin()
{
        //GPIOD5
    GPIOD->MODER &= ~(0b11 << 10);
    GPIOD->MODER |= 1 << 10;
    GPIOD->OSPEEDR &= ~(0b11<< 10);
    GPIOD->OSPEEDR |= 2 << 10;

}
u8 dht_start()
{
    dht_setout();
    GPIOD->ODR = 0 << 5;
    vTaskDelay(20);
    dht_setin();
    delay_us(40);
    if(!dht_read)
    {
        delay_us(80);
        if(dht_read)
        {
            while(dht_read);

        }
        return 1;
    }
    return 0;
}
u8 dht_read_byte()
{
    u8 value = 0;
    for(u8 i = 0; i < 8; i++)
    {
        while(!dht_read);
        delay_us(40);
        if(!dht_read)
        {
            value &= ~(1 << (7 - i));
        }else
        {
            value |= (1 << (7-i));
        }
        while(dht_read);
    }
    return value;
}
u8 dht_read_TempHum()
{
    if(dht_start)
    {
        dht_data.RH1 = dht_read_byte();
        dht_data.RH2 = dht_read_byte();
        dht_data.T1 = dht_read_byte();
        dht_data.T2 = dht_read_byte();
        dht_data.sum = dht_read_byte();
        if((u8)(dht_data.RH1 + dht_data.RH2 + dht_data.T1 + dht_data.T2) == dht_data.sum)
        {
            dht_data.RH = (float)(dht_data.RH1 + dht_data.RH2) / 10.0f;
            dht_data.T = (float)(dht_data.T1 + dht_data.T2)/10.0f;
            return 1;

        }

    }
    return 0;
}