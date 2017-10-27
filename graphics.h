#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>
#include "text.h"

#define CURSERBOTTOM 0
#define CURSERTOP 271

#define GRIDXSTART 0
#define GRIDXEND   480-1
#define GRIDYSTART 16
#define GRIDYEND   272-1
#define GRIDXSPACE 40
#define GRIDYSPACE 32

#define GRIDCOLOR (color){.colorValue=0xF0F0F0}
#define WAVECOLOR (color){.colorValue=0xFFFF00}
#define ACURSERCOLOR (color){.colorValue=0x0000FF}
#define BCURSERCOLOR (color){.colorValue=0X00FF00}

#define BLACKCOLOR (color){.colorValue=0x000000}


void graphicsInit(void);

void printRectangle(uint16_t,uint16_t,uint16_t,uint16_t,color);
void printLineBresenham(uint16_t,uint16_t,uint16_t,uint16_t,color);
void printWave(uint8_t*,uint16_t,uint16_t,color);
void printGrid(void);
void updateWave(uint16_t*,uint8_t*,uint16_t,uint16_t,color);

#endif
