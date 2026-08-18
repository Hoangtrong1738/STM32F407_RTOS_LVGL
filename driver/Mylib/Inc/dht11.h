#ifndef DHT11_H
#define DHT11_H
#include "stm32f407xx.h"
typedef struct
{
    u8 RH1;
    u8 RH2;
    u8 T1;
    u8 T2;
    u8 sum;
    float RH;
    float T;
}dht_data_t;
extern dht_data_t dht_data;
void dht_setout();
void dht_setin();
u8 dht_start();
u8 dht_read();
u8 dht_read_TempHum();


#endif