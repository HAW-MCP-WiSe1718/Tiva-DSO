#include "ui_curser.h"
#include "ui_waveform.h"
#include "ui_text.h"


/*
 *  Calculate the voltage Value Text of Cursers
 *
 */
void static inline vUICurserCalc()
{
    uint16_t uiVoltageA;
    uint16_t uiVoltageB;

    uint16_t uiTimeA;
    uint16_t uiTimeB;

    uint16_t uiDeltaVoltage;
    uint16_t uiDeltaTime;

    uint8_t uiDeltaVoltageAvaiable=1;

    //get actual timebase
    cursers.uiTimeBaseMiliPerDiv=uiSamplerGetTimebasePerDiv();

    //calculate time that is represented by curser position
    uiTimeA=(cursers.curserA.uiActualxPosition/UI_WAVEFORM_GRIDXSPACE)*cursers.uiTimeBaseMiliPerDiv;
    uiTimeB=(cursers.curserB.uiActualxPosition/UI_WAVEFORM_GRIDXSPACE)*cursers.uiTimeBaseMiliPerDiv;

    //CURSER A
    //VOLTAGE
    if(aiPrintBuffer[cursers.curserA.uiActualxPosition] > -1)
    {
        uiVoltageA=330*aiPrintBuffer[cursers.curserA.uiActualxPosition]/0xFF;     // factor 100

        cursers.curserA.aucVoltageString[2]='0' + uiVoltageA/100;
        cursers.curserA.aucVoltageString[4]='0' + (uiVoltageA%100)/10;
        cursers.curserA.aucVoltageString[5]='0' + (uiVoltageA%10);
    }
    else
    {
        uiDeltaVoltageAvaiable=0;
        cursers.curserA.aucVoltageString[2]='X';
        cursers.curserA.aucVoltageString[4]='X';
        cursers.curserA.aucVoltageString[5]='X';
    }

    //TIME

    if(uiTimeA < 1000)
    {
        cursers.curserA.aucTimeString[2]='0' + uiTimeA/100;
        cursers.curserA.aucTimeString[3]='0' + (uiTimeA%100)/10;
        cursers.curserA.aucTimeString[4]='0' + (uiTimeA%10);
        cursers.curserA.aucTimeString[5]='M';
        cursers.curserA.aucTimeString[6]='S';
    }
    else if(uiTimeA < 10000)
    {
        cursers.curserA.aucTimeString[2]='0' + uiTimeA/1000;
        cursers.curserA.aucTimeString[3]='.';
        cursers.curserA.aucTimeString[4]='0' + (uiTimeA%1000)/100;
        cursers.curserA.aucTimeString[5]='0' + (uiTimeA%100)/10;
        cursers.curserA.aucTimeString[6]='S';
    }
    else
    {
        cursers.curserA.aucTimeString[2]='0' + uiTimeA/10000;
        cursers.curserA.aucTimeString[3]='0' + (uiTimeA%10000)/1000;
        cursers.curserA.aucTimeString[4]='.';
        cursers.curserA.aucTimeString[5]='0' + (uiTimeA%1000)/100;
        cursers.curserA.aucTimeString[6]='S';
    }

    //COORDINATE
    if(cursers.curserA.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET <= UI_WAVEFORM_GRIDXSTART)
    {
        cursers.curserA.voltageStringCoord.uiX=cursers.curserA.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET;
        cursers.curserA.timeStringCoord.uiX=cursers.curserA.uiActualxPosition-UI_CURSER_TIME_X_OFFSET;
    }
    else
    {
        cursers.curserA.voltageStringCoord.uiX=cursers.curserA.uiActualxPosition+3;
        cursers.curserA.timeStringCoord.uiX=cursers.curserA.uiActualxPosition+3;
    }

    //CURSER B
    if(aiPrintBuffer[cursers.curserB.uiActualxPosition] > -1)
    {
        uiVoltageB=330*aiPrintBuffer[cursers.curserB.uiActualxPosition]/0xFF;     // factor 100

        cursers.curserB.aucVoltageString[2]='0' + uiVoltageB/100;
        cursers.curserB.aucVoltageString[4]='0' + (uiVoltageB%100)/10;
        cursers.curserB.aucVoltageString[5]='0' + (uiVoltageB%10);
    }
    else
    {
        uiDeltaVoltageAvaiable=0;
        cursers.curserB.aucVoltageString[2]='X';
        cursers.curserB.aucVoltageString[4]='X';
        cursers.curserB.aucVoltageString[5]='X';
    }

    //TIME

    if(uiTimeB < 1000)
    {
        cursers.curserB.aucTimeString[2]='0' + uiTimeB/100;
        cursers.curserB.aucTimeString[3]='0' + (uiTimeB%100)/10;
        cursers.curserB.aucTimeString[4]='0' + (uiTimeB%10);
        cursers.curserB.aucTimeString[5]='M';
        cursers.curserB.aucTimeString[6]='S';
    }
    else if(uiTimeB < 10000)
    {
        cursers.curserB.aucTimeString[2]='0' + uiTimeB/1000;
        cursers.curserB.aucTimeString[3]='.';
        cursers.curserB.aucTimeString[4]='0' + (uiTimeB%1000)/100;
        cursers.curserB.aucTimeString[5]='0' + (uiTimeB%100)/10;
        cursers.curserB.aucTimeString[6]='S';
    }
    else
    {
        cursers.curserB.aucTimeString[2]='0' + uiTimeB/10000;
        cursers.curserB.aucTimeString[3]='0' + (uiTimeB%10000)/1000;
        cursers.curserB.aucTimeString[4]='.';
        cursers.curserB.aucTimeString[5]='0' + (uiTimeB%1000)/100;
        cursers.curserB.aucTimeString[6]='S';
    }

    //COORDINATE
    if(cursers.curserB.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET <= UI_WAVEFORM_GRIDXSTART)
    {
        cursers.curserB.voltageStringCoord.uiX=cursers.curserB.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET;
        cursers.curserB.timeStringCoord.uiX=cursers.curserB.uiActualxPosition-UI_CURSER_TIME_X_OFFSET;
    }
    else
    {
        cursers.curserB.voltageStringCoord.uiX=cursers.curserB.uiActualxPosition+3;
        cursers.curserB.timeStringCoord.uiX=cursers.curserB.uiActualxPosition+3;
    }

    //Calculate Deltas
    //TIME
    if(uiTimeA>uiTimeB)
    {
        uiDeltaTime=uiTimeA-uiTimeB;
    }
    else
    {
        uiDeltaTime=uiTimeB-uiTimeA;
    }

    if(uiDeltaTime < 1000)
    {
        cursers.aucDeltaTimeString[3]='0' + uiDeltaTime/100;
        cursers.aucDeltaTimeString[4]='0' + (uiDeltaTime%100)/10;
        cursers.aucDeltaTimeString[5]='0' + (uiDeltaTime%10);
        cursers.aucDeltaTimeString[6]='M';
        cursers.aucDeltaTimeString[7]='S';
    }
    else if(uiDeltaTime < 10000)
    {
        cursers.aucDeltaTimeString[3]='0' + uiDeltaTime/1000;
        cursers.aucDeltaTimeString[4]='.';
        cursers.aucDeltaTimeString[5]='0' + (uiDeltaTime%1000)/100;
        cursers.aucDeltaTimeString[6]='0' + (uiDeltaTime%100)/10;
        cursers.aucDeltaTimeString[7]='S';
    }
    else
    {
        cursers.aucDeltaTimeString[3]='0' + uiDeltaTime/10000;
        cursers.aucDeltaTimeString[4]='0' + (uiDeltaTime%10000)/1000;
        cursers.aucDeltaTimeString[5]='.';
        cursers.aucDeltaTimeString[6]='0' + (uiDeltaTime%1000)/100;
        cursers.aucDeltaTimeString[7]='S';
    }

    if(uiDeltaVoltageAvaiable)
    {
        if(uiVoltageA>uiVoltageB)
        {
            uiDeltaVoltage=uiVoltageA-uiVoltageB;
        }
        else
        {
            uiDeltaVoltage=uiVoltageB-uiVoltageA;
        }

        cursers.aucDeltaVoltageString[3]='0' + uiDeltaVoltage/100;
        cursers.aucDeltaVoltageString[5]='0' + (uiDeltaVoltage%100)/10;
        cursers.aucDeltaVoltageString[6]='0' + (uiDeltaVoltage%10);

    }
    else
    {
        cursers.aucDeltaVoltageString[3]='X';
        cursers.aucDeltaVoltageString[5]='X';
        cursers.aucDeltaVoltageString[6]='X';

    }
}

/*
 *  Initialise Curser structures with constant and startvalues
 *
 */
void vUICurserInit(void)
{
    cursers.curserA.curserColor=UI_CURSER_A_COLOR;
    cursers.curserB.curserColor=UI_CURSER_B_COLOR;

    cursers.curserDeltaColor=UI_CURSER_TEXT_DELTA_COLOR;

    cursers.curserA.aucVoltageString[0]='U';
    cursers.curserA.aucVoltageString[1]='=';
    cursers.curserA.aucVoltageString[2]='X';
    cursers.curserA.aucVoltageString[3]='.';
    cursers.curserA.aucVoltageString[4]='X';
    cursers.curserA.aucVoltageString[5]='X';
    cursers.curserA.aucVoltageString[6]='V';
    cursers.curserA.aucVoltageString[7]='\0';

    cursers.curserB.aucVoltageString[0]='U';
    cursers.curserB.aucVoltageString[1]='=';
    cursers.curserB.aucVoltageString[2]='X';
    cursers.curserB.aucVoltageString[3]='.';
    cursers.curserB.aucVoltageString[4]='X';
    cursers.curserB.aucVoltageString[5]='X';
    cursers.curserB.aucVoltageString[6]='V';
    cursers.curserB.aucVoltageString[7]='\0';

    cursers.curserA.aucTimeString[0]='t';
    cursers.curserA.aucTimeString[1]='=';
    cursers.curserA.aucTimeString[2]='X';
    cursers.curserA.aucTimeString[3]='X';
    cursers.curserA.aucTimeString[4]='X';
    cursers.curserA.aucTimeString[5]='X';
    cursers.curserA.aucTimeString[6]='X';
    cursers.curserA.aucTimeString[7]='\0';

    cursers.curserB.aucTimeString[0]='t';
    cursers.curserB.aucTimeString[1]='=';
    cursers.curserB.aucTimeString[2]='X';
    cursers.curserB.aucTimeString[3]='X';
    cursers.curserB.aucTimeString[4]='X';
    cursers.curserB.aucTimeString[5]='X';
    cursers.curserB.aucTimeString[6]='X';
    cursers.curserB.aucTimeString[7]='\0';

    cursers.aucDeltaTimeString[0]='d';
    cursers.aucDeltaTimeString[1]='t';
    cursers.aucDeltaTimeString[2]='=';
    cursers.aucDeltaTimeString[3]='X';
    cursers.aucDeltaTimeString[4]='X';
    cursers.aucDeltaTimeString[5]='X';
    cursers.aucDeltaTimeString[6]='X';
    cursers.aucDeltaTimeString[7]='X';
    cursers.aucDeltaTimeString[8]='\0';

    cursers.aucDeltaVoltageString[0]='d';
    cursers.aucDeltaVoltageString[1]='V';
    cursers.aucDeltaVoltageString[2]='=';
    cursers.aucDeltaVoltageString[3]='X';
    cursers.aucDeltaVoltageString[4]='.';
    cursers.aucDeltaVoltageString[5]='X';
    cursers.aucDeltaVoltageString[6]='X';
    cursers.aucDeltaVoltageString[7]='V';
    cursers.aucDeltaVoltageString[8]='\0';

    cursers.curserA.uiActualxPosition=UI_CURSER_A_INIT_POSITION;
    cursers.curserA.uiNextxPosition=UI_CURSER_A_INIT_POSITION;

    cursers.curserB.uiActualxPosition=UI_CURSER_B_INIT_POSITION;
    cursers.curserB.uiNextxPosition=UI_CURSER_B_INIT_POSITION;

    cursers.curserA.voltageStringCoord.uiX=cursers.curserA.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET;
    cursers.curserA.voltageStringCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_A_VOLTAGE_Y_OFFSET;

    cursers.curserB.voltageStringCoord.uiX=cursers.curserB.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET;
    cursers.curserB.voltageStringCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_B_VOLTAGE_Y_OFFSET;

    cursers.curserA.timeStringCoord.uiX=cursers.curserA.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET;
    cursers.curserA.timeStringCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_A_TIME_Y_OFFSET;

    cursers.curserB.timeStringCoord.uiX=cursers.curserB.uiActualxPosition-UI_CURSER_VOLTAGE_X_OFFSET;
    cursers.curserB.timeStringCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_B_TIME_Y_OFFSET;

    cursers.deltaVoltageStringCoord.uiX=UI_CURSER_DELTA_VOLTAGE_X_POSITION;
    cursers.deltaVoltageStringCoord.uiY=UI_CURSER_DELTA_VOLTAGE_Y_POSITION;

    cursers.deltaTimeStringCoord.uiX=UI_CURSER_DELTA_TIME_X_POSITION;
    cursers.deltaTimeStringCoord.uiY=UI_CURSER_DELTA_TIME_Y_POSITION;

    cursers.state.stateSeperate.ucCurserEnabled=0;
    cursers.state.stateSeperate.ucCurserVisible=0;
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
}

/*
 * Erase a curser + cursertext
 *
 */
void inline vUICurserErase(tsCurserStruct* curser)
{
    vUICurserPrintLines((*curser).uiActualxPosition, GRAPHICS_BLACKCOLOR);
}

/*
 * Erase  cursertext
 *
 */
void inline vUICurserTextErase()
{
    vTextEraseLine(cursers.curserA.voltageStringCoord,UI_CURSER_VOLTAGE_LENGTH-1);
    vTextEraseLine(cursers.curserA.timeStringCoord,UI_CURSER_TIME_LENGTH-1);

    vTextEraseLine(cursers.curserB.voltageStringCoord,UI_CURSER_VOLTAGE_LENGTH-1);
    vTextEraseLine(cursers.curserB.timeStringCoord,UI_CURSER_TIME_LENGTH-1);

    vTextEraseLine(cursers.deltaVoltageStringCoord,UI_CURSER_DELTA_VOLTAGE_LENGTH-1);
    vTextEraseLine(cursers.deltaTimeStringCoord,UI_CURSER_DELTA_TIME_LENGTH-1);
}

/*
 * Print  cursertext
 *
 */
void inline vUICurserTextPrint()
{
    vTextPrintString(cursers.curserA.aucVoltageString,cursers.curserA.voltageStringCoord,cursers.curserA.curserColor);
    vTextPrintString(cursers.curserA.aucTimeString,cursers.curserA.timeStringCoord,cursers.curserA.curserColor);

    vTextPrintString(cursers.curserB.aucVoltageString,cursers.curserB.voltageStringCoord,cursers.curserB.curserColor);
    vTextPrintString(cursers.curserB.aucTimeString,cursers.curserB.timeStringCoord,cursers.curserB.curserColor);

    vTextPrintString(cursers.aucDeltaVoltageString,cursers.deltaVoltageStringCoord,cursers.curserDeltaColor);
    vTextPrintString(cursers.aucDeltaTimeString,cursers.deltaTimeStringCoord,cursers.curserDeltaColor);
}

/*
 * update cursertext
 *
 */
void inline vUICurserUpdateText()
{
    vUICurserTextErase();
    vUICurserTextPrint();
}

/*
 * refreshs an already visible curser
 *
 */
void inline vUICurserRefresh()
{
    if(cursers.state.stateSeperate.ucCurserVisible)
    {
        vUICurserPrint(&(cursers.curserA));
        vUICurserPrint(&(cursers.curserB));
        vUICurserTextPrint();
    }
}

/*
 *
 *   This function update the selected curser on the screen depending of the state of the curser. This function should be used with a constant frequency like 10 Hz.
 *
 */
void vUICurserUpdate()
{
    //calculate if curser position is static
    if(cursers.curserA.uiNextxPosition == cursers.curserA.uiActualxPosition && cursers.curserB.uiNextxPosition == cursers.curserB.uiActualxPosition)
    {
        cursers.state.stateSeperate.ucpositionStatic=1;
    }
    else
    {
        cursers.state.stateSeperate.ucpositionStatic=0;
    }

    switch(cursers.state.stateNumber)
    {
    case UI_CURSER_STATE_E0_V1_PS0:     //CURSER IS NOT ENABLED BUT VISIBLE --> NEED TO BE ERASED, NO MATTER IF POSITION STATIC
                                        //Erase previous Curser
                                        vUICurserErase(&(cursers.curserA));
                                        vUICurserErase(&(cursers.curserB));

                                        //Erase previous Text
                                        vUICurserTextErase();

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                                        cursers.state.stateSeperate.ucCurserVisible=0;
                                        break;

    case UI_CURSER_STATE_E0_V1_PS1:     //CURSER IS NOT ENABLED BUT VISIBLE --> NEED TO BE ERASED, NO MATTER IF POSITION STATIC
                                        //Erase previous Curser and String
                                        vUICurserErase(&(cursers.curserA));
                                        vUICurserErase(&(cursers.curserB));

                                        //Erase previous Text
                                        vUICurserTextErase();

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                                        //Set state as not visible
                                        cursers.state.stateSeperate.ucCurserVisible=0;
                                        break;

    case UI_CURSER_STATE_E1_V0_PS0:     //CURSER IS ENABLED BUT NOT VISIBLE AND THE POSITION HAS CHANGED --> Need repositioning, calculate and print
                                        //repositioning
    									cursers.curserA.uiActualxPosition=cursers.curserA.uiNextxPosition;
    									cursers.curserB.uiActualxPosition=cursers.curserB.uiNextxPosition;

                                        //Calculate new Voltage and Time Value String and coordinate
                                        vUICurserCalc();

                                        //Print Curser and Text
                                        vUICurserPrint(&(cursers.curserA));
                                        vUICurserPrint(&(cursers.curserB));
                                        vUICurserTextPrint();

                                        //Set state as visible
                                        cursers.state.stateSeperate.ucCurserVisible=1;
                                        break;

    case UI_CURSER_STATE_E1_V0_PS1:     //CURSER IS ENABLED BUT NOT VISIBLE BUT POSITION IS STATIC -->  calculate and print
                                        //Calculate new Voltage and Time Value String and coordinate
                                        vUICurserCalc();

                                        //Print Cursers and Text
                                        vUICurserPrint(&(cursers.curserA));
                                        vUICurserPrint(&(cursers.curserB));
                                        vUICurserTextPrint();

                                        //Set state as visible
                                        cursers.state.stateSeperate.ucCurserVisible=1;

    case UI_CURSER_STATE_E1_V1_PS0:     //ENABLED; VISIBLE AND POSITION HAS CHANGED --> Erase previous Curser, refresh grid and waveform, calculate new voltage value String and coordinate
                                        //Erase previous Cursers and String
                                        vUICurserErase(&(cursers.curserA));
                                        vUICurserErase(&(cursers.curserB));
                                        vUICurserTextErase();

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                                        //repositioning
                                        cursers.curserA.uiActualxPosition=cursers.curserA.uiNextxPosition;
                                        cursers.curserB.uiActualxPosition=cursers.curserB.uiNextxPosition;

                                        //Calculate new Voltage and Time Value String and coordinate
                                        vUICurserCalc();

                                        //Print Cursers and Text
                                        vUICurserPrint(&(cursers.curserA));
                                        vUICurserPrint(&(cursers.curserB));
                                        vUICurserTextPrint();
                                        break;

    case UI_CURSER_STATE_E1_V1_PS1:     //MOST COMMON STATE: ENABLED; VISIBLE AND POSITION IS STATIC --> FOR NOW HERE IS AN INEFFICIENT WAY OF HANDLING THIS STATE. MORE EFFICIENT WAY WILL COME SOON
                                        //Erase previous Curser and String
                                        vUICurserTextErase();

                                        //Reprint Grid
                                        vUIWaveformGridInit();

                                        //Reprint Wave
                                        vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                                        //Calculate new Voltage Value String and coordinate
                                        vUICurserCalc();

                                        //refresh cursers and text
                                        vUICurserPrint(&(cursers.curserA));
                                        vUICurserPrint(&(cursers.curserB));
                                        vUICurserTextPrint();
                                        break;
    }

}
