#ifndef UI_INPUT_H_
#define UI_INPUT_H_

/*- Headerfiles -------------------------------------------------------------*/
#include "ui.h"


/*- Defines -----------------------------------------------------------------*/
#define UI_MAX_NUM_BOXES    8       /* Maximum number of touch input areas   */


/*- Type definitions --------------------------------------------------------*/
typedef void (*tp_fUiTouchCallback)(void);

typedef struct tag_tsUiTouchBox {
    tsUiCoords sTopLeft;
    tsUiCoords sBtmRight;
    tp_fUiTouchCallback p_fOnTouch;
} tsUiTouchBox;

extern tsUiTouchBox asUiTouchBoxConfiguration[UI_MAX_NUM_BOXES];

/*- Prototypes --------------------------------------------------------------*/
void vUiTouchInit(void);
void vUiTouchPoll(void);

#endif /* UI_INPUT_H_ */
