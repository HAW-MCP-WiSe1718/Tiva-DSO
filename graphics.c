#include "graphics.h"
#include "display.h"

static inline void vGraphicsPixelWrite(colorStruct color)
{
    vDisplayPixelWrite(color.redValue, color.greenValue, color.blueValue);
}

curserStruct curserA,curserB;

void graphicsInit()
{

vDisplayClear();    //blackout the display

curserA.curserColor.colorValue=ACURSERCOLOR;
curserB.curserColor.colorValue=BCURSERCOLOR;

printGrid();    //print the grid

}

//printRectangle prints a Rectangle

void printRectangle(uint16_t xStart,uint16_t xEnd,uint16_t yStart,uint16_t yEnd,color lineColor)
{
    uint16_t row,column;

    vDisplayWindowSet(xStart,xEnd,yStart,yEnd);
    vDisplayStartPixelWrite();

    for(row=yStart;row<=yEnd;row++)
    {
        for(column=xStart;column<=xEnd;column++)
        {
            vGraphicsPixelWrite(lineColor.colorSeperate);
        }
    }
}


//printLineBresenham prints a line between two points, including the second point, excluding the first point
void printLineBresenham(uint16_t xA,uint16_t yA,uint16_t xB,uint16_t yB,color lineColor)
{
uint16_t x,y=yA,dx=xB-xA,dy=yB-yA;
uint16_t delta= dy+dx;

for(x=(xA+1);x<=(xB);x++)
{
    delta-=dy;

    if(delta<=dy)
    {
        delta+=dx;
        y++;
    }

    vDisplayWindowSet(x,x,y,y);
    vDisplayStartPixelWrite();
    vGraphicsPixelWrite(lineColor.colorSeperate);
}

}

/* printWave prints the wave (data saved in sampleBuffer) from the least printed sample to the least measured sample.
 * It returns the bufferposition of the least printed sample.
 * After reaching the end of screen it cleans out the wave and repairs the grid.*/
uint16_t printWave(uint16_t* sampleBuffer,uint8_t* printBuffer,uint16_t nextPrintPosition,uint16_t lastSamplePosition,color waveColor)
{

while(nextPrintPosition!=lastSamplePosition)
{
color black;
black.colorValue=0x000000;
uint16_t clearCounter;
/*
    if(nextPrintPosition>0)
    {
        printLineBresenham(nextPrintPosition-1,sampleBuffer[nextPrintPosition-1],nextPrintPosition,sampleBuffer[nextPrintPosition],waveColor);
    }
    else
    {
        vDisplayWindowSet(0,0,sampleBuffer[0],sampleBuffer[0]);
        vDisplayStartPixelWrite();
        vGraphicsPixelWrite(waveColor.colorSeperate);
    }
*/
    printBuffer[nextPrintPosition]=(sampleBuffer[nextPrintPosition]>>4);

    vDisplayWindowSet(nextPrintPosition,nextPrintPosition,271-printBuffer[nextPrintPosition],271-printBuffer[nextPrintPosition]);
    vDisplayStartPixelWrite();
    vGraphicsPixelWrite(waveColor.colorSeperate);



    if(nextPrintPosition<480-1)
    {
        nextPrintPosition++;
    }
    else    //clears out the printed wavepoints and refresh the grid
    {
        for(nextPrintPosition=0;nextPrintPosition<=480-1;nextPrintPosition++)//clear wave
        {
            vDisplayWindowSet(nextPrintPosition,nextPrintPosition,271-printBuffer[nextPrintPosition],271-printBuffer[nextPrintPosition]);
            vDisplayStartPixelWrite();
            vGraphicsPixelWrite(black.colorSeperate);
        }
        nextPrintPosition=0;
        printGrid();
    }

}

return nextPrintPosition;
}

void printGrid()
{
color gridColor;
gridColor.colorSeperate=GRIDCOLORSTRUCT;

//printing vertical Lines
printRectangle(30,30,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(60,60,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(90,90,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(120,120,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(150,150,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(180,180,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(210,210,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(240,240,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(270,270,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(300,300,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(330,330,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(360,360,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(390,390,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(410,410,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(440,440,GRIDYSTART,GRIDYEND,gridColor);
printRectangle(470,470,GRIDYSTART,GRIDYEND,gridColor);

//Printing horizontal Lines
printRectangle(GRIDXSTART,GRIDXEND,30,30,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,60,60,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,90,90,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,90,90,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,120,120,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,150,150,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,180,180,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,210,210,gridColor);
printRectangle(GRIDXSTART,GRIDXEND,240,240,gridColor);
}


#if 0
/**
 *  @brief  Clear display (overwrite entire area with black pixels)
 *                                                                           */
void vDisplayClear(void)
{
    uint32_t ulCounter;
    colorStruct black={0,0,0,0};

    vDisplayWindowSet(0,480-1,0,272-1);
    vDisplayStartPixelWrite();
    for (ulCounter = 0; ulCounter < (480*272); ++ulCounter)
    {
        vGraphicsPixelWrite(black);
    }
}
#endif
