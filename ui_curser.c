#include "ui_curser.h"
#include "ui_waveform.h"
#include "ui_text.h"


/*
 *  Calculate the voltage Value Text of Cursers
 *
 */
void static inline vUICurserCalc(void)
{
    uint16_t uiValueA=330*aiPrintBuffer[curserA.uiActualxPosition]/0xFF,uiValueB=330*aiPrintBuffer[curserB.uiActualxPosition]/0xFF;     // factor 100

    curserA.aucVoltageValue[2]=(int) uiValueA/100;
    curserA.aucVoltageValue[4]=(int) (uiValueA%100)/10;
    curserA.aucVoltageValue[5]=(int) (uiValueA%10);

    curserB.aucVoltageValue[2]=(int) uiValueB/100;
    curserB.aucVoltageValue[4]=(int) (uiValueB%100)/10;
    curserB.aucVoltageValue[5]=(int) (uiValueB%10);

    curserA.voltageCoord.uiX=curserA.uiActualxPosition-UI_CURSER_TEXT_X_OFFSET;
    curserB.voltageCoord.uiX=curserB.uiActualxPosition-UI_CURSER_TEXT_X_OFFSET;
}

/*
 *  Initialise Curser structures with constant and startvalues
 *
 */
void vUICurserInit(void)
{
    curserA.curserColor=UI_CURSER_A_COLOR;
    curserB.curserColor=UI_CURSER_B_COLOR;

    curserA.aucVoltageValue[0]='U';
    curserA.aucVoltageValue[1]='=';
    curserB.aucVoltageValue[2]='X';
    curserB.aucVoltageValue[3]='.';
    curserB.aucVoltageValue[4]='X';
    curserB.aucVoltageValue[5]='X';
    curserA.aucVoltageValue[6]='V';
    curserA.aucVoltageValue[7]='\0';

    curserB.aucVoltageValue[0]='U';
    curserB.aucVoltageValue[1]='=';
    curserB.aucVoltageValue[2]='X';
    curserB.aucVoltageValue[3]='.';
    curserB.aucVoltageValue[4]='X';
    curserB.aucVoltageValue[5]='X';
    curserB.aucVoltageValue[6]='V';
    curserB.aucVoltageValue[7]='\0';

    curserA.uiActualxPosition=UI_CURSER_A_INIT_POSITION;
    curserA.uiNextxPosition=UI_CURSER_A_INIT_POSITION;

    curserB.uiActualxPosition=UI_CURSER_B_INIT_POSITION;
    curserB.uiNextxPosition=UI_CURSER_B_INIT_POSITION;

    curserA.voltageCoord.uiX=curserA.uiActualxPosition-UI_CURSER_TEXT_X_OFFSET;
    curserA.voltageCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_A_TEXT_Y_OFFSET;

    curserB.voltageCoord.uiX=curserB.uiActualxPosition-UI_CURSER_TEXT_X_OFFSET;
    curserB.voltageCoord.uiY=UI_WAVEFORM_GRIDYSTART+UI_CURSER_B_TEXT_Y_OFFSET;

    curserA.ucCurserEnabled=0;
    curserA.ucCurserVisible=0;

    curserB.ucCurserEnabled=0;
    curserB.ucCurserVisible=0;
}


/*
 * Printing Curserlines using vGraphicsPrintRectangle();
 *
 */
void vUICurserPrint(uint16_t uiXPosition,tuGraphicsColor color)
{

vGraphicsPrintRectangle(uiXPosition,uiXPosition,UI_WAVEFORM_GRIDYSTART,UI_WAVEFORM_GRIDYEND,color);
vGraphicsPrintRectangle(uiXPosition-UI_CURSER_BOUNDARY_LENGTH,uiXPosition+UI_CURSER_BOUNDARY_LENGTH,UI_WAVEFORM_GRIDYSTART,UI_WAVEFORM_GRIDYSTART+UI_CURSER_BOUNDARY_THICKNES,color);
vGraphicsPrintRectangle(uiXPosition-UI_CURSER_BOUNDARY_LENGTH,uiXPosition+UI_CURSER_BOUNDARY_LENGTH,UI_WAVEFORM_GRIDYSTART-UI_CURSER_BOUNDARY_THICKNES,UI_WAVEFORM_GRIDYEND,color);

}


/*
 *
 *   DISCRIPTION SOON
 *
 */
void vUICurserUpdate(void)
{
    //CURSER A

    if(curserA.ucCurserEnabled)
    {
        if(curserA.ucCurserVisible)
        {
            if(curserA.uiNextxPosition==curserA.uiActualxPosition)
            {
                //Erase previous Text
                vTextEraseLine(curserA.voltageCoord,UI_CURSER_TEXT_LENGTH);

                //Calculate new Voltage Value and Text coordinate
                vUICurserCalc();

                //Print Curser
                vUICurserPrint(curserA.uiActualxPosition,curserA.curserColor);
                vTextPrintString(curserA.aucVoltageValue,curserA.voltageCoord,curserA.curserColor);
            }
            else
            {
                //Erase previous Curser and Text
                vUICurserPrint(curserA.uiActualxPosition,GRAPHICS_BLACKCOLOR);
                vTextEraseLine(curserA.voltageCoord,UI_CURSER_TEXT_LENGTH);

                //Reprint Grid
                vUIWaveformGridInit();

                //Reprint Wave
                vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                //update position
                curserA.uiActualxPosition=curserA.uiNextxPosition;

                //Calculate new Voltage Value and Text coordinate
                vUICurserCalc();

                //Print Curser
                vUICurserPrint(curserA.uiActualxPosition,curserA.curserColor);
                vTextPrintString(curserA.aucVoltageValue,curserA.voltageCoord,curserA.curserColor);
            }
        }
        else
        {
            //update position
            curserA.uiNextxPosition=curserA.uiActualxPosition;

            //Calculate new Voltage Value and Text coordinate
            vUICurserCalc();

            //Print Curser
            vUICurserPrint(curserA.uiActualxPosition,curserA.curserColor);
            vTextPrintString(curserA.aucVoltageValue,curserA.voltageCoord,curserA.curserColor);
        }

        curserA.ucCurserVisible=1;  //set curser as printed
    }
    else
    {
        if(curserA.ucCurserVisible)
        {
            //Erase previous Curser and Text
            vUICurserPrint(curserA.uiActualxPosition,GRAPHICS_BLACKCOLOR);
            vTextEraseLine(curserA.voltageCoord,UI_CURSER_TEXT_LENGTH);

            //Reprint Grid
            vUIWaveformGridInit();

            //Reprint Wave
            vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);
        }

        curserA.ucCurserVisible=0;
    }


    //CURSER B


    if(curserB.ucCurserEnabled)
        {
            if(curserB.ucCurserVisible)
            {
                if(curserB.uiNextxPosition==curserB.uiActualxPosition)
                {
                    //Erase previous Text
                    vTextEraseLine(curserB.voltageCoord,UI_CURSER_TEXT_LENGTH);

                    //Calculate new Voltage Value and Text coordinate
                    vUICurserCalc();

                    //Print Curser
                    vUICurserPrint(curserB.uiActualxPosition,curserB.curserColor);
                    vTextPrintString(curserB.aucVoltageValue,curserB.voltageCoord,curserB.curserColor);
                }
                else
                {
                    //Erase previous Curser and Text
                    vUICurserPrint(curserB.uiActualxPosition,GRAPHICS_BLACKCOLOR);
                    vTextEraseLine(curserB.voltageCoord,UI_CURSER_TEXT_LENGTH);

                    //Reprint Grid
                    vUIWaveformGridInit();

                    //Reprint Wave
                    vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);

                    //update position
                    curserB.uiActualxPosition=curserB.uiNextxPosition;

                    //Calculate new Voltage Value and Text coordinate
                    vUICurserCalc();

                    //Print Curser
                    vUICurserPrint(curserB.uiActualxPosition,curserB.curserColor);
                    vTextPrintString(curserB.aucVoltageValue,curserB.voltageCoord,curserB.curserColor);
                }
            }
            else
            {
                //update position
                curserB.uiNextxPosition=curserB.uiActualxPosition;

                //Calculate new Voltage Value and Text coordinate
                vUICurserCalc();

                //Print Curser
                vUICurserPrint(curserB.uiActualxPosition,curserB.curserColor);
                vTextPrintString(curserB.aucVoltageValue,curserB.voltageCoord,curserB.curserColor);
            }

            curserB.ucCurserVisible=1;  //set curser as printed
        }
        else
        {
            if(curserB.ucCurserVisible)
            {
                //Erase previous Curser and Text
                vUICurserPrint(curserB.uiActualxPosition,GRAPHICS_BLACKCOLOR);
                vTextEraseLine(curserB.voltageCoord,UI_CURSER_TEXT_LENGTH);

                //Reprint Grid
                vUIWaveformGridInit();

                //Reprint Wave
                vUIWaveformPrintWave(UI_WAVEFORM_WAVE_COLOR);
            }

            curserB.ucCurserVisible=0;
        }
}
