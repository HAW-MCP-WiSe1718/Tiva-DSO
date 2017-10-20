#include "delay.h"


void vDelayInit(void)
{
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;      /* Timer 0               */
    asm("\tnop\r\n\tnop\r\n\tnop\r\n");
}


void vDelay_us(uint16_t uiTenthMillis)
{
    uint16_t uiCounter = 0;

    /* Configure Timer0A as Delay timer with 1us period                      */
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;    /* Disable timer before making chgs. */
    TIMER0_CFG_R = 0x04;
    TIMER0_TAMR_R = 0x02;               /* Periodic mode                     */

    /* Preload Value: 16MHz Sysclock, 8bit Prescaler                         */
    /*      ILV: 125
     *      PRE: 128
     *      CLK: 16MHz
     */
    /*TIMER0_TAILR_R = 125;
    TIMER0_TAPR_R = 128;*/

    TIMER0_TAMATCHR_R = 0;

    TIMER0_TAILR_R = 10;
    TIMER0_TAPR_R = 250;

    TIMER0_ICR_R |= TIMER_ICR_TATOCINT;

    /* Start Timer0A                                                         */
    TIMER0_CTL_R |= TIMER_CTL_TAEN;

    while(uiCounter < uiTenthMillis / 100)
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


void vDelay_ms(uint16_t uiMilliseconds)
{
    uint16_t uiCounter = 0;

    /* Configure Timer0A as Delay timer with 1us period                      */
    TIMER0_CTL_R &= ~TIMER_CTL_TAEN;    /* Disable timer before making chgs. */
    TIMER0_CFG_R = 0x04;
    TIMER0_TAMR_R = 0x02;               /* Periodic mode                     */

    /* Preload Value: 16MHz Sysclock, 8bit Prescaler                         */
    /*      ILV: 125
     *      PRE: 128
     *      CLK: 16MHz
     */
    /*TIMER0_TAILR_R = 125;
    TIMER0_TAPR_R = 128;*/

    TIMER0_TAMATCHR_R = 0;

    TIMER0_TAILR_R = 100;
    TIMER0_TAPR_R = 250;

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
