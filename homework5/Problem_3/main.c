#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "portable.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"
//#include "projdefs.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


#define SEMAPHORE_MAX_COUNT         1
#define SEMAPHORE_INITIAL_COUNT     0

#define TOGGLE_LED_EVENT            ((EventBits_t) 1<<0)        //Toggle LED Event bit
#define LOG_STRING_EVENT            ((EventBits_t ) 1<<1)        //Log String Event bit

#define SYS_CLOCK_MHZ           1000000
#define SYS_CLOCK_FREQUENCY     120*SYS_CLOCK_MHZ
#define UART_PORT               0
#define BAUD_RATE               115200

SemaphoreHandle_t task1_sem, task2_sem;             //Semaphores for Taks 1 & task 2
uint32_t timer1_period, timer2_period;              //Timer periods for Timer 1 & Timer2
BaseType_t timer1_id, timer2_id;                    //Timer Id for the 2 timers used by the two tasks.

EventGroupHandle_t toggle_led_log_uart_event_group;         //Event Group used tos ignal happening of Toggle LED and Log UART events.
QueueHandle_t log_queue;                                    //Queue used to send the Log string from Task 2 to Task 3.

//Timer Callback Handler for both timer 1 and timer 2. TimerId is used to differentiate between the two timers and give the semaphore for
//the appropriate task based on the which timer has set off.
void timer_callback(TimerHandle_t timer)
{
    BaseType_t *timer_id = (BaseType_t *) pvTimerGetTimerID(timer);
    BaseType_t pxHigherPriorityTaskWoken = pdTRUE;

    if(*timer_id == timer1_id)
        xSemaphoreGiveFromISR(task1_sem, &pxHigherPriorityTaskWoken);       //If timer1 has been set off then the callback handler gives the semaphore for task 1
    else if(*timer_id == timer2_id)
        xSemaphoreGiveFromISR(task2_sem, &pxHigherPriorityTaskWoken);       //If timer2 has been set off then the callback handler gives the semaphore for task 2
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
        xEventGroupSetBits(toggle_led_log_uart_event_group, TOGGLE_LED_EVENT);
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
        TickType_t tick_count;
        char queue_string[30];

        xSemaphoreTake(task2_sem, portMAX_DELAY);
        tick_count = xTaskGetTickCount();
        sprintf(queue_string, "Tick Count = %u\n", tick_count);

        xQueueSend(log_queue, queue_string, portMAX_DELAY);


        xEventGroupSetBits(toggle_led_log_uart_event_group, LOG_STRING_EVENT);
    }
}

//Task 3 does the actual toggling LED and logging the Tick Count value. While Task 1 & Task 2 notify this task to do the mentioned activities by using event groups
void vTask3( void *pvParameters )
{
    const char *timer_name = "Timer 2\r\n";
    BaseType_t led_state = pdFALSE;
    EventBits_t event_group_bits;

    while(1)
    {
        event_group_bits = xEventGroupWaitBits(toggle_led_log_uart_event_group, (TOGGLE_LED_EVENT | LOG_STRING_EVENT), pdTRUE, pdFALSE, portMAX_DELAY);

        if(event_group_bits & TOGGLE_LED_EVENT)
        {
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
        if(event_group_bits & LOG_STRING_EVENT)
        {
            char buffer[30];
            xQueueReceive(log_queue, buffer, portMAX_DELAY);
            UARTprintf(buffer);
        }
    }
}
//This used to configure the UART By Enabling the Appropriate GPIO peripheral used by the UART0 and also to set the pin modes of UART0 RX and TX
void UART_Configure(uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTStdioConfig(UART_PORT, BAUD_RATE, ui32SrcClock);
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

    UART_Configure(sys_clock_get);

    timer1_period = 1000/4;
    timer2_period = 1000/8;

    toggle_led_log_uart_event_group = xEventGroupCreate();

    log_queue = xQueueCreate(10, 30);

    /* Create Task 1*/
    xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL ); /* This example does not use the task handle. */
    /* Create Task 2*/
    xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
    /*Creating Task 3*/
    xTaskCreate( vTask3, "Task 3", 1000, NULL, 2, NULL );
    /* Start the scheduler so the tasks start executing. */
    vTaskStartScheduler();
    for( ;; );
    return 0;
}
