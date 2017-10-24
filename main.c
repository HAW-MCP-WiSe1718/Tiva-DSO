/*- Header files ------------------------------------------------------------*/
#include <stdio.h>                  /* Libc Standard IO                      */
#include "display.h"                /* Display module                        */
#include "touch.h"                  /* Touch Controller module               */
#include "delay.h"                  /* Delay timer module                    */
#include "system.h"                 /* System configuration module           */

/**
 *  @brief  Hauptprogramm
 *                                                                           */
int main(void)
{
    uint16_t uiCounter;
    //tsTouchData sTouchData;
    tsTouchPos sTouchPos;

    /* Configure MOSC and PLL for 120MHz SysClk                              */
    vSystemClkInit();

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
