#include "text.h"

uint8_t aucTextGlyphLUT[TEXT_NUMBEROFCHARACTERS][TEXT_NUMBEROFGLYPHROWS]={TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,
                                                                          TEXT_BLANK,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,
                                                                          TEXT_0,TEXT_1,TEXT_2,TEXT_3,TEXT_4,TEXT_5,TEXT_6,TEXT_7,TEXT_8,TEXT_9,
                                                                          TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,TEXT_RESERVED,
                                                                          TEXT_A,TEXT_B,TEXT_C,TEXT_D,TEXT_E,TEXT_F,TEXT_G,TEXT_H,TEXT_I,TEXT_J,TEXT_K,TEXT_L,TEXT_M,TEXT_N,TEXT_O,TEXT_P,TEXT_Q,TEXT_R,TEXT_S,TEXT_T,TEXT_U,TEXT_V,TEXT_W,TEXT_X,TEXT_Y,TEXT_Z};       //Look up table for characters

tsTextCharacter aCharacterTextStatusMemory[7];       //Memory for statusline text (for testing)


/*
 *      Converts a letter to tsTextCharacter pixelData using the GlyphLUT
 *
 */
static void vTextConvertLetter(uint8_t* pucPixelData,uint8_t ucCharacter)
{
    uint16_t uiCounter;

    for(uiCounter=0;uiCounter<TEXT_NUMBEROFGLYPHROWS;uiCounter++)
    {
            pucPixelData[uiCounter]=aucTextGlyphLUT[ucCharacter-TEXT_ASCI_OFFSET][uiCounter];
    }
}


/*
 *      Initialise textmodule
 */

void vTextInit(void)
{

    vTextWriteTextIntoMemory("RUN ",aCharacterTextStatusMemory,TEXT_STATUS_COORD,TEXT_MEMORY_LOCATION_RUN,GRAPHICS_GREENCOLOR);    //for testing
    vTextWriteTextIntoMemory("STOP",aCharacterTextStatusMemory,TEXT_STATUS_COORD,TEXT_MEMORY_LOCATION_STOP,GRAPHICS_REDCOLOR);
}

/*
 *      converts a written text into tsTextCharacter Array and saves it to aCharacterMemory address.
 *
 *      string is the text that shall be converted
 *      characterCoord gives the text the user defined coordinate
 *      uiMemoryLocation defines the startaddress in the destination array
 *      textColor is the color of the text
 *
 *      Position of text is defined as the position of the pixel in the top left of the text
 */
void vTextWriteTextIntoMemory(uint8_t* string,tsTextCharacter* aCharacterMemory,tsGraphicsCoord characterCoord,uint16_t uiMemoryLocation,tuGraphicsColor textColor)
{
    uint16_t uiStringCounter=0;

    while(string[uiStringCounter]!='\0')     //fill tsTextCharacter Array with Information
    {
        vTextConvertLetter(aCharacterMemory[uiMemoryLocation+uiStringCounter].auiCharacterPixel,string[uiStringCounter]);
        aCharacterMemory[uiMemoryLocation+uiStringCounter].coordinate.uiX=characterCoord.uiX+6*uiStringCounter;
        aCharacterMemory[uiMemoryLocation+uiStringCounter].coordinate.uiY=characterCoord.uiY;
        aCharacterMemory[uiMemoryLocation+uiStringCounter].characterColor=textColor;

        uiStringCounter++;
    }
}


/**
 *  @brief  Prints a character of type tsTextCharacter
 *
 *                                                                           */
void vTextPrintCharacter(tsTextCharacter characterPrint)
{
    uint16_t uiRowCounter,uiColumnCounter;

    for(uiRowCounter=0;uiRowCounter<6;uiRowCounter++)
    {
        vDisplayWindowSet(characterPrint.coordinate.uiX,characterPrint.coordinate.uiX+4,characterPrint.coordinate.uiY+uiRowCounter,characterPrint.coordinate.uiY+uiRowCounter);// Preset windowsSet as if all pixels of this uiRow shall be transmitted
        vDisplayStartPixelWrite();

        for(uiColumnCounter=0;uiColumnCounter<5;uiColumnCounter++)
        {
            if((characterPrint.auiCharacterPixel[uiRowCounter] << uiColumnCounter) & 0x80)  //if pixel representating bit is 1, write pixel.
            {
                vGraphicsPixelWrite(characterPrint.characterColor);
            }
            else if(uiColumnCounter < 4)  //if not and it is not the last pixel of this row, set a new windowSet for the rest of the row. This procedure is saving transmit time. without this, windowset has to be executed for every pixel.
            {
                vDisplayWindowSet(characterPrint.coordinate.uiX+uiColumnCounter+1,characterPrint.coordinate.uiX+4,characterPrint.coordinate.uiY+uiRowCounter,characterPrint.coordinate.uiY+uiRowCounter);
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
void vTextPrintCharacterArray(tsTextCharacter* tsTextCharacterArray,uint16_t uiMemoryLocation,uint16_t length)
{
    uint16_t uiCounter;
    for(uiCounter=uiMemoryLocation;uiCounter<length;uiCounter++)
    {
        vTextPrintCharacter(tsTextCharacterArray[uiCounter]);
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

    vDisplayWindowSet(lineCoord.uiX,lineCoord.uiX+TEXT_NUMBEROFGLYPHCOLUMNS*length,lineCoord.uiY,lineCoord.uiY+TEXT_NUMBEROFGLYPHROWS);
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
    vTextEraseLine(TEXT_STATUS_COORD,TEXT_STATUS_LENGTH);
    vTextPrintCharacterArray(aCharacterTextStatusMemory,TEXT_MEMORY_LOCATION_RUN,3);
}

void vTextStatusSTOP()  //for testing
{
    vTextEraseLine(TEXT_STATUS_COORD,TEXT_STATUS_LENGTH);
    vTextPrintCharacterArray(aCharacterTextStatusMemory,TEXT_MEMORY_LOCATION_STOP,4);

}
