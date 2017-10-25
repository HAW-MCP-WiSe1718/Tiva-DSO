/*- Header files ------------------------------------------------------------*/
#include "delay.h"


/**
 *  @brief  Initialise Timers for delay routines
 *                                                                           */
void vDelayInit(void)
{
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;      /* Timer 0               */
    asm("\tnop\r\n\tnop\r\n\tnop\r\n");
}

/**
 *  @brief Timer-controlled busy wait for x*1/10ms
 *
 *  @param[in]  uiTenthMillis   Zehntel-Millisekunden Verzögerung
 *                                                                           */
void vDelay_us(uint16_t uiTenthMillis)
{
    uint16_t uiCounter = 0;

    /* Configure Timer0A as Delay timer with 1us period                      */
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;    /* Disable timer before making chgs. */
    TIMER0_CFG_R = 0x04;
    TIMER0_TAMR_R = 0x02;               /* Periodic mode                     */

    /* Configure Timer match register to trigger at "0"                      */
    TIMER0_TAMATCHR_R = 0;

#if SYSCLK_25MHz
    /* Timer configuration for f_Sys = 25MHz                                 */
    TIMER0_TAILR_R = 10;
    TIMER0_TAPR_R = 250;
#else
    /* Timer configuration for f_Sys = 120MHz                                */
    TIMER0_TAILR_R = 100;
    TIMER0_TAPR_R = 120;
#endif

    /* Clear Timer interrupt                                                 */
    TIMER0_ICR_R |= TIMER_ICR_TATOCINT;

    /* Start Timer0A                                                         */
    TIMER0_CTL_R |= TIMER_CTL_TAEN;

    while(uiCounter < uiTenthMillis / 100)
    {
       /* Entered in 100µs Interval                                          */
       if (TIMER0_RIS_R & TIMER_RIS_TATORIS)
       {
           ++uiCounter;
           TIMER0_ICR_R |= TIMER_ICR_TATOCINT;
       }
    }

    /* Done. Stop timer                                                      */
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
}

/**
 *  @brief  Timer-controlled busy-wait for x*1ms
 *
 *  @param[in]  uiMilliseconds  Millisekunden Verzögerung
 *                                                                           */
void vDelay_ms(uint16_t uiMilliseconds)
{
    uint16_t uiCounter = 0;

    /* Configure Timer0A as Delay timer with 1us period                      */
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;    /* Disable timer before making chgs. */
    TIMER0_CFG_R = 0x04;
    TIMER0_TAMR_R = 0x02;               /* Periodic mode                     */

    /* Configure Timer match register to trigger at "0"                      */
    TIMER0_TAMATCHR_R = 0;

#if SYSCLK_25MHz
    /* Timer configuration for f_Sys = 25MHz                                 */
    TIMER0_TAILR_R = 100;
    TIMER0_TAPR_R = 250;
#else
    /* Timer configuration for f_sys = 120MHz                                */
    TIMER0_TAILR_R = 1000;
    TIMER0_TAPR_R = 120;
#endif

    /* Clear Timer interrupt                                                 */
    TIMER0_ICR_R |= TIMER_ICR_TATOCINT;

    /* Start Timer0A                                                         */
    TIMER0_CTL_R |= TIMER_CTL_TAEN;

    while(uiCounter < uiMilliseconds)
    {
       /* Entered in 1ms Interval                                            */
       if (TIMER0_RIS_R & TIMER_RIS_TATORIS)
       {
           ++uiCounter;
           TIMER0_ICR_R |= TIMER_ICR_TATOCINT;
       }
    }

    /* Done. Stop timer                                                      */
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
}
