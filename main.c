/*- Header files ------------------------------------------------------------*/
#include <stdbool.h>                /* Libc Standard boolean                 */
#include <stdint.h>                 /* Libc Standard integer                 */
#include <stdio.h>                  /* Libc Standard I/O                     */
#include "driverlib/interrupt.h"    /* TivaWare Interrupt Library            */
#include "system.h"                 /* System configuration module           */
#include "ui.h"                     /* UI module: Input                      */
#include "scope.h"                  /* Main application: Scope               */

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
