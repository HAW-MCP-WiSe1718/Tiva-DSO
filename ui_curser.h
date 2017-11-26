#include "graphics.h"

#ifndef UI_CURSER_H_
#define UI_CURSER_H_

#define UI_CURSER_BOTTOM 271
#define UI_CURSER_TOP 16
#define UI_CURSER_VOLTAGE_LENGTH (8+1) //+1 for \0
#define UI_CURSER_TEXT_LENGTH 7

#define UI_CURSER_BOUNDARY_THICKNES 3
#define UI_CURSER_BOUNDARY_LENGTH 5

#define UI_CURSER_TEXT_X_OFFSET 25
#define UI_CURSER_A_TEXT_Y_OFFSET 10
#define UI_CURSER_B_TEXT_Y_OFFSET 230

#define UI_CURSER_REF_VOLTAGE 330

#define UI_CURSER_A_COLOR (tuGraphicsColor){.uiColorValue=0x00FFFF00}
#define UI_CURSER_B_COLOR (tuGraphicsColor){.uiColorValue=0XFFFF0000}

#define UI_CURSER_A_INIT_POSITION 120
#define UI_CURSER_B_INIT_POSITION 360


typedef struct{
    uint16_t uiActualxPosition;
    uint16_t uiNextxPosition;

    tuGraphicsColor curserColor;
    uint8_t ucCurserEnabled;
    uint8_t ucCurserVisible;

    uint8_t aucVoltageValue[UI_CURSER_VOLTAGE_LENGTH];
    tsGraphicsCoord voltageCoord;

}tsCurserStruct;

tsCurserStruct curserA,curserB;

void vUICurserInit(void);

void vUICurserPrint(uint16_t,tuGraphicsColor);
void vUICurserUpdate(void);


#endif /* UI_CURSER_H_ */
