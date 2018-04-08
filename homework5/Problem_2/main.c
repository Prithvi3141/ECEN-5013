#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "portable.h"
#include "semphr.h"
#include "timers.h"

#define SEMAPHORE_MAX_COUNT         1
#define SEMAPHORE_INITIAL_COUNT     0

#define SYS_CLOCK_MHZ           1000000
#define SYS_CLOCK_FREQUENCY     120*SYS_CLOCK_MHZ

SemaphoreHandle_t task1_sem, task2_sem;                 //Semaphores for Taks 1 & task 2
uint32_t timer1_period, timer2_period;                  //Timer periods for Timer 1 & Timer2
BaseType_t timer1_id, timer2_id;                        //Timer ids used to differentiate the two timers used by 2 tasks in the common timer callback handler

//Timer Callback Handler for both timer 1 and timer 2. TimerId is used to differentiate between the two timers and give the semaphore for
//the appropriate task based on the which timer has set off.
void timer_callback(TimerHandle_t timer)
{
    BaseType_t *timer_id = (BaseType_t *) pvTimerGetTimerID(timer);
    BaseType_t pxHigherPriorityTaskWoken = pdTRUE;

    if(*timer_id == timer1_id)
        xSemaphoreGiveFromISR(task1_sem, &pxHigherPriorityTaskWoken);
    else if(*timer_id == timer2_id)
        xSemaphoreGiveFromISR(task2_sem, &pxHigherPriorityTaskWoken);
}
//Task 1 is used to do LED blinking at the rate of 2Hz
void vTask1( void *pvParameters )
{
    const char *timer_name = "Timer 1\r\n";
    BaseType_t led_state = pdFALSE;
    TimerHandle_t timer;

    task1_sem = xSemaphoreCreateCounting(SEMAPHORE_MAX_COUNT, SEMAPHORE_INITIAL_COUNT);

    if(task1_sem == NULL)
        vTaskDelete(NULL);

    timer1_id = 1;
    timer = xTimerCreate(timer_name, pdMS_TO_TICKS(timer1_period), pdTRUE, (void *)&timer1_id, timer_callback);
    if(timer == NULL)
        vTaskDelete(NULL);

    if(!xTimerStart(timer, portMAX_DELAY))
        vTaskDelete(NULL);


    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    while(1)
    {
        xSemaphoreTake(task1_sem, portMAX_DELAY);

        if(led_state)
        {
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
            led_state = pdFALSE;
            //Switch off the led
        }
        else
        {
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
            led_state = pdTRUE;
            //Switch on the led
        }
    }
}
//Task 2 is used to do Log the tick count to the UART at the rate of 4Hz
void vTask2( void *pvParameters )
{
    const char *timer_name = "Timer 2\r\n";
    BaseType_t led_state = pdFALSE;
    TimerHandle_t timer;

    task2_sem = xSemaphoreCreateCounting(SEMAPHORE_MAX_COUNT, SEMAPHORE_INITIAL_COUNT);

    if(task2_sem == NULL)
        vTaskDelete(NULL);

    timer2_id = 2;
    timer = xTimerCreate(timer_name, pdMS_TO_TICKS(timer2_period), pdTRUE, (void *)&timer2_id, timer_callback);
    if(timer == NULL)
        vTaskDelete(NULL);

    if(!xTimerStart(timer, portMAX_DELAY))
        vTaskDelete(NULL);


    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

    while(1)
    {
        xSemaphoreTake(task2_sem, portMAX_DELAY);

        if(led_state)
        {
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
            led_state = pdFALSE;
            //Switch off the led
        }
        else
        {
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
            led_state = pdTRUE;
            //Switch on the led
        }
    }
}




/**
 * main.c
 */
int main( void )
{

    uint32_t sys_clock_get;

    sys_clock_get = SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_CFG_VCO_480, SYS_CLOCK_FREQUENCY);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));

    timer1_period = 1000/4;
    timer2_period = 1000/8;

    /* Create Task 1*/
    xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL ); /* This example does not use the task handle. */
    /* Create Task 2*/
    xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
    /* Start the scheduler so the tasks start executing. */
    vTaskStartScheduler();
    for( ;; );
    return 0;
}
