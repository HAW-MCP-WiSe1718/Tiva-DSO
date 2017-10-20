/*- Headerdateien -----------------------------------------------------------*/
#include "display.h"
#include "delay.h"


/*- Modulglobale Funktionsprototypen ----------------------------------------*/
static void vDisplayInitGPIO(void);

static void vDisplaySendCmd(uint8_t ucCommand);
static void vDisplaySendData(uint8_t ucData);

void vDisplayInitGPIO(void)
{
    /* GPIO Sysclock                */
    SYSCTL_RCGCGPIO_R |= (1 << 10) | (1 << 11);     /* PORTL, PORTM          */
    asm("\tnop\r\n\tnop\r\n\tnop\r\n");             /* Lock time             */

    /* Enable digital function      */
    DISPLAY_CTL_DEN = 0x1F;
    DISPLAY_DATA_DEN = 0xFF;

    /* Setup Data direction         */
    DISPLAY_CTL_DIR = 0x1F;                         /* All Output            */
    DISPLAY_DATA_DIR = 0xFF;                        /* All Output            */

    /* Initial values               */
    DISPLAY_CTL_PORT = 0x1F;
    DISPLAY_DATA_PORT = 0x00;
}


void vDisplaySendCmd(uint8_t ucCommand)
{
    DISPLAY_CTL_PORT = 0x1F;        /* Initial state                         */

    DISPLAY_CTL_PORT &= ~(1 << DISPLAY_CTL_PIN_CS); /* Activate chip select  */
    DISPLAY_CTL_PORT &= ~(1 << DISPLAY_CTL_PIN_RS); /* Command mode          */
    DISPLAY_CTL_PORT &= ~(1 << DISPLAY_CTL_PIN_WR); /* Write state           */

    DISPLAY_DATA_PORT = ucCommand;  /* Command                               */

    DISPLAY_CTL_PORT |= 1 << DISPLAY_CTL_PIN_WR;    /* Exit write state      */
    DISPLAY_CTL_PORT |= 1 << DISPLAY_CTL_PIN_CS;    /* Disable chip select   */
}


void vDisplaySendData(uint8_t ucData)
{
    DISPLAY_CTL_PORT = 0x1F;        /* Initial state                         */

    DISPLAY_CTL_PORT &= ~(1 << DISPLAY_CTL_PIN_CS); /* Activate chip select  */
    DISPLAY_CTL_PORT |= 1 << DISPLAY_CTL_PIN_RS;    /* Data mode             */
    DISPLAY_CTL_PORT &= ~(1 << DISPLAY_CTL_PIN_WR); /* Write state           */

    DISPLAY_DATA_PORT = ucData;     /* Data                                  */

    DISPLAY_CTL_PORT |= 1 << DISPLAY_CTL_PIN_WR;    /* Exit write state      */
    DISPLAY_CTL_PORT |= 1 << DISPLAY_CTL_PIN_CS;    /* Disable chip select   */
}


void vDisplayInit(void)
{
    vDisplayInitGPIO();             /* Setup GPIO registers                  */

    /* Reset display                                                         */
    DISPLAY_CTL_PORT = 0x1F;        /* Initial state                         */
    DISPLAY_CTL_PORT &= ~(1 << DISPLAY_CTL_PIN_RST); /* Reset display        */
    vDelay_us(150);
    DISPLAY_CTL_PORT |= 1 << DISPLAY_CTL_PIN_RST;   /* Enable display        */

    /* Display soft-reset                                                    */
    vDisplaySendCmd(DISPLAY_CMD_SOFTRESET);
    vDelay_ms(6);

    /* Setup PLL Frequency = 100MHz                                          */
    vDisplaySendCmd(DISPLAY_CMD_SETPLL);
    vDisplaySendData(0x1D);
    vDisplaySendData(0x02);
    vDisplaySendData(0x04);

    vDisplaySendCmd(DISPLAY_CMD_STARTPLL);
    vDisplaySendData(0x03);
    vDelay_us(150);

    /* Soft-reset                                                            */
    vDisplaySendCmd(DISPLAY_CMD_SOFTRESET);
    vDelay_ms(6);

    /* Setup LCD panel                                                       */
    vDisplaySendCmd(DISPLAY_CMD_SETPCLK);
    vDisplaySendData(0x01);
    vDisplaySendData(0x70);
    vDisplaySendData(0xA3);

    vDisplaySendCmd(DISPLAY_CMD_SETLCDMODE);
    vDisplaySendData(0x20);
    vDisplaySendData(0x00);
    vDisplaySendData(0x01);
    vDisplaySendData(0xDF);
    vDisplaySendData(0x01);
    vDisplaySendData(0x0F);
    vDisplaySendData(0x00);

    /* Setup LCD timing                                                      */
    vDisplaySendCmd(DISPLAY_CMD_SETHORT);
    vDisplaySendData(0x02);
    vDisplaySendData(0x13);
    vDisplaySendData(0x00);
    vDisplaySendData(0x2B);
    vDisplaySendData(0x0A);
    vDisplaySendData(0x00);
    vDisplaySendData(0x08);
    vDisplaySendData(0x00);

    vDisplaySendCmd(DISPLAY_CMD_SETVERT);
    vDisplaySendData(0x01);
    vDisplaySendData(0x20);
    vDisplaySendData(0x00);
    vDisplaySendData(0x0C);
    vDisplaySendData(0x0A);
    vDisplaySendData(0x00);
    vDisplaySendData(0x04);

    /* Flip display for touch panel                                          */
    vDisplaySendCmd(DISPLAY_CMD_SETFLIP);
    vDisplaySendData(0x03);

    /* Set pixel data format: 8bit                                           */
    vDisplaySendCmd(DISPLAY_CMD_PIXDFMT);
    vDisplaySendData(0x00);

    /* Set display on                                                        */
    vDisplaySendCmd(DISPLAY_CMD_DISPON);
}


void vDisplayWindowSet(uint16_t uiColStart, uint16_t uiColEnd, uint16_t uiRowStart, uint16_t uiRowEnd)
{
    vDisplaySendCmd(DISPLAY_CMD_SETPAGE);
    vDisplaySendData(uiColStart >> 8);
    vDisplaySendData(uiColStart & 0xFF);
    vDisplaySendData(uiColEnd >> 8);
    vDisplaySendData(uiColEnd & 0xFF);

    vDisplaySendCmd(DISPLAY_CMD_SETCOL);
    vDisplaySendData(uiRowStart >> 8);
    vDisplaySendData(uiRowStart & 0xFF);
    vDisplaySendData(uiRowEnd >> 8);
    vDisplaySendData(uiRowEnd & 0xFF);
}


void vDisplayStartPixelWrite(void)
{
    vDisplaySendCmd(DISPLAY_CMD_WRITEPX);
}


void vDisplayPixelWrite(uint8_t ucRed, uint8_t ucGreen, uint8_t ucBlue)
{
    vDisplaySendData(ucRed);
    vDisplaySendData(ucGreen);
    vDisplaySendData(ucBlue);
}


void vDisplayClear(void)
{
	uint32_t ulCounter;

    vDisplayWindowSet(0,480-1,0,272-1);
    vDisplayStartPixelWrite();
    for (ulCounter = 0; ulCounter < (479*271); ++ulCounter)
    {
    	vDisplayPixelWrite(0x00, 0x00, 0x00);
    }
}
