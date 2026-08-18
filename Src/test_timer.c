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
//#include "spi.h"
char msg[40];

void SystemInit()
{
    led_init();

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
        led_on(A6);
        vTaskDelay(1000);
        led_off(A6);
        vTaskDelay(1000);

    }
}

void func_2(void *param)
{
    while(1)
    {
        led_on(A7);
        vTaskDelay(1000);
        led_off(A7);
        vTaskDelay(1000);
        
    }

}


int main()
{   
     clock_init();
    //delay_init();
    UART1_init();
    adc_init();
    led_init();
    TaskHandle_t task_1;
    TaskHandle_t task_2;
    if(xTaskCreate(func_1, "task 1", 512, NULL, 1, &task_1) != pdPASS)
    {
        while(1)
        {
        }
    }

    if(xTaskCreate(func_2, "task 2", 512, NULL, 1, &task_2) != pdPASS)
    {
        while(1)
        {
        }
    }
    led_on(A6);
    

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
