#include "stdio.h"
#include "gpio.h"
#include "stm32f407xx.h"
#include "delay.h"
#include "clock.h"
#include "uart.h"
#include "adc.h"
#include "led.h"
#include "dht11.h"

#define TEMP_BATCH_SIZE    10
#define DHT_BATCH_SIZE      5

typedef struct
{
    float t[DHT_BATCH_SIZE];
    float rh[DHT_BATCH_SIZE];

} dht_data_t;

dht_handle_t dht_1;
dht_handle_t dht_2;
float temp_buffer[TEMP_BATCH_SIZE];
u8 temp_index = 0;

dht_data_t dht_data1;
dht_data_t dht_data2;

u8 i1 = 0;
u8 i2 = 0;
void system_init(void);

void task_led_1(void);
void task_led_2(void);

void task_adc(void);
void task_process_adc(void);

void task_uart(void);

void task_dht(void);

void dht_read_task(
    dht_handle_t *dht,
    dht_data_t *data,
    u8 *index
);

void task_process_dht(void);

void dht_process_task(
    dht_data_t *data,
    u8 *index,
    GPIO_TypeDef *port,
    u16 pin,
    const char *name
);
void system_init(void)
{
    clock_init();

    delay_init();

    adc_init();

    UART1_init();
    led_init(GPIOA, PIN_6);
    led_init(GPIOA, PIN_7);

    led_init(GPIOC, PIN_6);
    led_init(GPIOC, PIN_7);
    led_init(GPIOC, PIN_8);
    led_init(GPIOC, PIN_9);
    led_init(GPIOC, PIN_10);

    led_init(GPIOD, PIN_13);
    led_init(GPIOD, PIN_14);
    led_init(GPIOD, PIN_15);

    dht_init(&dht_1, GPIOD, PIN_5);

    dht_init(&dht_2, GPIOD, PIN_4);
}


void task_led_1(void)
{
    static u32 last_time = 0;
    static u8 state = 0;

    u32 now = millis();


    if((u32)(now - last_time) >= 2500)
    {
        last_time = now;

        state = !state;


        if(state)
        {
            led_on(GPIOA, PIN_6);
        }
        else
        {
            led_off(GPIOA, PIN_6);
        }

        led_toggle(GPIOD, PIN_15);
    }
}


void task_led_2(void)
{
    static u32 last_time = 0;
    static u8 state = 0;

    u32 now = millis();


    if((u32)(now - last_time) >= 1000)
    {
        last_time = now;

        state = !state;


        if(state)
        {
            led_on(GPIOA, PIN_7);
        }
        else
        {
            led_off(GPIOA, PIN_7);
        }

        led_toggle(GPIOD, PIN_14);
    }
}

void task_adc(void)
{
    static u32 last_time = 0;

    u32 now = millis();


    if((u32)(now - last_time) >= 300)
    {
        last_time = now;

        temp_buffer[temp_index] =
            adc_get_temp_ss();
        temp_index++;
        led_toggle(GPIOC, PIN_8);
    }
}

void task_process_adc(void)
{
    if(temp_index < TEMP_BATCH_SIZE)
    {
        return;
    }

    temp_index = 0;

    led_toggle(GPIOD, PIN_13);

    printlog(
        "\033[0;31m"
        "[SUPER LOOP - ADC]"
        "\033[0m"
        " Temp: ["
    );

    for(int i = 0; i < TEMP_BATCH_SIZE; i++)
    {
        printlog(
            "%.2f; ",
            temp_buffer[i]
        );
    }


    printlog("\b\b]\r\n");
}

void task_uart(void)
{
    static u32 last_time = 0;

    u32 now = millis();


    if((u32)(now - last_time) >= 1000)
    {
        last_time = now;

        led_toggle(GPIOC, PIN_7);


        printlog(
            "\033[0;32m"
            "[SUPER LOOP]"
            "\033[0m"
            " Hello World\r\n"
        );
    }
}

void dht_read_task(
    dht_handle_t *dht,
    dht_data_t *data,
    u8 *index
)
{
    if(dht_read_TempHum(dht) == 1)
    {
        data->t[*index] =
            dht->T;

        data->rh[*index] =
            dht->RH;


        (*index)++;
    }
}

void task_dht(void)
{
    static u32 last_time = 0;

    u32 now = millis();


    if((u32)(now - last_time) >= 1000)
    {
        last_time = now;



        led_toggle(GPIOC, PIN_6);

        dht_read_task(
            &dht_1,
            &dht_data1,
            &i1
        );
        dht_read_task(
            &dht_2,
            &dht_data2,
            &i2
        );
    }
}

void dht_process_task( dht_data_t *data, u8 *index, GPIO_TypeDef *port, u16 pin,const char *name)
{
    if(*index < DHT_BATCH_SIZE)
    {
        return;
    }
    *index = 0;
    led_toggle(port, pin);
    printlog(
        "\033[0;33m"
        "[%s]"
        "\033[0m"
        " T: [",
        name
    );
    for(int i = 0; i < DHT_BATCH_SIZE; i++)
    {
        printlog(
            "%.2f; ",
            data->t[i]
        );
    }
    printlog("\b\b] RH: [");
    for(int i = 0; i < DHT_BATCH_SIZE; i++)
    {
        printlog(
            "%.2f; ",
            data->rh[i]
        );
    }
    printlog("\b\b]\r\n");
}

void task_process_dht(void)
{
    dht_process_task( &dht_data1, &i1, GPIOC, PIN_9,"DHT1"
    );
    dht_process_task( &dht_data2, &i2,GPIOC,PIN_10,"DHT2"
    );
}

int main(void)
{
    system_init();
    while(1)
    {
        task_led_1();

        task_led_2();
        task_adc();

        task_process_adc();

        task_uart();

        task_dht();
        task_process_dht();
    }
}


