#ifndef DHT11_H
#define DHT11_H
#include "stm32f407xx.h"
typedef struct
{
    GPIO_RegDef_t *port;
    u8 pin;

    u8 RH1;
    u8 RH2;
    u8 T1;
    u8 T2;
    u8 sum;
    float RH;
    float T;
}dht_handle_t;

void dht_init(dht_handle_t *dht, GPIO_RegDef_t *port, u8 pin);
u8 dht_start(dht_handle_t *dht);
u8 dht_read_TempHum(dht_handle_t *dht);


#endif
