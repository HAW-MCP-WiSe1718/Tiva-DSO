/*- Header files ------------------------------------------------------------*/
#include <stdbool.h>                /* Libc Standard Boolean                 */
#include <stdint.h>                 /* Libc Standard Integer                 */
#include <inc/hw_memmap.h>          /* TivaWare Hardware Memory Map          */
#include <driverlib/timer.h>        /* TivaWare Timer DriverLib              */
#include <driverlib/sysctl.h>       /* TivaWare SysCtl DriverLib             */
#include "delay.h"


/**
 *  @brief  Initialise Timers for delay routines
 *                                                                           */
void vDelayInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    asm("\tnop\r\n\tnop\r\n\tnop\r\n");
}

/**
 *  @brief Timer-controlled busy wait
 *
 *  @param[in]  uiMicroseconds      Delay in microseconds, rounded to the
 *                                  next 100us.
 *                                                                           */
void vDelay_us(uint16_t uiMicroseconds)
{
    uint16_t uiCounter = 0;

    /* Use system clock as timer clock source                                */
    TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);

    /* Configure Timer for periodic counting                                 */
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC | TIMER_CFG_A_ACT_NONE | TIMER_CFG_SPLIT_PAIR);
    TimerMatchSet(TIMER0_BASE, TIMER_A, 0);

    /* Setup Timer prescaler and interval load values for f_SysClk           */
    TimerLoadSet(TIMER0_BASE, TIMER_A, 100);
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 120);

    /* Clear Interrupt flag and start timer                                  */
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);

    while (uiCounter < uiMicroseconds / 100)
    {
        /* Busy wait until timer timeout interrupt flag is set               */
        if (TimerIntStatus(TIMER0_BASE, false) & TIMER_TIMA_TIMEOUT)
        {
            ++uiCounter;

            /* Clear timeout interrupt flag                                  */
            TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        }
    }

    /* Done. Stop the timer                                                  */
    TimerDisable(TIMER0_BASE, TIMER_A);
}

/**
 *  @brief  Timer-controlled busy-wait
 *
 *  @param[in]  uiMilliseconds  Delay in milliseconds
 *                                                                           */
void vDelay_ms(uint16_t uiMilliseconds)
{
    uint16_t uiCounter = 0;

    /* Use system clock as timer clock source                                */
    TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);

    /* Configure Timer for periodic counting                                 */
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC | TIMER_CFG_A_ACT_NONE | TIMER_CFG_SPLIT_PAIR);
    TimerMatchSet(TIMER0_BASE, TIMER_A, 0);

    /* Setup Timer prescaler and interval load values for f_SysClk           */
    TimerLoadSet(TIMER0_BASE, TIMER_A, 1000);
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 120);

    /* Clear Interrupt flag and start timer                                  */
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);

    while (uiCounter < uiMilliseconds)
    {
        /* Busy wait until timer timeout interrupt flag is set               */
        if (TimerIntStatus(TIMER0_BASE, false) & TIMER_TIMA_TIMEOUT)
        {
            ++uiCounter;

            /* Clear timeout interrupt flag                                  */
            TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        }
    }

    /* Done. Stop the timer.                                                 */
    TimerDisable(TIMER0_BASE, TIMER_A);
}
