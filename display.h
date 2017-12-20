#ifndef DISPLAY_H_
#define DISPLAY_H_

/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                             /* C Standard Integer Typen  */


/*- Defines -----------------------------------------------------------------*/
#define DISPLAY_SIZE_X      480                 /* Display Breite in px      */
#define DISPLAY_SIZE_Y      272                 /* Display H�he in pc        */

#define DISPLAY_CTL_RCGC    SYSCTL_PERIPH_GPIOL /* Control Pins GPIO Periph. */
#define DISPLAY_CTL_PORT    GPIO_PORTL_BASE     /* Control Pins GPIO Port    */
#define DISPLAY_CTL_PORT_R  GPIO_PORTL_DATA_R   /* Ctrl Pins GPIO Port Reg.  */

#define DISPLAY_CTL_PIN_RD  0                   /* Read-Pin Out              */
#define DISPLAY_CTL_PIN_WR  1                   /* Write-Pin Out             */
#define DISPLAY_CTL_PIN_RS  2                   /* R/S Pin Out               */
#define DISPLAY_CTL_PIN_CS  3                   /* Chip select Out           */
#define DISPLAY_CTL_PIN_RST 4                   /* Display Reset Out         */

#define DISPLAY_DATA_RCGC   SYSCTL_PERIPH_GPIOM /* Data Pins GPIO Peripheral */
#define DISPLAY_DATA_PORT   GPIO_PORTM_BASE     /* Data Pins GPIO Port       */
#define DISPLAY_DATA_PORT_R GPIO_PORTM_DATA_R   /* Data Pins GPIO Port Reg.  */

#define DISPLAY_CMD_SOFTRESET   0x01            /* Software reset command    */
#define DISPLAY_CMD_SETPLL      0xE2            /* Set PLL Freq. command     */
#define DISPLAY_CMD_STARTPLL    0xE0            /* Start PLL                 */
#define DISPLAY_CMD_SETPCLK     0xE6            /* Set Pixel clock           */
#define DISPLAY_CMD_SETLCDMODE  0xB0            /* Set LCD panel mode        */
#define DISPLAY_CMD_SETHORT     0xB4            /* Set horizontal period     */
#define DISPLAY_CMD_SETVERT     0xB6            /* Set vertical period       */
#define DISPLAY_CMD_SETFLIP     0x36            /* Set flip                  */
#define DISPLAY_CMD_PIXDFMT     0xF0            /* Set pixel data format     */
#define DISPLAY_CMD_DISPON      0x29            /* Set display on            */
#define DISPLAY_CMD_SETPAGE     0x2A            /* Set page address          */
#define DISPLAY_CMD_SETCOL      0x2B            /* Set column address        */
#define DISPLAY_CMD_WRITEPX     0x2C            /* Write pixel               */


/*- Prototypes -------------------------------------------------------------*/
void vDisplayInit(void);
void vDisplayWindowSet(uint16_t uiColStart, uint16_t uiColEnd, uint16_t uiRowStart, uint16_t uiRowEnd);
void vDisplayStartPixelWrite(void);
void vDisplayPixelWrite(uint8_t ucRed, uint8_t ucGreen, uint8_t ucBlue);
void vDisplayClear(void);

#endif /* DISPLAY_H_ */
