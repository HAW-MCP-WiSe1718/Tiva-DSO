/*- Headerfiles -------------------------------------------------------------*/
#include <assert.h>                 /* Assertions                            */
#include <stdlib.h>                 /* Libc Standard Library Header          */
#include <stdbool.h>                /* Libc Standard Boolean                 */
#include "touch.h"                  /* Touch module                          */
#include "ui_input.h"


/*- Module variables --------------------------------------------------------*/
tsUiTouchBox asUiTouchBoxConfiguration[UI_MAX_NUM_BOXES] = {0};
static bool bUiTouchLastPenDown = false;


/**
 *  @brief  Initialise UI Touch module
 *                                                                           */
void vUiTouchInit(void)
{

}

/**
 *  @brief  Handle touch input.
 *
 *  This function will call the specified callback function for each box when
 *  a click is detected.
 *                                                                           */
void vUiTouchPoll(void)
{
    uint8_t ucBoxNum;
    tsTouchPos sTouchPos;

    /* Get current touch position                                            */
    sTouchPos = sGetTouchPos();

    /* Abort, if no pen detected                                             */
    if (!bTouchIsPenDown(sTouchPos))
    {
        bUiTouchLastPenDown = false;
        return;
    }

    /* Abort, if pen has already been pressed                                */
    if (bUiTouchLastPenDown)
        return;

    /* Edge detect: Pen touches screen                                       */
    bUiTouchLastPenDown = true;

    /* Find out, which box has been pressed                                  */
    for (ucBoxNum = 0; ucBoxNum < UI_MAX_NUM_BOXES; ++ucBoxNum)
    {
        if ((asUiTouchBoxConfiguration[ucBoxNum].sTopLeft.iX <= sTouchPos.iX) &&
            (asUiTouchBoxConfiguration[ucBoxNum].sTopLeft.iY <= sTouchPos.iY) &&
            (asUiTouchBoxConfiguration[ucBoxNum].sBtmRight.iX >= sTouchPos.iX) &&
            (asUiTouchBoxConfiguration[ucBoxNum].sBtmRight.iY >= sTouchPos.iY))
        {
            /* Found box, run callback function                              */
            asUiTouchBoxConfiguration[ucBoxNum].p_fOnTouch();
        }
    }
}

/**
 *  @brief  Add a new touch box
 *                                                                           */
void vUiTouchAddBox(int iArrayIndex, tsUiCoords sTopLeft, tsUiCoords sBtmRight, tp_fUiTouchCallback p_fCallback)
{
    assert(iArrayIndex >= 0 && iArrayIndex < UI_MAX_NUM_BOXES);

    asUiTouchBoxConfiguration[iArrayIndex].sTopLeft = sTopLeft;
    asUiTouchBoxConfiguration[iArrayIndex].sBtmRight = sBtmRight;
    asUiTouchBoxConfiguration[iArrayIndex].p_fOnTouch = p_fCallback;
}
