#ifndef UI_TOUCH_H_
#define UI_TOUCH_H_

/*- Headerfiles -------------------------------------------------------------*/
#include "ui.h"


/*- Defines -----------------------------------------------------------------*/
#define UI_MAX_NUM_BOXES    16       /* Maximum number of touch input areas   */


/*- Type definitions --------------------------------------------------------*/
typedef void (*tp_fUiTouchCallback)(tsUiCoords sTouchPosition);

typedef struct tag_tsUiTouchBox {
    tsUiCoords sTopLeft;
    tsUiCoords sBtmRight;
    tp_fUiTouchCallback p_fOnTouch;
} tsUiTouchBox;

extern tsUiTouchBox asUiTouchBoxConfiguration[UI_MAX_NUM_BOXES];

/*- Prototypes --------------------------------------------------------------*/
void vUiTouchInit(void);
void vUiTouchPoll(void);
void vUiTouchAddBox(int iArrayIndex, tsUiCoords sTopLeft, tsUiCoords sBottomRight, tp_fUiTouchCallback p_fCallback);

#endif /* UI_INPUT_H_ */
