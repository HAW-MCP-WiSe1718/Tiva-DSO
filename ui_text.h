#ifndef UI_TEXT_H_
#define UI_TEXT_H_

#include "graphics.h"

#define TEXT_NUMBEROFCHARACTERS 70
#define TEXT_NUMBEROFGLYPHROWS 8
#define TEXT_NUMBEROFGLYPHCOLUMNS 5

#define TEXT_ASCI_OFFSET ' '

#define TEXT_STATUS_COORD (tsGraphicsCoord){1,1}
#define TEXT_TRIGSRC_COORD (tsGraphicsCoord){60,1}
#define TEXT_TRIGEDGE_COORD (tsGraphicsCoord){150,1}
#define TEXT_TIMEBASE_COORD (tsGraphicsCoord){420,1}

#define TEXT_RESERVED {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}

#define TEXT_BLANK {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#define TEXT_EQUAL {0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00}
#define TEXT_DOT {0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60}
#define TEXT_PLUS {0x00,0x00,0x20,0x20,0xF8,0x20,0x20,0x00}
#define TEXT_COMMA {0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x40}
#define TEXT_MINUS {0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00}
#define TEXT_COLON {0x00,0x60,0x60,0x00,0x00,0x60,0x60,0x00}
#define TEXT_SLASH {0x18,0x10,0x30,0x20,0x60,0x40,0xC0,0x80}
#define TEXT_RISINGEDGE {0x18,0x10,0x30,0x20,0x60,0x40,0xC0,0x80}
#define TEXT_FALLINGEDGE {0x18,0x10,0x30,0x20,0x60,0x40,0xC0,0x80}
#define TEXT_DELTA {0x20,0x20,0x50,0x50,0x50,0x88,0x88,0xF8}
#define TEXT_0 {0x70,0xD8,0x88,0xA8,0xA8,0x88,0xD8,0x70}
#define TEXT_1 {0x18,0x38,0x78,0xD8,0x98,0x18,0x18,0x18}
#define TEXT_2 {0x70,0xFF,0xD8,0x18,0x30,0x60,0xC0,0xFF}
#define TEXT_3 {0x70,0xFF,0x98,0x30,0x30,0x98,0xFF,0x70}
#define TEXT_4 {0x38,0x78,0xD8,0x98,0xFF,0xFF,0x18,0x18}
#define TEXT_5 {0xFF,0x80,0x80,0xFF,0xFF,0x08,0x18,0xFF}
#define TEXT_6 {0x78,0x80,0x80,0xF0,0xFF,0x88,0x88,0x70}
#define TEXT_7 {0xFF,0xFF,0x18,0x38,0x70,0xE0,0xC0,0xC0}
#define TEXT_8 {0x70,0x88,0x88,0xFF,0xFF,0x88,0x88,0x70}
#define TEXT_9 {0x70,0x88,0x88,0xFF,0x08,0x08,0x88,0x70}
#define TEXT_A {0x20,0x70,0xD8,0x88,0xF8,0x88,0x88,0x88}
#define TEXT_B {0xF0,0x88,0x88,0xF0,0xF0,0x88,0x88,0xF0}
#define TEXT_C {0x38,0x40,0x80,0x80,0x80,0x80,0x40,0x38}
#define TEXT_D {0xE0,0x90,0x88,0x88,0x88,0x88,0x90,0xE0}
#define TEXT_E {0xF8,0x80,0x80,0xF0,0xF0,0x80,0x80,0xF8}
#define TEXT_F {0xF8,0x80,0x80,0xF8,0x80,0x80,0x80,0x80}
#define TEXT_G {0x38,0x40,0x80,0x80,0xB8,0x88,0x88,0x70}
#define TEXT_H {0x88,0x88,0x88,0xF8,0xF8,0x88,0x88,0x88}
#define TEXT_I {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20}
#define TEXT_J {0xF8,0x08,0x08,0x08,0x08,0x08,0x88,0x70}
#define TEXT_K {0x88,0x98,0xB0,0xE0,0xE0,0xB0,0x98,0x88}
#define TEXT_L {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xF8}
#define TEXT_M {0x88,0xD8,0xF8,0xA8,0x88,0x88,0x88,0x88}
#define TEXT_N {0x88,0xC8,0xC8,0xE8,0xB8,0x98,0x98,0x88}
#define TEXT_O {0xF8,0x88,0x88,0x88,0x88,0x88,0x88,0xF8}
#define TEXT_P {0xF8,0x88,0x88,0xF0,0x80,0x80,0x80,0x80}
#define TEXT_Q {0x70,0x88,0x88,0x88,0x88,0xA8,0x98,0x78}
#define TEXT_R {0xF0,0x88,0x88,0xF0,0xC0,0xA0,0x90,0x88}
#define TEXT_S {0x78,0x80,0x80,0xD0,0x38,0x08,0x08,0xF0}
#define TEXT_T {0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x20}
#define TEXT_U {0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x70}
#define TEXT_V {0x88,0x88,0x88,0xD8,0x50,0x70,0x20,0x20}
#define TEXT_W {0x88,0x88,0x88,0x88,0xC8,0xF8,0xD8,0x88}
#define TEXT_X {0x88,0x88,0x50,0x20,0x20,0x50,0x88,0x88}
#define TEXT_Y {0x88,0x88,0xD8,0x50,0x50,0x20,0x20,0x20}
#define TEXT_Z {0xF8,0x18,0x80,0x30,0x60,0x40,0xC0,0xF8}


/*position is defined as the top left pixel of the character*/
typedef struct{
    tsGraphicsCoord coordinate;
    tuGraphicsColor characterColor;
    uint8_t auiCharacterPixel[TEXT_NUMBEROFGLYPHROWS];
} tsTextCharacter;

void vTextPrintCharacter(uint8_t,tsGraphicsCoord,tuGraphicsColor);
void vTextPrintString(uint8_t*,tsGraphicsCoord,tuGraphicsColor);
void vTextEraseLine(tsGraphicsCoord,uint16_t);

void vTextInit(void);

#endif /* UI_TEXT_H_ */
