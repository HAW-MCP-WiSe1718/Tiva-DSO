#include "ui_text.h"


//deklaration and initialisation of Glyph-LUT
//-------------------------------------------------------------------------------
uint8_t aucTextGlyphLUT[TEXT_NUMBEROFCHARACTERS][TEXT_NUMBEROFGLYPHROWS]=

    {TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,
     TEXT_BLANK,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_DOT,TEXT_RESERVED,
     TEXT_0,TEXT_1,TEXT_2,TEXT_3,TEXT_4,TEXT_5,TEXT_6,TEXT_7,TEXT_8,TEXT_9,
     TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_EQUAL,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,
     TEXT_A,TEXT_B,TEXT_C,TEXT_D,TEXT_E,TEXT_F,TEXT_G,TEXT_H,TEXT_I,TEXT_J,TEXT_K,TEXT_L,TEXT_M,TEXT_N,TEXT_O,TEXT_P,TEXT_Q,TEXT_R,TEXT_S,TEXT_T,TEXT_U,TEXT_V,TEXT_W,TEXT_X,TEXT_Y,TEXT_Z};       //Look up table for characters
//--------------------------------------------------------------------------------


/*
 *      Initialise textmodule
 */
void vTextInit(void)
{
}


/**
 *  @brief  Prints a character of type tsTextCharacter
 *
 *                                                                           */
void vTextPrintCharacter(uint8_t ucCharacter,tsGraphicsCoord position,tuGraphicsColor color)
{
    uint16_t uiRowCounter,uiColumnCounter;

    for(uiRowCounter=0;uiRowCounter<TEXT_NUMBEROFGLYPHROWS;uiRowCounter++)
    {
        vDisplayWindowSet(position.uiX,position.uiX+TEXT_NUMBEROFGLYPHCOLUMNS,position.uiY+uiRowCounter,position.uiY+uiRowCounter);// Preset windowsSet as if all pixels of this uiRow shall be transmitted
        vDisplayStartPixelWrite();

        for(uiColumnCounter=0;uiColumnCounter<TEXT_NUMBEROFGLYPHCOLUMNS;uiColumnCounter++)
        {
            if((aucTextGlyphLUT[ucCharacter-TEXT_ASCI_OFFSET][uiRowCounter] << uiColumnCounter) & 0x80)  //if pixel representating bit is 1, write pixel.
            {
                vGraphicsPixelWrite(color);
            }
            else if(uiColumnCounter < TEXT_NUMBEROFGLYPHCOLUMNS-1)  //if not and it is not the last pixel of this row, set a new windowSet for the rest of the row. This procedure is saving transmit time. without this, windowset has to be executed for every pixel.
            {
                vDisplayWindowSet(position.uiX+uiColumnCounter+1,position.uiX+4,position.uiY+uiRowCounter,position.uiY+uiRowCounter);
                vDisplayStartPixelWrite();
            }
        }
    }
}

/**
 *  @brief  Prints an Array of tsTextCharacter
 *
 *          using vTextPrintCharacter
 *
 *                                    */
void vTextPrintString(uint8_t* auiString,tsGraphicsCoord position,tuGraphicsColor color)
{
    uint16_t uiCounter=0;
    while(auiString[uiCounter]!='\0')
    {
        position.uiX=position.uiX+(uiCounter*(TEXT_NUMBEROFGLYPHCOLUMNS+1));       //+1 for blank column between characters
        vTextPrintCharacter(auiString[uiCounter],position,color);
        uiCounter++;
    }
}


/*
 *
 *      Erasing a field of text
 *
 */
void vTextEraseLine(tsGraphicsCoord lineCoord, uint16_t length)
{
    uint16_t uiRowCounter,uiColumnCounter;

    vDisplayWindowSet(lineCoord.uiX,lineCoord.uiX+(TEXT_NUMBEROFGLYPHCOLUMNS+1)*length,lineCoord.uiY,lineCoord.uiY+TEXT_NUMBEROFGLYPHROWS);     //+1 for blank column between characters
    vDisplayStartPixelWrite();

    for(uiRowCounter=0;uiRowCounter<TEXT_NUMBEROFGLYPHROWS;uiRowCounter++)
    {
        for(uiColumnCounter=0;uiColumnCounter<length*TEXT_NUMBEROFGLYPHCOLUMNS;uiColumnCounter++)           //blacks out every pixel in selected textfield
        {
            vGraphicsPixelWrite(GRAPHICS_BLACKCOLOR);
        }
    }
}

void vTextStatusRUN()   //for testing
{
    vTextEraseLine(TEXT_STATUS_COORD,4);
    vTextPrintString("RUN",TEXT_STATUS_COORD,GRAPHICS_GREENCOLOR);
}

void vTextStatusSTOP()  //for testing
{
    vTextEraseLine(TEXT_STATUS_COORD,4);
    vTextPrintString("STOP",TEXT_STATUS_COORD,GRAPHICS_REDCOLOR);

}
