/*- Header files ------------------------------------------------------------*/
#include <stdbool.h>                /* Libc Standard boolean                 */
#include <stdint.h>                 /* Libc Standard integer                 */
#include <stdio.h>                  /* Libc Standard I/O                     */
#include "driverlib/interrupt.h"    /* TivaWare Interrupt Library            */
#include "display.h"                /* Display module                        */
#include "touch.h"                  /* Touch Controller module               */
#include "delay.h"                  /* Delay timer module                    */
#include "adc.h"                    /* ADC module                            */
#include "sampler.h"                /* Sampler module                        */
#include "system.h"                 /* System configuration module           */

/**
 *  @brief  Hauptprogramm
 *                                                                           */
int main(void)
{
    /* Configure MOSC and PLL for 120MHz SysClk                              */
    vSystemClkInit();

    /* Init peripherals                                                      */
    vDelayInit();
    vDisplayInit();
    vTouchInit();
    vAdcInit();
    vSamplerInit();

    /* Enable interrupts                                                     */
    IntMasterEnable();

    /* Clear any previous display data                                       */
    vDisplayClear();

    /* ADC Sampler Test                                                      */
    vSetSamplerTimebase(EN_SAMPLER_TIMEBASE_1ms);
    vSetSamplerTrigger(EN_SAMPLER_TRIGSRC_CONTINUOUS, EN_SAMPLER_TRIGMODE_NORMAL);

    while(1)
    {
        printf("CPU Temp: %3d\tADC PE1: %4d\r\n", iGetAdcChipTemperature(), g_aiSampleBuffer[0]);
        g_aiSampleBuffer[0] = SAMPLER_SAMPLE_INVALID;

        vDelay_ms(100);
    }
}
