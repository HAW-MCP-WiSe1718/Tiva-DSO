#include "ui_curser.h"
#include "ui_waveform.h"
#include "ui_text.h"


/*
 *  Calculate the voltage Value Text of Cursers
 *
 */
void static inline vUICurserCalc(tsCurserStruct* curser)
{
    uint16_t uiValue=330*aiPrintBuffer[(*curser).uiActualxPosition]/0xFF;     // factor 100

    if(aiPrintBuffer[(*curser).uiActualxPosition] > -1)
    {
        (*curser).aucVoltageString[2]='0' + uiValue/100;
        (*curser).aucVoltageString[4]='0' + (uiValue%100)/10;
        (*curser).aucVoltageString[5]='0' + (uiValue%10);
    }
    else
    {
        (*curser).aucVoltageString[2]='X';
        (*curser).aucVoltageString[4]='X';
        (*curser).aucVoltageString[5]='X';
    }

    if((*curser).uiActualxPosition-UI_CURSER_TEXT_X_OFFSET <= UI_WAVEFORM_GRIDXSTART)
    {
        (*curser).voltageStringCoord.uiX=(*curser).uiActualxPosition-UI_CURSER_TEXT_X_OFFSET;
    }
    else
    {
        (*curser).voltageStringCoord.uiX=(*curser).uiActualxPosition+3;
    }

}

/*
 *  Initialise Curser structures with constant and startvalues
 *
 */
void vUICurserInit(void)
{
    curserA.curserColor=UI_CURSER_A_COLOR;
    curserB.curserColor=UI_CURSER_B_COLOR;

    curserA.aucVoltageString[0]='U';
    curserA.aucVoltageString[1]='=';
    curserA.aucVoltageString[2]='X';
    curserA.aucVoltageString[3]='.';
    curserA.aucVoltageString[4]='X';
    curserA.aucVoltageString[5]='X';
    curserA.aucVoltageString[6]='V';
    curserA.aucVoltageString[7]='\0';

    curserB.aucVoltageString[0]='U';
    curserB.aucVoltageString[1]='=';
    curserB.aucVoltageString[2]='X';
    curserB.aucVoltageString[3]='.';
    curserB.aucVoltageString[4]='X';
    curserB.aucVoltageString[5]='X';
    curserB.aucVoltageString[6]='V';
    curserB.aucVoltageString[7]='\0';

    curserA.uiActualxPosition=UI_CURSER_A_INIT_POSITION;
    curserA.uiNextxPosition=UI_CURSER_A_INIT_POSITION;

    curserB.uiActualxPosition=UI_CURSER_B_INIT_POSITION;
    curserB.uiNextxPosition=UI_CURSER_B_INIT_POSITION;

    curserA.voltageStringCoord.uiX=curserA.uiActualxPosition-UI_CURSER_TEXT_X_OFFSET;
    curserA.voltageStringCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_A_TEXT_Y_OFFSET;

    curserB.voltageStringCoord.uiX=curserB.uiActualxPosition-UI_CURSER_TEXT_X_OFFSET;
    curserB.voltageStringCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_B_TEXT_Y_OFFSET;

    curserA.state.stateSeperate.ucCurserEnabled=0;
    curserA.state.stateSeperate.ucCurserVisible=0;

    curserB.state.stateSeperate.ucCurserEnabled=0;
    curserB.state.stateSeperate.ucCurserVisible=0;
}


/*
 * Printing Curserlines using vGraphicsPrintRectangle();
 *
 */
void inline vUICurserPrintLines(uint16_t uiXPosition,tuGraphicsColor color)
{
    vGraphicsPrintRectangle(uiXPosition,uiXPosition,UI_WAVEFORM_GRIDYSTART,UI_WAVEFORM_GRIDYEND,color);
    vGraphicsPrintRectangle(uiXPosition-UI_CURSER_BOUNDARY_LENGTH,uiXPosition+UI_CURSER_BOUNDARY_LENGTH,UI_WAVEFORM_GRIDYSTART,UI_WAVEFORM_GRIDYSTART+UI_CURSER_BOUNDARY_THICKNES,color);
    vGraphicsPrintRectangle(uiXPosition-UI_CURSER_BOUNDARY_LENGTH,uiXPosition+UI_CURSER_BOUNDARY_LENGTH,UI_WAVEFORM_GRIDYEND-UI_CURSER_BOUNDARY_THICKNES,UI_WAVEFORM_GRIDYEND,color);
}

/*
 * Printing a curser + cursertext
 *
 */
void inline vUICurserPrint(tsCurserStruct* curser)
{
    vUICurserPrintLines((*curser).uiActualxPosition, (*curser).curserColor);
    vTextPrintString((*curser).aucVoltageString,(*curser).voltageStringCoord,(*curser).curserColor);

    (*curser).state.stateSeperate.ucCurserVisible=1;
}

/*
 * Erase a curser + cursertext
 *
 */
void inline vUICurserErase(tsCurserStruct* curser)
{
        vUICurserPrintLines((*curser).uiActualxPosition, GRAPHICS_BLACKCOLOR);
        vTextEraseLine((*curser).voltageStringCoord,UI_CURSER_TEXT_LENGTH+1);

        (*curser).state.stateSeperate.ucCurserVisible=0;
}

/*
 * refreshs an already visible curser
 *
 */
void inline vUICurserRefresh(tsCurserStruct* curser)
{
    if((*curser).state.stateSeperate.ucCurserVisible)
    {
        vUICurserPrint(curser);
    }
}

/*
 *
 *   This function update the selected curser on the screen depending of the state of the curser. This function should be used with a constant frequency like 10 Hz.
 *
 */
void vUICurserUpdate(tsCurserStruct* curser)
{
    //calculate if curser position is static
    if((*curser).uiNextxPosition == (*curser).uiActualxPosition)
    {
        (*curser).state.stateSeperate.ucpositionStatic=1;
    }
    else
    {
        (*curser).state.stateSeperate.ucpositionStatic=0;
    }

    switch((*curser).state.stateNumber)
    {
    case UI_CURSER_STATE_E0_V1_PS0:     //CURSER IS NOT ENABLED BUT VISIBLE --> NEED TO BE ERASED, NO MATTER IF POSITION STATIC
                                        //Erase previous Curser and String
                                        vUICurserErase(curser);

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);
                                        break;

    case UI_CURSER_STATE_E0_V1_PS1:     //CURSER IS NOT ENABLED BUT VISIBLE --> NEED TO BE ERASED, NO MATTER IF POSITION STATIC
                                        //Erase previous Curser and String
                                        vUICurserErase(curser);

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);
                                        break;

    case UI_CURSER_STATE_E1_V0_PS0:     //CURSER IS ENABLED BUT NOT VISIBLE AND THE POSITION HAS CHANGED --> Need repositioning, calculate and print
                                        //repositioning
                                        //(*curser).uiNextxPosition=(*curser).uiActualxPosition;
    									(*curser).uiActualxPosition=(*curser).uiNextxPosition;

                                        //Calculate new Voltage Value String and coordinate
                                        vUICurserCalc(curser);

                                        //Print Curser
                                        vUICurserPrint(curser);
                                        break;

    case UI_CURSER_STATE_E1_V0_PS1:     //CURSER IS ENABLED BUT NOT VISIBLE BUT POSITION IS STATIC -->  calculate and print
                                        //Calculate new Voltage Value and Text coordinate
                                        vUICurserCalc(curser);

                                        //Print Curser
                                        vUICurserPrint(curser);
                                        break;

    case UI_CURSER_STATE_E1_V1_PS0:     //ENABLED; VISIBLE AND POSITION HAS CHANGED --> Erase previous Curser, refresh grid and waveform, calculate new voltage value String and coordinate
                                        //Erase previous Curser and String
                                        vUICurserErase(curser);

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                                        (*curser).uiActualxPosition=(*curser).uiNextxPosition;

                                        //Calculate new Voltage Value String and coordinate
                                        vUICurserCalc(curser);

                                        //Print Curser
                                        vUICurserPrint(curser);
                                        break;

    case UI_CURSER_STATE_E1_V1_PS1:     //MOST COMMON STATE: ENABLED; VISIBLE AND POSITION IS STATIC --> FOR NOW HERE IS AN INEFFICIENT WAY OF HANDLING THIS STATE. MORE EFFICIENT WAY WILL COME SOON
                                        //Erase previous Curser and String
                                        vUICurserErase(curser);

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                                        //Calculate new Voltage Value String and coordinate
                                        vUICurserCalc(curser);

                                        //Print Curser
                                        vUICurserPrint(curser);
                                        break;
    }

}
