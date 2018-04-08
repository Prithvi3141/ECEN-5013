#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"


#define SYS_CLOCK_MHZ           1000000
#define SYS_CLOCK_FREQUENCY     120*SYS_CLOCK_MHZ
#define UART_PORT               0
#define BAUD_RATE               115200


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
void main(void)
{
    uint32_t sys_clock_get;
    int count = 1;
    va_list print_count;
    print_count.__ap = &count;

    sys_clock_get = SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_CFG_VCO_480, SYS_CLOCK_FREQUENCY);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));

    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    UART_Configure(sys_clock_get);

    UARTprintf("Project For: Prithvi Veeravalli 04/06/2018");

    while(1)
    {
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);

        SysCtlDelay(sys_clock_get/(4*3) );

        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);

        SysCtlDelay(sys_clock_get/(4*3) );

        UARTvprintf("Count = %d\t", print_count);

        count++;

    }
}
