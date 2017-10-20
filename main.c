#include "display.h"
#include "touch.h"
#include "delay.h"
#include "mpp1.h"
#include <stdio.h>

/**
 * main.c
 */
int main(void)
{
    uint16_t uiCounter;
    tsTouchPos sTouchPos;

    SYSCTL_MOSCCTL_R &= ~(SYSCTL_MOSCCTL_OSCRNG | SYSCTL_MOSCCTL_PWRDN | SYSCTL_MOSCCTL_NOXTAL);
    SYSCTL_MOSCCTL_R |= SYSCTL_MOSCCTL_OSCRNG;
    SYSCTL_RSCLKCFG_R = SYSCTL_RSCLKCFG_OSCSRC_MOSC;

    vDelayInit();

    /*SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;
    asm("\tnop\r\n\tnop\r\n\tnop\r\n");

    GPIO_PORTM_DEN_R |= 1 << 0;
    GPIO_PORTM_DIR_R |= 1 << 0;
    GPIO_PORTM_DATA_R |= 1 << 0;

    while(1)
    {
        vDelay_us(100);
        GPIO_PORTM_DATA_R ^= (1 << 0);
    }*/

    /*vDisplayInit();

    vDisplayClear();

    vDisplayWindowSet(240, 247, 136, 136);
    vDisplayStartPixelWrite();
    for (uiCounter = 0; uiCounter < 8; ++uiCounter)
    {
        vDisplayPixelWrite(0xFF, 0xFF, 0xFF);
    }

    while(1)
    {
        ;
    }*/

    vTouchInit();


    while(1)
    {
        sTouchPos = sGetTouchPos();
        uiCounter = bIsTouchPenDown();

        printf("Pen: %d. Pos X:%3d, Y:%3d\r\n", uiCounter, sTouchPos.uiX, sTouchPos.uiY);

        vDelay_ms(1000);
    }
}
