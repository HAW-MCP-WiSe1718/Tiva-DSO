#ifndef TOUCH_H_
#define TOUCH_H_

/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* C Standard integer                    */
#include <stdbool.h>                /* C Standard boolean                    */
#include "tm4c1294ncpdt.h"          /* TivaWare Library                      */


/*- Konstantendefinitionen --------------------------------------------------*/
#define TOUCH_DATA_DEN          GPIO_PORTD_AHB_DEN_R    /* Port DEN          */
#define TOUCH_DATA_DIR          GPIO_PORTD_AHB_DIR_R    /* Port DIR          */
#define TOUCH_DATA_PORT         GPIO_PORTD_AHB_DATA_R   /* Port Daten        */
#define TOUCH_DATA_PIN_IRQ      4                       /* Interrupt In      */
#define TOUCH_DATA_PIN_CLK      3                       /* Clock Out         */
#define TOUCH_DATA_PIN_CS       2                       /* Chip select Out   */
#define TOUCH_DATA_PIN_MISO     1                       /* "TDO"             */
#define TOUCH_DATA_PIN_MOSI     0                       /* "TDI"             */

#define TOUCH_CMD_XPOS          0xD0        /* Read X Position command       */
#define TOUCH_CMD_YPOS          0x90        /* Read Y Position command       */

#define TOUCH_POS_INVALID       (tsTouchPos){-1,-1}     /* Position invalid  */

#define TOUCH_AVG_SAMPLES		30			/* Number of averaging samples	 */


/*- Type definitions --------------------------------------------------------*/
/**
 *  @struct tsTouchData
 *  @brief  Touch-Rohdaten
 *                                                                           */
typedef struct __attribute__((packed)) tag_tsTouchData {
    /*! @brief  X-Rohwert                                                    */
    uint16_t uiX;
    /*! @brief  Y-Rohwert                                                    */
    uint16_t uiY;
} tsTouchData;

/**
 *  @struct tsTouchPos
 *  @brief  Touch-Position als Displaykoordinaten
 *                                                                           */
typedef struct __attribute__((packed)) tag_tsTouchPos {
    /*! @brief  X-Position (vom linken Rand aus)                             */
    int16_t iX;
    /*! @brief  Y-Position (vom oberen Rand aus)                             */
    int16_t iY;
} tsTouchPos;


/*- Prototypes --------------------------------------------------------------*/
void vTouchInit(void);
tsTouchData sGetTouchData(void);
tsTouchPos sGetTouchPos(void);

#endif /* TOUCH_H_ */
