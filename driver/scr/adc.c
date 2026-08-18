
#include "adc.h"
#include "clock.h"
#include "stm32f407xx.h"
#include "gpio.h"

#define ADC1_ADDRESS_BASE 0x40012000U
#define ADC1_IN16  16

void adc_init(void)
{ 
    clock_enable_APB2(ADC1_peripheral);

    
    _vo u32 *ADC_CR1 = (_vo u32*)(ADC1_ADDRESS_BASE + 0X04);
    _vo u32 *ADC_CR2 = (_vo u32*)(ADC1_ADDRESS_BASE + 0X08);
    _vo u32 *ADC_JSQR = (_vo u32*)(ADC1_ADDRESS_BASE + 0X38);
    _vo u32 *ADC_CCR = (_vo u32*)(ADC1_ADDRESS_BASE + 0X04 + 0x300);
    _vo u32 *ADC_SMPR1 = (_vo u32*)(ADC1_ADDRESS_BASE + 0X0C);

    *ADC_CR1 &= ~(0b11 << 24); // 12-bit resolution
    *ADC_SMPR1 &= ~(0b111 << 18);
    *ADC_SMPR1 |= (0b111 << 18); // channel 16 sample time = 480 cycles
    *ADC_JSQR &= ~(0b11 << 20); // Injected sequence length = 1 conversion
    *ADC_JSQR &= ~(0b11111 << 15);
    *ADC_JSQR |= (ADC1_IN16 << 15); // set source for JSQ4 is temp sensor(IN16)
    *ADC_CCR |= 1 << 23; // enable temperature sensor
    *ADC_CR2 |= 1 << 0; // enable ADC1
     
 
    }

float adc_get_temp_ss(void)
{
    float temp = 0;
    float vin = 0;
    u16 raw_data = 0;

    //trigger ADC start convert
    volatile u32 *ADC_CR2 = (volatile u32*)(ADC1_ADDRESS_BASE + 0X08);
    volatile u32 *ADC_SR = (volatile u32*)(ADC1_ADDRESS_BASE + 0X00);
    volatile u32 *ADC_JDR1 = (volatile u32*)(ADC1_ADDRESS_BASE + 0x3C);
    *ADC_CR2 |= 1 << 22;
      
     // wait until end of conversion
    while(((*ADC_SR >> 2) & 1) == 0);
    *ADC_SR &= ~(1 << 2); //clear JEOCM fiag
     // read ADC data from JDR1 (data of JSQ4)
     raw_data = *ADC_JDR1;
     vin = (raw_data * 3000) / 4095.0;
     temp = ((vin - 760) / 2.5 + 25);
     return temp;
   
}
