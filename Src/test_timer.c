#include "stdio.h"
#include "gpio.h"
#include "stm32f407xx.h"
#include "delay.h"
#include "clock.h"
#include "uart.h"
#include "adc.h"
#include "led.h"
#include "FREERTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "dht11.h"
//#include "spi.h"
char msg[40];
float temp_ss;
volatile float common_menmony;
QueueHandle_t temp_queue;
QueueHandle_t dht_1_queue;
QueueHandle_t dht_2_queue;
EventGroupHandle_t temp_event;
SemaphoreHandle_t uart_lock;
dht_handle_t dht_1;
dht_handle_t dht_2;

#define TEMP_READY_BIT (1 << 0)
#define DHT1_READY_BIT (1 << 0)
#define DHT2_READY_BIT (1 << 1)
#define TEMP_BATCH_SIZE 10
u8 i1 = 0;
u8 i2 = 0;

typedef struct 
{
    /* data */
    float t[5];
    float rh[5];
}dht_data_t;
dht_data_t dht_data1;
dht_data_t dht_data2;

void SystemInit()
{
    clock_init();
    delay_init(); 
    adc_init();
    led_init(GPIOA,PIN_6);
    led_init(GPIOA,PIN_7);
    led_init(GPIOC,PIN_6);
    led_init(GPIOC,PIN_7);
    led_init(GPIOC,PIN_8);
    led_init(GPIOC,PIN_9);
    led_init(GPIOD,PIN_15);
    led_init(GPIOD,PIN_14);
    led_init(GPIOD,PIN_13);

}
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
{
    (void)xTask;
    (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    while(1)
    {
    }
}

void func_1(void *param)
{
    while(1)
    {
        led_toggle(GPIOD,PIN_15);
        led_on(GPIOA,PIN_6);
        vTaskDelay(2500);
        led_off(GPIOA,PIN_6);
        vTaskDelay(2500);

    }
}

void func_2(void *param)
{
    while(1)
    {
        led_toggle(GPIOD,PIN_14);
        led_on(GPIOA,PIN_7);
        vTaskDelay(1000);
        led_off(GPIOA,PIN_7);
        vTaskDelay(1000);
        
    }

}

void func_3(void *param)
{
    UART1_init();
    while(1)
    {
       //wait unitl temp_event is set
        xEventGroupWaitBits(temp_event, TEMP_READY_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
        //float temp = common_menmony; // read dât from common menmory
        led_toggle(GPIOD,PIN_13);
        float temp_list[TEMP_BATCH_SIZE];
        /*
        xQueueReceive(temp_queue, &temp, pdMS_TO_TICKS(10000));      
        printlog("\033[0;31m[task 3]\033[0m: temp: %.2f\r\n",temp);
        */
        for(int i = 0; i < TEMP_BATCH_SIZE; i++)
        {
            xQueueReceive(temp_queue, &temp_list[i], portMAX_DELAY);
        }

        xSemaphoreTake(uart_lock, portMAX_DELAY);
        printlog("\033[0;31m[task 3]\033[0m: temp: [");
        for(int i = 0; i < TEMP_BATCH_SIZE; i++)
        {
            printlog("%.2f, ", temp_list[i]);
        }
        printlog("\b\b]\r\n");
        xSemaphoreGive(uart_lock);
        
    }
}

void func_4(void *param)
{
    adc_init();
    int measure_cnt = 0;
    while(1)
    {
        float temp = adc_get_temp_ss();
        //common_menmony = temp; //write temp_ss data common menmory
        xQueueSend(temp_queue, &temp, pdMS_TO_TICKS(10000));
        led_toggle(GPIOC,PIN_8);
        if(++measure_cnt >= TEMP_BATCH_SIZE)
        {
            measure_cnt = 0;
            xEventGroupSetBits(temp_event, TEMP_READY_BIT);

        }
         vTaskDelay(pdMS_TO_TICKS(300));
    }
}
void func_5(void *param)
{

    while(1)
    {
        xSemaphoreTake(uart_lock, portMAX_DELAY);
        led_toggle(GPIOC,PIN_7);
        printlog("\033[0;32m[task 5]\033[0m: hello world \r\n");
        xSemaphoreGive(uart_lock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void func_6(void *param)
{
    dht_init(&dht_1, GPIOD,PIN_5);
    dht_init(&dht_2, GPIOD,PIN_4);
    while(1)
    {
        // sensor 1
        led_toggle(GPIOC,PIN_6);
        if(dht_read_TempHum(&dht_1) == 1)
        {
            dht_data1.t[i1] = dht_1.T;
            dht_data1.rh[i1] = dht_1.RH;
            i1++;
            if(i1>=5)
            {
                i1 = 0;
                xQueueSend(dht_1_queue, &dht_data1,pdMS_TO_TICKS(100));
            }

        }

        // sensor 2
        if(dht_read_TempHum(&dht_2) == 1)
        {
            dht_data2.t[i2] = dht_2.T;
            dht_data2.rh[i2] = dht_2.RH;
            i2++;
            if(i2 >= 5)
            {
                i2 = 0;
                xQueueSend(dht_2_queue, &dht_data2,pdMS_TO_TICKS(100));
            }

        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void func_7(void *param)
{
    
    dht_data_t dht_data;
    while (1)
    {
        /* code */
        xQueueReceive(dht_1_queue,&dht_data,portMAX_DELAY);
        xSemaphoreTake(uart_lock, portMAX_DELAY);
        led_toggle(GPIOC,PIN_9);
        printlog("\033[0;33m[task 7]\033[0m T: [");
        for(int i = 0;i < 5; i++)
        {
            printlog("%.2f; ",dht_data.t[i]);
        }
        printlog("\b\b] RH: [");
        for(int i = 0;i < 5; i++)
        {
            printlog("%.2f; ",dht_data.rh[i]);
        }
        printlog("\b\b]\r\n");
        xSemaphoreGive(uart_lock);
        
        vTaskDelay(pdMS_TO_TICKS(5000));

    }
    
}
void func_8(void *param)
{
    dht_data_t dht_data;
    while (1)
    {
        /* code */
        xQueueReceive(dht_2_queue,&dht_data,portMAX_DELAY);
        xSemaphoreTake(uart_lock, portMAX_DELAY);
        led_toggle(GPIOC,PIN_10);
        printlog("\033[0;33m[task 8]\033[0m T: [");
        for(int i = 0;i < 5; i++)
        {
            printlog("%.2f; ",dht_data.t[i]);
        }
        printlog("\b\b] RH: [");
        for(int i = 0;i < 5; i++)
        {
            printlog("%.2f; ",dht_data.rh[i]);
        }
        printlog("\b\b]\r\n");
        xSemaphoreGive(uart_lock);    
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    
}
int main()
{   
    TaskHandle_t task_1 = NULL;
    TaskHandle_t task_2 = NULL;
    TaskHandle_t task_3 = NULL;
    TaskHandle_t task_4 = NULL;
    TaskHandle_t task_5 = NULL;
    TaskHandle_t task_6 = NULL;
    TaskHandle_t task_7 = NULL;
    TaskHandle_t task_8 = NULL;
    temp_queue = xQueueCreate(20, sizeof(float));
    dht_1_queue = xQueueCreate(10, sizeof(dht_data_t));
    dht_2_queue = xQueueCreate(10, sizeof(dht_data_t));
    if(temp_queue == NULL)
    {
        while(1)
        {
        }
    }
    if(dht_1_queue == NULL)
    {
        while(1);      
    }
    if(dht_2_queue == NULL)
    {
        while(1);
    }

    temp_event = xEventGroupCreate();
    if(temp_event == NULL)
    {
        while(1);
    }
    uart_lock = xSemaphoreCreateMutex();
    if(uart_lock == NULL)
    {
        while(1);
    }
    if(xTaskCreate(func_1, "task 1", 512, NULL, 0, &task_1) != pdPASS)
    {
        while(1);
    }
    if(xTaskCreate(func_2, "task 2", 512, NULL, 0, &task_2) != pdPASS)
    {
        while(1);
    }
    if(xTaskCreate(func_3, "task_3", 512, NULL, 0, &task_3) != pdPASS)
    {
        while(1);
    }

    if(xTaskCreate(func_4, "task_4", 512, NULL, 0, &task_4) != pdPASS)
    {
        while(1);
    }
    if(xTaskCreate(func_5, "task_5", 512, NULL, 0, &task_5) != pdPASS)
    {
        while(1);
    }
    if(xTaskCreate(func_8, "task_8", 512, NULL, 0, &task_8) != pdPASS)
    {
        while(1);
    }
    if(xTaskCreate(func_6, "task_6", 512, NULL, 0, &task_6) != pdPASS)
    {
        while(1);
    }
    if(xTaskCreate(func_7, "task_7", 512, NULL, 0, &task_7) != pdPASS)
    {
        while(1);
    }
    vTaskStartScheduler(); 
    while(1)
    {          
    }
    return 0;
}

void HardFault_Handler()
{
    while(1)
    {
    }
}
