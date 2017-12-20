#include "ui_waveform.h"
#include "sampler.h"
#include "ui_curser.h"

/**
 *  @brief  fills the printbuffer with new samples
 *
 *
 *                                                                           */
void static vUIWaveformUpdatePrintBuffer(int16_t* p_aiSampleBuffer,uint16_t uiX_Start,uint16_t uiX_End)
{
    uint16_t uiBufferCounter=uiX_Start;

    while(uiBufferCounter <= uiX_End)
    {
        if(p_aiSampleBuffer[uiBufferCounter]!=SAMPLER_SAMPLE_INVALID)
        {
            aiPrintBuffer[uiBufferCounter]=(p_aiSampleBuffer[uiBufferCounter]>>4)&0xFF;    //casting samples on a printable size
        }
        else
        {
            aiPrintBuffer[uiBufferCounter]=-1;
        }

        uiBufferCounter++;
    }
}

/**
 *  @brief  prints the DSO grid
 *
 *          using vGraphicsPrintRectAngle()
 *
 *          GRID CAN BE MODIFIED BY CHANGING THE GRIDMAKROS
 *
 *                                                                           */
void vUIWaveformGridInit(void)
{

    uint8_t uiLineCounter;

    //printing vertical lines
    for(uiLineCounter=1;uiLineCounter<(UI_WAVEFORM_GRIDXEND+1-UI_WAVEFORM_GRIDXSTART)/UI_WAVEFORM_GRIDXSPACE;uiLineCounter++)
    {
        vGraphicsPrintRectangle(UI_WAVEFORM_GRIDXSTART+uiLineCounter*UI_WAVEFORM_GRIDXSPACE,UI_WAVEFORM_GRIDXSTART+uiLineCounter*UI_WAVEFORM_GRIDXSPACE,UI_WAVEFORM_GRIDYSTART,UI_WAVEFORM_GRIDYEND,GRAPHICS_GRIDCOLOR);
    }

    //printing horizontal lines
    for(uiLineCounter=1;uiLineCounter<(UI_WAVEFORM_GRIDYEND+1-UI_WAVEFORM_GRIDYSTART)/UI_WAVEFORM_GRIDYSPACE;uiLineCounter++)
    {
        vGraphicsPrintRectangle(UI_WAVEFORM_GRIDXSTART,UI_WAVEFORM_GRIDXEND,UI_WAVEFORM_GRIDYSTART+uiLineCounter*UI_WAVEFORM_GRIDYSPACE,UI_WAVEFORM_GRIDYSTART+uiLineCounter*UI_WAVEFORM_GRIDYSPACE,GRAPHICS_GRIDCOLOR);
    }

}


/*
 *
 *
 */
void vUIWaveformInit(void)
{
    uint16_t uiCounter;

    vGraphicsPrintRectangle(UI_WAVEFORM_GRIDXSTART,UI_WAVEFORM_GRIDXEND,UI_WAVEFORM_GRIDYSTART-UI_WAVEFORM_GRID_TOPBOUNDARY_THICKNESS,UI_WAVEFORM_GRIDYSTART-1,GRAPHICS_GRIDCOLOR);

    vUIWaveformGridInit();    //print the grid

    for(uiCounter=0;uiCounter<UI_WAVEFORM_PRINTBUFFER_SIZE;uiCounter++)
    {
        aiPrintBuffer[uiCounter]=0;
    }
}


/**
 *  @brief  Kurzbeschreibung
 *
 *          vGraphicsPrintWave prints the wave (data saved in aiPrintBuffer) from uiX_Start to uiX_End.
 *          In cases where it is needed it uses the bresenHam algorithm to print lines.
 *          PREVIOUS WAVES NEED TO BE DELETED SOMEWHERE ELSE
 *
 *  @param[in]  aiPrintBuffer array buffer of wavedata.
 *              uiX_Start aiPrintBuffer index of first dataposition
 *              uiX_End aiPrintBuffer index of last dataposition
 *
 *              waveColor color of wave
 *                                                                           */
void vUIWaveformPrintPartOfWave(uint16_t uiX_Start,uint16_t uiX_End,tuGraphicsColor waveColor)
{

    int16_t idY;
    uint16_t uixCounter=0;
    uint8_t uiBresenhamEnable;

    tsGraphicsCoord dataPointA,dataPointB;      //May will be removed


    while(aiPrintBuffer[uixCounter]!=-1 && uixCounter <= uiX_End)   //printing until first missing point
    {

        uiBresenhamEnable=0;
        if(uixCounter!=0)
        {
            idY=aiPrintBuffer[uixCounter]-aiPrintBuffer[uixCounter-1];
            if(idY > 1 || idY < -1)                             //if bresenham is nececary
            {
                uiBresenhamEnable=1;
            }
        }

        dataPointA.uiX=uixCounter-1;
        dataPointA.uiY=aiPrintBuffer[uixCounter-1];    //preparing coordinates of datapoints
        dataPointB.uiX=uixCounter;
        dataPointB.uiY=aiPrintBuffer[uixCounter];


        if(uiBresenhamEnable)  //draw actual data point or print line between actual and next point. Depending of what is neccary to print a continous wave.
        {
            vGraphicsDrawLine(dataPointA,dataPointB,waveColor);
        }
        else
        {
            vDisplayWindowSet(uixCounter,uixCounter,DISPLAY_SIZE_Y-1-aiPrintBuffer[uixCounter],DISPLAY_SIZE_Y-1-aiPrintBuffer[uixCounter]);
            vDisplayStartPixelWrite();
            vGraphicsPixelWrite(waveColor);
        }

        uixCounter++;   //increment loop counter

    }

}

/*
 *      prints a single datapoint of the wave
 */
void vUIWaveformPrintWaveSingle(uint16_t uiX,tuGraphicsColor waveColor)
{
    vUIWaveformPrintPartOfWave(uiX,uiX,waveColor);
}


/*
 *      Prints the whole wave
 */
void vUIWaveformPrintWave(tuGraphicsColor waveColor)
{
    vUIWaveformPrintPartOfWave(UI_WAVEFORM_WAVEXSTART,UI_WAVEFORM_WAVEXEND,waveColor);
}

/**
 *  @brief  Removes part of previous wave, prints part of next wave
 *
 *          blacks out previous wave from uiX_Start to uiX_End, refreshs the grid,
 *          load new samples in the aiPrintBuffer from uiX_Start to uiX_End and prints this part of the wave.
 *          uiX_End maximum value is 480
 *
 *  @param[in]  p_aiSampleBuffer   Buffer of samples
 *                                                                           */
void vUIWaveformUpdatePartOfWave(int16_t* p_aiSampleBuffer,uint16_t uiX_Start,uint16_t uiX_End)
{
    //Erase previous part wave
    vUIWaveformPrintPartOfWave(uiX_Start,uiX_End,GRAPHICS_BLACKCOLOR);

    //Refresh Grid
    vUIWaveformGridInit();

    //Update printBuffer
    vUIWaveformUpdatePrintBuffer(p_aiSampleBuffer,uiX_Start,uiX_End);

    //print new part of wave
    vUIWaveformPrintPartOfWave(0,479,UI_WAVEFORM_WAVE_COLOR);

    //Refresh the cursers
    vUICurserRefresh();

}

/**
 *  @brief  Removes single point of previous wave, prints single point of next wave
 *
 *          blacks out single point of previous wave, refreshs the grid,
 *          loads a single sample in the aiPrintBuffer and prints this point of the wave
 *
 *  @param[in]  p_aiSampleBuffer  Buffer of samples
 *              uiX position of single point
 *              waveColor color of the wave
 *
 *                                                                           */
void vUIWaveformUpdateWaveSingle(int16_t* p_aiSampleBuffer,uint16_t uiX)
{
    vUIWaveformUpdatePartOfWave(p_aiSampleBuffer,uiX,uiX);
}

/**
 *  @brief  Removes previous wave, prints next wave
 *
 *          blacks out previous wave, refreshs the grid,
 *          load new samples in the aiPrintBuffer and prints the wave
 *
 *  @param[in]  p_aiSampleBuffer  Buffer of samples
 *
 *                                                                           */
void vUIWaveformUpdateWave(int16_t* p_aiSampleBuffer)
{
    vUIWaveformUpdatePartOfWave(p_aiSampleBuffer,UI_WAVEFORM_WAVEXSTART,UI_WAVEFORM_WAVEXEND);
}



