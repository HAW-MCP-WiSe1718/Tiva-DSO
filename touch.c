/*- Headerdateien -----------------------------------------------------------*/
#include "touch.h"

/*- Inlinefunktionen --------------------------------------------------------*/
static inline vTouchBusyWait(void)
{
    uint8_t c;

    for(c=0; c<10; ++c);                /* Short delay                       */
}


/*- Modulglobale Funktionsprototypen ----------------------------------------*/
static void vTouchSend(uint8_t ucData);
static uint16_t uiTouchReceive(void);


void vTouchSend(uint8_t ucData)
{
    uint8_t ucBitCounter;

    /* "Bit bang" output each data bit                                       */
    for (ucBitCounter = 8; ucBitCounter > 0; --ucBitCounter)
    {
        /* Set output bit                                                    */
        if (ucData & 0x80)  /* MSB first */
        {
            TOUCH_DATA_PORT |= 1 << TOUCH_DATA_PIN_MOSI;
        }
        else
        {
            TOUCH_DATA_PORT &= ~(1 << TOUCH_DATA_PIN_MOSI);
        }

        ucData <<= 1;

        /* Clock                                                             */
        TOUCH_DATA_PORT |= 1 << TOUCH_DATA_PIN_CLK;
        vTouchBusyWait();
        TOUCH_DATA_PORT &= ~(1 << TOUCH_DATA_PIN_CLK);
        vTouchBusyWait();
    }

    //TOUCH_DATA_PORT |= 1 << TOUCH_DATA_PIN_CS;      /* Deassert CS           */
}


uint16_t uiTouchReceive(void)
{
    uint8_t ucBitCounter;
    uint16_t uiValue = 0;

    for (ucBitCounter = 16; ucBitCounter > 0; --ucBitCounter)
    {
        /* MSB first */
        uiValue <<= 1;

        /* Clock                                                             */
        TOUCH_DATA_PORT |= 1 << TOUCH_DATA_PIN_CLK;
        vTouchBusyWait();
        TOUCH_DATA_PORT &= ~(1 << TOUCH_DATA_PIN_CLK);
        vTouchBusyWait();

        uiValue |= (TOUCH_DATA_PORT >> TOUCH_DATA_PIN_MISO) & 0x01;
    }

    uiValue >>= 4;

    return uiValue;
}


void vTouchInit(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;   /* PORTD                 */
    asm("\tnop\r\n\tnop\r\n\tnop\r\n");

    TOUCH_DATA_PORT = 0;

    /* Enable PORTD0...4                                                     */
    TOUCH_DATA_DEN |= 0x1F;

    /* Configure Direction                                                   */
    TOUCH_DATA_DIR = 0x0D;
}


tsTouchPos sGetTouchPos(void)
{
    tsTouchPos sTouchPos;

    /* Initial: Clk = 1                                                      */
    TOUCH_DATA_PORT &= ~(1 << TOUCH_DATA_PIN_CLK);

    /* Assert CS                                                             */
    TOUCH_DATA_PORT &= ~(1 << TOUCH_DATA_PIN_CS);   /* Assert CS             */

    /* Get X Position                                                        */
    vTouchSend(TOUCH_CMD_XPOS);
    vTouchBusyWait();
    sTouchPos.uiX = uiTouchReceive();// / 8;           /* x/8 : 4096->480       */

    /* Get Y Position                                                        */
    vTouchSend(TOUCH_CMD_YPOS);
    vTouchBusyWait();
    sTouchPos.uiY = uiTouchReceive();// / 16;         /* x/16: 4096->256        */

    /* Deassert CS                                                           */
    TOUCH_DATA_PORT |= 1 << TOUCH_DATA_PIN_CS;    /* Deassert CS             */

    return sTouchPos;
}


bool bIsTouchPenDown(void)
{
	tsTouchPos sTouchPos;

	sTouchPos = sGetTouchPos();

	return !(sTouchPos.uiX == 0 && sTouchPos.uiY == 4095);
}
