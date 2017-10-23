/*- Header files ------------------------------------------------------------*/
#include <stdio.h>                  /* Libc Standard IO                      */
#include "display.h"                /* Display module                        */
#include "touch.h"                  /* Touch Controller module               */
#include "delay.h"                  /* Delay timer module                    */

/**
 *  @brief  Hauptprogramm
 *                                                                           */
int main(void)
{
    uint16_t uiCounter;
    //tsTouchData sTouchData;
    tsTouchPos sTouchPos;

    /* System clock configuration                                            */
    /* Reference  [Rms]  "Serielle  Kommunikation  RS-232+UART  ARM Cortex M4 /
     * TM4C1294", p. 62                                                      */
    SYSCTL_MOSCCTL_R &= ~(SYSCTL_MOSCCTL_OSCRNG | SYSCTL_MOSCCTL_PWRDN | SYSCTL_MOSCCTL_NOXTAL);
    SYSCTL_MOSCCTL_R |= SYSCTL_MOSCCTL_OSCRNG;
    SYSCTL_RSCLKCFG_R = SYSCTL_RSCLKCFG_OSCSRC_MOSC;

    /* -- Todo: PLL auf 120MHz -- */

    /* Init peripherals                                                      */
    vDelayInit();
    vDisplayInit();
    vTouchInit();

    /* Clear any previous display data                                       */
    vDisplayClear();

    /* Main application                                                      */
    vDisplayWindowSet(240, 247, 136, 136);
    vDisplayStartPixelWrite();
    for (uiCounter = 0; uiCounter < 8; ++uiCounter)
    {
        vDisplayPixelWrite(0xFF, 0xFF, 0xFF);
    }

    while(1)
    {
        //sTouchData = sGetTouchData();
    	sTouchPos = sGetTouchPos();

        //printf("Data X: %4d, Y: %4d\tPos X:%3d, Y:%3d\r\n", sTouchData.uiX, sTouchData.uiY, sTouchPos.iX, sTouchPos.iY);

        if (sTouchPos.iX != -1)
        {
        	vDisplayWindowSet(sTouchPos.iX, sTouchPos.iX, sTouchPos.iY, sTouchPos.iY);
        	vDisplayStartPixelWrite();
        	vDisplayPixelWrite(0xFF, 0xFF, 0xFF);
        }
    }
}
