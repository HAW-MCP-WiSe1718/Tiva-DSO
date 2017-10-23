/*- Header files ------------------------------------------------------------*/
#include "display.h"                /* Display module for screen size def.   */
#include "touch.h"

/*- Calibration values ------------------------------------------------------*/
#define TOUCH_CAL_MIN_X 158         /* Minimum X raw value                   */
#define TOUCH_CAL_MAX_X 3028        /* Maximum X raw value                   */
#define TOUCH_CAL_MIN_Y 271         /* Minimum Y raw value                   */
#define TOUCH_CAL_MAX_Y 3715        /* Maximum Y raw value                   */


/*- Inline functions --------------------------------------------------------*/
/**
 *  @brief  Short busy-wait delay
 *
 *  Reference    [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section B.1
 *                                                                           */
static inline void vTouchBusyWait(void)
{
    uint8_t c;
    for(c=0; c<10; ++c);            /* Short delay                           */
}


/*- Prototypes --------------------------------------------------------------*/
static void vTouchSend(uint8_t ucData);
static uint16_t uiTouchReceive(void);
static bool bIsTouchDataValid(tsTouchData sTouchData);


/**
 *  @brief  Send data byte to Touch controller
 *
 *  Reference    [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section B.1
 *
 *  @todo   Rewrite using Hardware SSI
 *  @param[in]  ucData  Data byte to be sent
 *                                                                           */
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

/**
 *  @brief  Read data word from Touch controller
 *
 *  Reference    [Pnr,  Pro,  Snd 9/13,  Pro8/2017]  "Projektaufgabe:  Digitales
 *  Speicheroszilloskop",  Section B.1
 *
 *  @todo   Rewrite using Hardware SSI
 *  @return uint16_t    Data word from Touch controller
 *                                                                           */
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

    /* Data is 12 Bits long. 0 appended after last bit
     * 4 extra clock cycles for debugging purposes                           */
    uiValue >>= 4;

    return uiValue;
}


/**
 *  @brief  Check, whether raw data is representing a valid point on the
 *          screen, or is showing "pen up"
 *
 *  @return bool    True, if pen is down and position is within the display
 *                  area.
 *                                                                           */
static bool bIsTouchDataValid(tsTouchData sTouchData)       /* inline? */
{
    return (sTouchData.uiX > TOUCH_CAL_MIN_X) && (sTouchData.uiX < TOUCH_CAL_MAX_X) &&
           (sTouchData.uiY > TOUCH_CAL_MIN_Y) && (sTouchData.uiY < TOUCH_CAL_MAX_Y);
}

/**
 *  @brief  Initialise GPIO for the touch controller interface
 *                                                                           */
void vTouchInit(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;   /* PORTD                      */
    asm("\tnop\r\n\tnop\r\n\tnop\r\n");

    TOUCH_DATA_PORT = 0;

    /* Enable PORTD0...4                                                     */
    TOUCH_DATA_DEN |= 0x1F;

    /* Configure Direction                                                   */
    TOUCH_DATA_DIR = 0x0D;
}

/**
 *  @brief  Get raw touch data
 *
 *  @return tsTouchData Raw touch data (12 Bits for X and Y)
 *                                                                           */
tsTouchData sGetTouchData(void)
{
    tsTouchData sTouchData;

    /* Initial: Clk = 1                                                      */
    TOUCH_DATA_PORT &= ~(1 << TOUCH_DATA_PIN_CLK);

    /* Assert CS                                                             */
    TOUCH_DATA_PORT &= ~(1 << TOUCH_DATA_PIN_CS);   /* Assert CS             */

    /* Get X Position                                                        */
    vTouchSend(TOUCH_CMD_XPOS);
    vTouchBusyWait();
    sTouchData.uiX = uiTouchReceive();

    /* Get Y Position                                                        */
    vTouchSend(TOUCH_CMD_YPOS);
    vTouchBusyWait();
    sTouchData.uiY = uiTouchReceive();

    /* Deassert CS                                                           */
    TOUCH_DATA_PORT |= 1 << TOUCH_DATA_PIN_CS;    /* Deassert CS             */

    return sTouchData;
}

/**
 *  @brief  Get raw touch data and convert it to display coordinates
 *
 *  @todo   Rework using integer math exclusively
 *  @return tsTouchPos  Touch position as display coordinates, or {-1,-1}, if
 *                      pen is "up".
 *                                                                           */
tsTouchPos sGetTouchPos(void)
{
    tsTouchData sTouchData;

    /* Get current touch data                                                */
    sTouchData = sGetTouchData();

    /* Check, whether pen is down and data is valid                          */
    if (bIsTouchDataValid(sTouchData))
    {
        tsTouchPos sTouchPos;

        /* Pen down: convert to display coordinates                          */
        sTouchPos.iX = (sTouchData.uiX - TOUCH_CAL_MIN_X) * ((float)DISPLAY_SIZE_X/(TOUCH_CAL_MAX_X-TOUCH_CAL_MIN_X));
        sTouchPos.iY = (sTouchData.uiY - TOUCH_CAL_MIN_Y) * ((float)DISPLAY_SIZE_Y/(TOUCH_CAL_MAX_Y-TOUCH_CAL_MIN_Y));

        return sTouchPos;
    }
    else
    {
        /* Pen up: return {-1,-1}                                            */
        return TOUCH_POS_INVALID;
    }
}
