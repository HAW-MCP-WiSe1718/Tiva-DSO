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

#define GRAPHICS_BLACKCOLOR  (tuGraphicsColor){.uiColorValue=0x00000000}
#define GRAPHICS_REDCOLOR    (tuGraphicsColor){.uiColorValue=0x0000FF00}
#define GRAPHICS_GREENCOLOR  (tuGraphicsColor){.uiColorValue=0x00FF0000}
#define GRAPHICS_BLUECOLOR   (tuGraphicsColor){.uiColorValue=0xFF000000}
#define GRAPHICS_YELLOWCOLOR (tuGraphicsColor){.uiColorValue=0x00FFFF00}
#define GRAPHICS_WHITECOLOR  (tuGraphicsColor){.uiColorValue=0xFFFFFF00}

#define GRAPHICS_GRIDCOLOR    (tuGraphicsColor){.uiColorValue=0x2a2a2a00}

void vGraphicsInit(void);

void vGraphicsPixelWrite(tuGraphicsColor);
void vGraphicsPrintRectangle(uint16_t,uint16_t,uint16_t,uint16_t,tuGraphicsColor);
void vGraphicsDrawLine(tsGraphicsCoord,tsGraphicsCoord,tuGraphicsColor);

#endif
