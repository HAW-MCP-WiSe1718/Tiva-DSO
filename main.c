/*- Header files ------------------------------------------------------------*/
#include <stdio.h>                  /* Libc Standard IO                      */
#include "display.h"                /* Display module                        */
#include "touch.h"                  /* Touch Controller module               */
#include "delay.h"                  /* Delay timer module                    */
#include "adc.h"                    /* ADC module                            */
#include "system.h"                 /* System configuration module           */
#include "graphics.h"               /* Graphics module                       */

#define SAMPLE_COUNT    480
static uint16_t m_auiSampleBuffer[SAMPLE_COUNT];
static uint8_t m_aucPrintBuffer[SAMPLE_COUNT];

/**
 *  @brief  Hauptprogramm
 *                                                                           */
int main(void)
{
    uint16_t uiCounter;
    tsTouchData sTouchData;
    tsTouchPos sTouchPos;
    uint16_t uiNextPrintIndex = 0;
    uint16_t uiLastSamplePosition = 0;

    /* Configure MOSC and PLL for 120MHz SysClk                              */
    vSystemClkInit();

    /* Init peripherals                                                      */
    vDelayInit();
    vDisplayInit();
    vTouchInit();
    vAdcInit();

    /* Clear any previous display data                                       */
    vDisplayClear();

    /* Main application                                                      */
#if 0
    vDisplayWindowSet(240, 247, 136, 136);
    vDisplayStartPixelWrite();
    for (uiCounter = 0; uiCounter < 8; ++uiCounter)
    {
        vDisplayPixelWrite(0xFF, 0xFF, 0xFF);
    }
#endif
    graphicsInit();

    while(1)
    {
        sTouchData = sGetTouchData();
    	sTouchPos = sGetTouchPos();

        //printf("Data X: %4d, Y: %4d\tPos X:%3d, Y:%3d\r\n", sTouchData.uiX, sTouchData.uiY, sTouchPos.iX, sTouchPos.iY);

        if (sTouchPos.iX != -1)
        {
        	vDisplayWindowSet(sTouchPos.iX, sTouchPos.iX, sTouchPos.iY, sTouchPos.iY);
        	vDisplayStartPixelWrite();
        	vDisplayPixelWrite(0xFF, 0xFF, 0xFF);
        }
        else
        {
			/* ADC Sample einlesen                                               */
			vAdcStartSampleVin();
			m_auiSampleBuffer[uiLastSamplePosition] = uiGetAdcSampleVin();
			//printf("ADC [%d]: %4d\r\n", uiLastSamplePosition, m_auiSampleBuffer[uiLastSamplePosition]);
			++uiLastSamplePosition;
			uiLastSamplePosition %= SAMPLE_COUNT;

			/* Wellenform zeichnen                                               */
			color sColor = {.colorSeperate = {0x00, 0xFF, 0x00, 0x00}};
			uiNextPrintIndex = printWave(m_auiSampleBuffer, m_aucPrintBuffer, uiNextPrintIndex, uiLastSamplePosition, sColor);

        }
    }
}
