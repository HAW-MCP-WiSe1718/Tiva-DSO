#include "graphics.h"

/**
 *  @brief  initialize graphical unit
 *
 *          Blacks out the display -> prints grid
 *                                                                           */
void vGraphicsInit()
{
    vDisplayClear();    //blackout the display
}


/**
 *  @brief  writes a pixel into displaymemory.
 *
 *  @param[in]  pixelColor  contains colorinformation of pixel
 *                                                                           */
void vGraphicsPixelWrite(tuGraphicsColor pixelColor)
{
    vDisplayPixelWrite(pixelColor.colorSeperate.uiRedValue, pixelColor.colorSeperate.uiGreenValue, pixelColor.colorSeperate.uiBlueValue);
}


//printRectangle prints a Rectangle
/**
 *  @brief  prints a rectangle directly on the display
 *
 *  @param[in]  uiX_Start,uiX_End,uiY_Start,uiY_End define the corners of the rectangle
 *              lineColor defines the color of the rectangle
 *                                                                           */
void vGraphicsPrintRectangle(uint16_t uiX_Start,uint16_t uiX_End,uint16_t uiY_Start,uint16_t uiY_End,tuGraphicsColor lineColor)
{
    uint16_t uiRow,uiColumn;

    vDisplayWindowSet(uiX_Start,uiX_End,uiY_Start,uiY_End);     //setting transmitwindow
    vDisplayStartPixelWrite();

    for(uiRow=uiY_Start;uiRow<=uiY_End;uiRow++)   //loop for selected rows
    {
        for(uiColumn=uiX_Start;uiColumn<=uiX_End;uiColumn++)  //loop for selected columns
        {
            vGraphicsPixelWrite(lineColor);     //transmit pixel of active row and column
        }
    }
}


/**
 *  @brief  prints a line between two points, including the second point, excluding the first point
 *
 *          Function uses the bresenham algorithm
 *
 *  @param[in]  A is coordinate of first point. B is coordinate of second point.
 *              lineColor defines the color of the line
 *                                                                           */
void vGraphicsDrawLine(tsGraphicsCoord A,tsGraphicsCoord B,tuGraphicsColor lineColor)
{

    uint16_t uiX=A.uiX,uiY=A.uiY,uidX=B.uiX-A.uiX,uidY;
    int16_t iDelta,iStep=1;

    if(B.uiY-A.uiY > 0)   //decision if line shall be printed in negative or positive uiY-direction
    {
        uidY=B.uiY-A.uiY;
    }
    else
    {
        uidY=A.uiY-B.uiY;
        iStep=-1;
    }


    iDelta=uidY>>1; //first delta = dY*0.5

    while(uiY!=B.uiY)  //until position of second point is reached
    {
        iDelta-=uidX;

        uiY+=iStep;     //step further in direction of uiY or -uiY depending on rising or falling line.

        if(iDelta<=0)
        {
            iDelta+=uidY;
            uiX++;
        }

        //write pixel for every step of uiY
        vDisplayWindowSet(uiX,uiX,271-uiY,271-uiY);
        vDisplayStartPixelWrite();
        vGraphicsPixelWrite(lineColor);
    }

}

