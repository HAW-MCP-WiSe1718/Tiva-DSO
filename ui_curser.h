#include "graphics.h"

#ifndef UI_CURSER_H_
#define UI_CURSER_H_

#define UI_CURSER_BOTTOM 271
#define UI_CURSER_TOP 16

#define UI_CURSER_VOLTAGE_LENGTH (7+1) //+1 for \0

#define UI_CURSER_TIME_LENGTH (7+1) //+1 for \0

#define UI_CURSER_DELTA_VOLTAGE_LENGTH (8+1) //+1 for \0

#define UI_CURSER_DELTA_TIME_LENGTH (8+1) //+1 for \0

#define UI_CURSER_BOUNDARY_THICKNES 3
#define UI_CURSER_BOUNDARY_LENGTH 5

#define UI_CURSER_VOLTAGE_X_OFFSET 3
#define UI_CURSER_VOLTAGE_BOUNDARYOVERLAP_X_OFFSET -25
#define UI_CURSER_A_VOLTAGE_Y_OFFSET 10
#define UI_CURSER_B_VOLTAGE_Y_OFFSET 230

#define UI_CURSER_TIME_X_OFFSET 3
#define UI_CURSER_TIME_BOUNDARYOVERLAP_X_OFFSET -25
#define UI_CURSER_A_TIME_Y_OFFSET (UI_CURSER_A_VOLTAGE_Y_OFFSET+10)
#define UI_CURSER_B_TIME_Y_OFFSET (UI_CURSER_B_VOLTAGE_Y_OFFSET+10)

#define UI_CURSER_DELTA_VOLTAGE_X_POSITION 180
#define UI_CURSER_DELTA_VOLTAGE_Y_POSITION 0

#define UI_CURSER_DELTA_TIME_X_POSITION (UI_CURSER_DELTA_VOLTAGE_X_POSITION+75)
#define UI_CURSER_DELTA_TIME_Y_POSITION 0

#define UI_CURSER_REF_VOLTAGE 330

#define UI_CURSER_INTEGER_DIVISION_MULTIPLIER 100
//definitions of CurserStateNumbers: E for ucCurserEnabled, V for ucCurserVisible, PS for ucpositionStatic
#define UI_CURSER_STATE_E0_V1_PS0 0x00010000
#define UI_CURSER_STATE_E0_V1_PS1 0x00010100
#define UI_CURSER_STATE_E1_V0_PS0 0x01000000
#define UI_CURSER_STATE_E1_V0_PS1 0x01000100
#define UI_CURSER_STATE_E1_V1_PS0 0x01010000
#define UI_CURSER_STATE_E1_V1_PS1 0x01010100

#define UI_CURSER_A_COLOR (tuGraphicsColor){.uiColorValue=0x00FFFF00}
#define UI_CURSER_B_COLOR (tuGraphicsColor){.uiColorValue=0XFFFF0000}
#define UI_CURSER_TEXT_DELTA_COLOR (tuGraphicsColor){.uiColorValue=0XFFEFEF00}

#define UI_CURSER_A_INIT_POSITION 120
#define UI_CURSER_B_INIT_POSITION 360

typedef struct{
    uint8_t reserved;
    uint8_t ucpositionStatic;
    uint8_t ucCurserVisible;
    uint8_t ucCurserEnabled;
}tsCurserState;

typedef union{
    tsCurserState stateSeperate;
    uint32_t stateNumber;
}tuCurserState;

//struct for a single curser
typedef struct{
    uint16_t uiActualxPosition;
    uint16_t uiNextxPosition;

    tuGraphicsColor curserColor;

    uint8_t aucVoltageString[UI_CURSER_VOLTAGE_LENGTH];
    tsGraphicsCoord voltageStringCoord;
    uint8_t aucTimeString[UI_CURSER_TIME_LENGTH];
    tsGraphicsCoord timeStringCoord;

}tsCurserStruct;

//struct for a couple of cursers
typedef struct{

    tsCurserStruct curserA;
    tsCurserStruct curserB;

    //Information of both Cursers
    tuCurserState state;

    uint16_t uiTimeBaseMiliPerDiv;

    uint8_t aucDeltaVoltageString[UI_CURSER_DELTA_VOLTAGE_LENGTH];
    tsGraphicsCoord deltaVoltageStringCoord;
    uint8_t aucDeltaTimeString[UI_CURSER_DELTA_TIME_LENGTH];
    tsGraphicsCoord deltaTimeStringCoord;

    tuGraphicsColor curserDeltaColor;

}tsCurserCoupleStruct;

tsCurserCoupleStruct cursers;

void vUICurserInit(void);

void vUICurserPrintLines(uint16_t,tuGraphicsColor);

void vUICurserPrint(tsCurserStruct*);
void vUICurserErase(tsCurserStruct*);
void vUICurserRefresh(void);

void vUICurserUpdate(void);


#endif /* UI_CURSER_H_ */
