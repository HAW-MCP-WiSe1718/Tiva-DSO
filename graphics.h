#include "display.h"

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef struct{
    uint16_t uiX;
    uint16_t uiY;
}tsGraphicsCoord;

typedef struct{
    uint8_t reserved;

    uint8_t uiRedValue;
    uint8_t uiGreenValue;
    uint8_t uiBlueValue;
}tsGraphicsColor;

typedef union{
    uint32_t uiColorValue;
    tsGraphicsColor colorSeperate;
}tuGraphicsColor;

#define GRAPHICS_GRIDXSTART 0
#define GRAPHICS_GRIDXEND   480-1
#define GRAPHICS_GRIDYSTART 16
#define GRAPHICS_GRIDYEND   272-1
#define GRAPHICS_GRIDXSPACE 40
#define GRAPHICS_GRIDYSPACE 32

#define GRAPHICS_WAVEXSTART 1-1
#define GRAPHICS_WAVEXEND 480-1

#define GRAPHICS_PRINTBUFFER_SIZE 480

#define GRAPHICS_BLACKCOLOR  (tuGraphicsColor){.uiColorValue=0x000000}
#define GRAPHICS_REDCOLOR    (tuGraphicsColor){.uiColorValue=0x0000FF}
#define GRAPHICS_GREENCOLOR  (tuGraphicsColor){.uiColorValue=0x00FF00}
#define GRAPHICS_BLUECOLOR   (tuGraphicsColor){.uiColorValue=0xFF0000}

#define GRAPHICS_GRIDCOLOR    (tuGraphicsColor){.uiColorValue=0x808080}
#define GRAPHICS_WAVECOLOR    (tuGraphicsColor){.uiColorValue=0xFFFF00}
#define GRAPHICS_ACURSERCOLOR (tuGraphicsColor){.uiColorValue=0x0000FF}
#define GRAPHICS_BCURSERCOLOR (tuGraphicsColor){.uiColorValue=0X00FF00}


void vGraphicsInit(void);

void vGraphicsPixelWrite(tuGraphicsColor);
void vGraphicsPrintRectangle(uint16_t,uint16_t,uint16_t,uint16_t,tuGraphicsColor);
void vGraphicsDrawLine(tsGraphicsCoord,tsGraphicsCoord,tuGraphicsColor);

void vGraphicsPrintWave(tuGraphicsColor);
void vGraphicsPrintWaveSingle(uint16_t,tuGraphicsColor);
void vGraphicsPrintPartOfWave(uint16_t,uint16_t,tuGraphicsColor);

void vGraphicsUpdateWave(int16_t*,tuGraphicsColor);
void vGraphicsUpdateWaveSingle(int16_t*,uint16_t,tuGraphicsColor);
void vGraphicsUpdatePartOfWave(int16_t*,uint16_t,uint16_t,tuGraphicsColor);

#endif
