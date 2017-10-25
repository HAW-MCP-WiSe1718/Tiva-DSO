#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>

#define CURSERBOTTOM 0
#define CURSERTOP 272

#define GRIDXSTART  0
#define GRIDXEND    480-1
#define GRIDYSTART  0
#define GRIDYEND    272-1

#define GRIDCOLOR 0xF0F0F0
#define WAVECOLOR 0xFFFF00
#define ACURSERCOLOR 0x0000FF
#define BCURSERCOLOR 0X00FF00


#define GRIDCOLORSTRUCT ((colorStruct){0,0xC0,0xC0,0xC0})


typedef struct{
    uint8_t reserved;

    uint8_t redValue;
    uint8_t greenValue;
    uint8_t blueValue;
}colorStruct;

typedef union{
    uint32_t colorValue;
    colorStruct colorSeperate;
}color;

typedef struct{
    uint16_t xPosition;
    color curserColor;
    uint8_t curserVisible;
}curserStruct;

void graphicsInit();

void printRectangle(uint16_t,uint16_t,uint16_t,uint16_t,color);
void printLineBresenham(uint16_t,uint16_t,uint16_t,uint16_t,color);
uint16_t printWave(uint16_t*,uint8_t*,uint16_t,uint16_t,color);
void printGrid();

#if 0
void vDisplayClear(void);
#endif

#endif
