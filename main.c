/*- Header files ------------------------------------------------------------*/
#include <stdbool.h>                /* Libc Standard boolean                 */
#include <stdint.h>                 /* Libc Standard integer                 */
#include <stdio.h>                  /* Libc Standard I/O                     */
#include "driverlib/interrupt.h"    /* TivaWare Interrupt Library            */
#if 0
#include "dma.h"                    /* DMA init functions                    */
#include "display.h"                /* Display module                        */
#include "touch.h"                  /* Touch Controller module               */
#include "delay.h"                  /* Delay timer module                    */
#include "adc.h"                    /* ADC module                            */
#include "sampler.h"                /* Sampler module                        */
#include "trigger.h"                /* Trigger module                        */
#endif
#include "system.h"                 /* System configuration module           */
#include "ui.h"                     /* UI module: Input                      */
#include "scope.h"


extern void vScopeMainTouch_RunStop(tsUiCoords sTouchCoords);

/**
 *  @brief  Main program loop
 *                                                                           */
int main(void)
{
    /* Configure MOSC and PLL for 120MHz SysClk                              */
    vSystemClkInit();

    /* Init modules                                                          */
    vScopeInit();

    /* Enable interrupts                                                     */
    IntMasterEnable();

    /* Background task                                                       */
    while(1)
    {
        /* Handle UI and touch input                                         */
        vScopePoll();
    }
}
