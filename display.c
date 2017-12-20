/*- Headerfiles -------------------------------------------------------------*/
#include <stdbool.h>                /* Libc Standard boolean                 */
#include <stdint.h>                 /* Libc Standard integer                 */
#include "tm4c1294ncpdt.h"          /* TivaWare Hardware Register Map        */
#include "inc/hw_memmap.h"          /* TivaWare Memory Map                   */
#include "driverlib/gpio.h"         /* TivaWare GPIO DriverLib               */
#include "driverlib/pin_map.h"      /* TivaWare GPIO Pin Mapping             */
#include "driverlib/sysctl.h"       /* TivaWare SysCtl DriverLib             */
#include "delay.h"                  /* Delay Module                          */
#include "display.h"


/*- Local Prototypes --------------------------------------------------------*/
static void vDisplayInitGPIO(void);

static void vDisplaySendCmd(uint8_t ucCommand);
static void vDisplaySendData(uint8_t ucData);


/**
 *  @brief  Configure GPIO for Display-Interface
 *
 *  PORTL: Control lines
 *  PORTM: Data lines (parallel interface)
 *                                                                           */
void vDisplayInitGPIO(void)
{
    /* GPIO Clock                                                            */
    SysCtlPeripheralEnable(DISPLAY_CTL_RCGC);
    SysCtlPeripheralEnable(DISPLAY_DATA_RCGC);
    asm("\tnop\r\n\tnop\r\n\tnop\r\n"); /* Lock time > 3 Cycles              */

    /* Configure digital function                                            */
    GPIOPadConfigSet(DISPLAY_CTL_PORT, 0x1F, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(DISPLAY_DATA_PORT, 0xFF, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    /* Set pin directions                                                    */
    GPIODirModeSet(DISPLAY_CTL_PORT, 0x1F, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(DISPLAY_DATA_PORT, 0xFF, GPIO_DIR_MODE_OUT);

    /* Set initial values                                                    */
    DISPLAY_CTL_PORT_R = 0x1F;
    DISPLAY_DATA_PORT_R = 0x00;
}

/**
 *  @brief  Send command byte to display
 *
 *  Reference    [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section A.2.1
 *
 *  @param[in]  ucCommand   Display command byte as per Datasheet
 *                                                                           */
void vDisplaySendCmd(uint8_t ucCommand)
{
    DISPLAY_CTL_PORT_R = 0x1F;      /* Initial state                         */

    DISPLAY_CTL_PORT_R &= ~(1 << DISPLAY_CTL_PIN_CS); /* Activate CS         */
    DISPLAY_CTL_PORT_R &= ~(1 << DISPLAY_CTL_PIN_RS); /* Command mode        */
    DISPLAY_CTL_PORT_R &= ~(1 << DISPLAY_CTL_PIN_WR); /* Write state         */

    DISPLAY_DATA_PORT_R = ucCommand; /* Command                              */

    DISPLAY_CTL_PORT_R |= 1 << DISPLAY_CTL_PIN_WR;    /* Exit write state    */
    DISPLAY_CTL_PORT_R |= 1 << DISPLAY_CTL_PIN_CS;    /* Disable chip select */
}

/**
 *  @brief  Send data byte to display
 *
 *  Reference   [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section A.2.2
 *
 *  @param[in]  ucData  Display data byte
 *                                                                           */
void vDisplaySendData(uint8_t ucData)
{
    DISPLAY_CTL_PORT_R = 0x1F;      /* Initial state                         */

    DISPLAY_CTL_PORT_R &= ~(1 << DISPLAY_CTL_PIN_CS); /* Activate CS         */
    DISPLAY_CTL_PORT_R |= 1 << DISPLAY_CTL_PIN_RS;    /* Data mode           */
    DISPLAY_CTL_PORT_R &= ~(1 << DISPLAY_CTL_PIN_WR); /* Write state         */

    DISPLAY_DATA_PORT_R = ucData;   /* Data                                  */

    DISPLAY_CTL_PORT_R |= 1 << DISPLAY_CTL_PIN_WR;    /* Exit write state    */
    DISPLAY_CTL_PORT_R |= 1 << DISPLAY_CTL_PIN_CS;    /* Disable chip select */
}

/**
 *  @brief  Initialise display controller for TFT panel used in the lab.
 *
 *  Reference   [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section A.3
 *                                                                           */
void vDisplayInit(void)
{
    vDisplayInitGPIO();             /* Setup GPIO registers                  */

    /* Reset display                                                         */
    DISPLAY_CTL_PORT_R = 0x1F;      /* Initial state                         */
    DISPLAY_CTL_PORT_R &= ~(1 << DISPLAY_CTL_PIN_RST); /* Reset display      */
    vDelay_us(150);
    DISPLAY_CTL_PORT_R |= 1 << DISPLAY_CTL_PIN_RST;    /* Enable display     */

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

/**
 *  @brief  Set display window for next pixel block
 *
 *  Reference  [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section A.4
 */
void vDisplayWindowSet(uint16_t uiColStart, uint16_t uiColEnd, uint16_t uiRowStart, uint16_t uiRowEnd)
{
    /* Set page number (X direction)                                         */
    vDisplaySendCmd(DISPLAY_CMD_SETPAGE);
    vDisplaySendData(uiColStart >> 8);
    vDisplaySendData(uiColStart & 0xFF);
    vDisplaySendData(uiColEnd >> 8);
    vDisplaySendData(uiColEnd & 0xFF);

    /* Set column number (Y direction)                                       */
    vDisplaySendCmd(DISPLAY_CMD_SETCOL);
    vDisplaySendData(uiRowStart >> 8);
    vDisplaySendData(uiRowStart & 0xFF);
    vDisplaySendData(uiRowEnd >> 8);
    vDisplaySendData(uiRowEnd & 0xFF);
}

/**
 *  @brief  Begin pixel block
 *
 *  Reference  [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section A.5
 *                                                                           */
void vDisplayStartPixelWrite(void)
{
    vDisplaySendCmd(DISPLAY_CMD_WRITEPX);
}

/**
 *  @brief  Write single pixel data
 *
 *  Reference  [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section A.5
 *
 *  @note   vDisplayStartPixelWrite  is required  prior to first  call to this
 *          function.
 *
 *  @param[in]  ucRed   Red color component
 *  @param[in]  ucGreen Green color component
 *  @param[in]  ucBlue  Blue color component
 *                                                                           */
void vDisplayPixelWrite(uint8_t ucRed, uint8_t ucGreen, uint8_t ucBlue)
{
    vDisplaySendData(ucRed);
    vDisplaySendData(ucGreen);
    vDisplaySendData(ucBlue);
}

/**
 *  @brief  Clear display (overwrite entire area with black pixels)
 *                                                                           */
void vDisplayClear(void)
{
	uint32_t ulCounter;

    vDisplayWindowSet(0,480-1,0,272-1);
    vDisplayStartPixelWrite();
    for (ulCounter = 0; ulCounter < (480*272); ++ulCounter)
    {
    	vDisplayPixelWrite(0x00, 0x00, 0x00);
    }
}
