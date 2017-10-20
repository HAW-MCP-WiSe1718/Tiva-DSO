#ifndef TOUCH_H_
#define TOUCH_H_

/*- Headerdateien -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "tm4c1294ncpdt.h"


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


/*- Typdefinitionen ---------------------------------------------------------*/
typedef struct tag_tsTouchPos {
    uint16_t uiX;
    uint16_t uiY;
} tsTouchPos;

/*- Funktionsprototypen -----------------------------------------------------*/
void vTouchInit(void);
tsTouchPos sGetTouchPos(void);
bool bIsTouchPenDown(void);

#endif /* TOUCH_H_ */
