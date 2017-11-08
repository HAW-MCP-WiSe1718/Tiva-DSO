#include "graphics.h"


uint8_t auiPrintBuffer[GRAPHICS_PRINTBUFFER_SIZE];

/**
 *  @brief  prints the DSO grid
 *
 *          using vGraphicsPrintRectAngle()
 *
 *          GRID CAN BE MODIFIED BY CHANGING THE GRIDMAKROS
 *
 *                                                                           */
static inline void vGraphicsPrintGrid(void)
{

    uint8_t uiLineCounter;

    //printing vertical lines
    for(uiLineCounter=1;uiLineCounter<(GRAPHICS_GRIDXEND+1-GRAPHICS_GRIDXSTART)/GRAPHICS_GRIDXSPACE;uiLineCounter++)
    {
        vGraphicsPrintRectangle(GRAPHICS_GRIDXSTART+uiLineCounter*GRAPHICS_GRIDXSPACE,GRAPHICS_GRIDXSTART+uiLineCounter*GRAPHICS_GRIDXSPACE,GRAPHICS_GRIDYSTART,GRAPHICS_GRIDYEND,GRAPHICS_GRIDCOLOR);
    }

    //printing horizontal lines
    for(uiLineCounter=1;uiLineCounter<(GRAPHICS_GRIDYEND+1-GRAPHICS_GRIDYSTART)/GRAPHICS_GRIDYSPACE;uiLineCounter++)
    {
        vGraphicsPrintRectangle(GRAPHICS_GRIDXSTART,GRAPHICS_GRIDXEND,GRAPHICS_GRIDYSTART+uiLineCounter*GRAPHICS_GRIDYSPACE,GRAPHICS_GRIDYSTART+uiLineCounter*GRAPHICS_GRIDYSPACE,GRAPHICS_GRIDCOLOR);
    }

}


/**
 *  @brief  initialize graphical unit
 *
 *          Blacks out the display -> prints grid
 *                                                                           */
void vGraphicsInit()
{
    uint16_t uiCounter;

    vDisplayClear();    //blackout the display

    vGraphicsPrintGrid();    //print the grid

    for(uiCounter=0;uiCounter<GRAPHICS_PRINTBUFFER_SIZE;uiCounter++)
    {
        auiPrintBuffer[uiCounter]=0;
    }

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
    uint8_t uiRisingLine=1;

    if(B.uiY-A.uiY > 0)   //decision if line shall be printed in negative or positive uiY-direction
    {
        uidY=B.uiY-A.uiY;
    }
    else
    {
        uidY=A.uiY-B.uiY;
        uiRisingLine=0;
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

/**
 *  @brief  Kurzbeschreibung
 *
 *          vGraphicsPrintWave prints the wave (data saved in auiPrintBuffer) from uiX_Start to uiX_End.
 *          In cases where it is needed it uses the bresenHam algorithm to print lines.
 *          PREVIOUS WAVES NEED TO BE DELETED SOMEWHERE ELSE
 *
 *  @param[in]  auiPrintBuffer array buffer of wavedata.
 *              uiX_Start auiPrintBuffer index of first dataposition
 *              uiX_End auiPrintBuffer index of last dataposition
 *
 *              waveColor color of wave
 *                                                                           */
void vGraphicsPrintPartOfWave(uint16_t uiX_Start,uint16_t uiX_End,tuGraphicsColor waveColor)
{

    int16_t idY;
    uint16_t uixCounter;
    uint8_t uiBresenhamEnable;

    tsGraphicsCoord dataPointA,dataPointB;      //May will be removed


    for(uixCounter=uiX_Start;uixCounter<=uiX_End;uixCounter++)
    {

    uiBresenhamEnable=0;
    if(uixCounter!=0)
    {
        idY=auiPrintBuffer[uixCounter]-auiPrintBuffer[uixCounter-1];
        if(idY > 2 || idY < -2)                             //if bresenham is nececary
        {
            uiBresenhamEnable=1;
        }
    }

    dataPointA.uiX=uixCounter-1;
    dataPointA.uiY=auiPrintBuffer[uixCounter-1];    //preparing coordinates of datapoints
    dataPointB.uiX=uixCounter;
    dataPointB.uiY=auiPrintBuffer[uixCounter];


    if(uiBresenhamEnable)  //draw actual data point or print line between actual and next point. Depending of what is neccary to print a continous wave.
    {
        vGraphicsDrawLine(dataPointA,dataPointB,waveColor);
    }
    else
    {
        vDisplayWindowSet(uixCounter,uixCounter,DISPLAY_SIZE_Y-1-auiPrintBuffer[uixCounter],DISPLAY_SIZE_Y-1-auiPrintBuffer[uixCounter]);
        vDisplayStartPixelWrite();
        vGraphicsPixelWrite(waveColor);
    }

    }

}

/*
 *      prints a single datapoint of the wave
 */
void vGraphicsPrintWaveSingle(uint16_t uiX,tuGraphicsColor waveColor)
{
    vGraphicsPrintPartOfWave(uiX,uiX,waveColor);
}


/*
 *      Prints the whole wave
 */
void vGraphicsPrintWave(tuGraphicsColor waveColor)
{
    vGraphicsPrintPartOfWave(GRAPHICS_WAVEXSTART,GRAPHICS_WAVEXEND,waveColor);
}

/**
 *  @brief  Removes part of previous wave, prints part of next wave
 *
 *          blacks out previous wave from uiX_Start to uiX_End, refreshs the grid,
 *          load new samples in the auiPrintBuffer from uiX_Start to uiX_End and prints this part of the wave
 *
 *  @param[in]  p_aiSampleBuffer   Buffer of samples
 *                                                                           */
void vGraphicsUpdatePartOfWave(int16_t* p_aiSampleBuffer,uint16_t uiX_Start,uint16_t uiX_End,tuGraphicsColor waveColor)
{
    uint16_t uiBufferCounter;

    vGraphicsPrintPartOfWave(uiX_Start,uiX_End,GRAPHICS_BLACKCOLOR);
    vGraphicsPrintGrid();

    for(uiBufferCounter=uiX_Start;uiBufferCounter<=uiX_End;uiBufferCounter++)
    {
        if(p_aiSampleBuffer[uiBufferCounter]!=-1)        //change -1 to SAMPLER_SAMPLE_INVALID when connected to sampler module
        {
            auiPrintBuffer[uiBufferCounter]=p_aiSampleBuffer[uiBufferCounter]>>4;    //casting samples on a printable size
        }
        else
        {
            auiPrintBuffer[uiBufferCounter]=0;      //May this will be replaced by another way of handling measurement failures
        }
    }

    vGraphicsPrintPartOfWave(0,479,waveColor);
}

/**
 *  @brief  Removes single point of previous wave, prints single point of next wave
 *
 *          blacks out single point of previous wave, refreshs the grid,
 *          loads a single sample in the auiPrintBuffer and prints this point of the wave
 *
 *  @param[in]  p_aiSampleBuffer  Buffer of samples
 *              uiX position of single point
 *              waveColor color of the wave
 *
 *                                                                           */
void vGraphicsUpdateWaveSingle(int16_t* p_aiSampleBuffer,uint16_t uiX,tuGraphicsColor waveColor)
{
    vGraphicsUpdatePartOfWave(p_aiSampleBuffer,uiX,uiX,waveColor);
}

/**
 *  @brief  Removes previous wave, prints next wave
 *
 *          blacks out previous wave, refreshs the grid,
 *          load new samples in the auiPrintBuffer and prints the wave
 *
 *  @param[in]  p_aiSampleBuffer  Buffer of samples
 *
 *                                                                           */
void vGraphicsUpdateWave(int16_t* p_aiSampleBuffer,tuGraphicsColor waveColor)
{
    vGraphicsUpdatePartOfWave(p_aiSampleBuffer,GRAPHICS_WAVEXSTART,GRAPHICS_WAVEXEND,waveColor);
}



