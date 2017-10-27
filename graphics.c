#include "graphics.h"
#include "display.h"

static inline void vGraphicsPixelWrite(color pixelColor)
{
    vDisplayPixelWrite(pixelColor.colorSeperate.redValue, pixelColor.colorSeperate.greenValue, pixelColor.colorSeperate.blueValue);
}


curserStruct curserA,curserB;

void graphicsInit()
{

vDisplayClear();    //blackout the display

curserA.curserColor=ACURSERCOLOR;
curserB.curserColor=BCURSERCOLOR;

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
            vGraphicsPixelWrite(lineColor);
        }
    }
}


//printLineBresenham prints a line between two points, including the second point, excluding the first point
void printLineBresenham(uint16_t xA,uint16_t yA,uint16_t xB,uint16_t yB,color lineColor)
{
uint16_t x=xA,y=yA,dX=xB-xA,dY;
int16_t delta,step=1;
uint8_t risingLine=1;

if(yB-yA > 0)   //decision if line shall be printed in negative or positive y-direction
{
    dY=yB-yA;
}
else
{
    dY=yA-yB;
    risingLine=0;
    step=-1;
}


delta=dY>>1;

while(y!=yB)
{
    delta-=dX;

    y+=step;

    if(delta<=0)
    {
        delta+=dY;
        x++;
    }

    vDisplayWindowSet(x,x,271-y,271-y);
    vDisplayStartPixelWrite();
    vGraphicsPixelWrite(lineColor);
}

}

/* printWave prints the wave (data saved in printBuffer) from xStart to xEnd. In cases where it is needed it uses the bresenHam algorithm to print lines.
 * PREVIOUS WAVES NEED TO BE DELETED MANUALY*/

void printWave(uint8_t* printBuffer,uint16_t xStart,uint16_t xEnd,color waveColor)
{

int16_t dY;
uint16_t xCounter;
uint8_t bresenhamEnable;

for(xCounter=xStart;xCounter<=xEnd;xCounter++)
{

bresenhamEnable=0;
if(xCounter!=0)
{
    dY=printBuffer[xCounter]-printBuffer[xCounter-1];
    if(dY > 2 || dY < -2)
    {
        bresenhamEnable=1;
    }
}

if(bresenhamEnable)
{
    printLineBresenham(xCounter-1,printBuffer[xCounter-1],xCounter,printBuffer[xCounter],waveColor);
}
else
{
    vDisplayWindowSet(xCounter,xCounter,271-printBuffer[xCounter],271-printBuffer[xCounter]);
    vDisplayStartPixelWrite();
    vGraphicsPixelWrite(waveColor);
}

}

}

/*blacks out previous wave from xStart to xEnd, refreshs the grid, load new samples in the printBuffer from xStart to xEnd and prints the whole wave */
void updateWave(uint16_t* sampleBuffer,uint8_t* printBuffer,uint16_t xStart,uint16_t xEnd,color waveColor)
{
    uint16_t bufferCounter;

    printWave(printBuffer,xStart,xEnd,BLACKCOLOR);
    printGrid();

    for(bufferCounter=xStart;bufferCounter<=xEnd;bufferCounter++)
    {
        printBuffer[bufferCounter]=sampleBuffer[bufferCounter]>>4;
    }

    printWave(printBuffer,0,479,waveColor);
}


void printGrid(void)
{

uint8_t lineCounter;

//printing vertical lines
for(lineCounter=1;lineCounter<(GRIDXEND+1-GRIDXSTART)/GRIDXSPACE;lineCounter++)
{
    printRectangle(GRIDXSTART+lineCounter*GRIDXSPACE,GRIDXSTART+lineCounter*GRIDXSPACE,GRIDYSTART,GRIDYEND,GRIDCOLOR);
}

//printing horizontal lines
for(lineCounter=1;lineCounter<(GRIDYEND+1-GRIDYSTART)/GRIDYSPACE;lineCounter++)
{
    printRectangle(GRIDXSTART,GRIDXEND,GRIDYSTART+lineCounter*GRIDYSPACE,GRIDYSTART+lineCounter*GRIDYSPACE,GRIDCOLOR);
}



}

