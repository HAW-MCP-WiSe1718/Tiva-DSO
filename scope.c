/*- Header files ------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "arraytools.h"
#include "dma.h"
#include "adc.h"
#include "sampler.h"
#include "trigger.h"
#include "delay.h"
#include "display.h"
#include "graphics.h"
#include "touch.h"
#include "ui.h"
#include "ui_touch.h"
#include "ui_waveform.h"
#include "ui_text.h"
#include "ui_curser.h"
#include "scope.h"


/*- Module variables --------------------------------------------------------*/
static const tsUiCoords asScopeUiCoords_RunStop[2] = { {0, 0}, {47, 17} };
static const tsUiCoords asScopeUiCoords_TriggerType[2] = { {60, 0}, {120, 17} };
static const tsUiCoords asScopeUiCoords_TriggerEdge[2] = { {150, 0}, {180, 17} };
static const tsUiCoords asScopeUiCoords_TimebaseSlower[2] = { {420, 0}, {449, 17} };
static const tsUiCoords asScopeUiCoords_TimebaseFaster[2] = { {451, 0}, {480, 17} };
static const tsUiCoords asScopeUiCoords_SetCursor1[2] = { {0, 18}, {480, 116} };
static const tsUiCoords asScopeUiCoords_DisableCursors[2] = { {0, 117}, {480, 176} };
static const tsUiCoords asScopeUiCoords_SetCursor2[2] = { {0, 177}, {480, 272} };
static const teTriggerEdge aeScopeTriggerEdges[] = { EN_TRIGGER_EDGE_RISING, EN_TRIGGER_EDGE_FALLING/*, EN_TRIGGER_EDGE_BOTH*/ };
static const teTriggerSrc aeScopeTriggerSources[] = { EN_TRIGGER_SRC_CONTINUOUS, EN_TRIGGER_SRC_EXTERNAL, EN_TRIGGER_SRC_COMPARATOR };
static const teSamplerTimebase aeScopeSamplerTimebase[] = { EN_SAMPLER_TIMEBASE_1ms, EN_SAMPLER_TIMEBASE_2ms, EN_SAMPLER_TIMEBASE_5ms,
                                                            EN_SAMPLER_TIMEBASE_10ms, EN_SAMPLER_TIMEBASE_20ms, EN_SAMPLER_TIMEBASE_50ms,
                                                            EN_SAMPLER_TIMEBASE_100ms, EN_SAMPLER_TIMEBASE_200ms, EN_SAMPLER_TIMEBASE_500ms,
                                                            EN_SAMPLER_TIMEBASE_1s, EN_SAMPLER_TIMEBASE_2s, EN_SAMPLER_TIMEBASE_5s };
static const char* acScopeUiTimebaseString[] = { "1MS/DIV", "2MS/DIV", "5MS/DIV",
												 "10MS/DIV", "20MS/DIV", "50MS/DIV",
												 "100MS/DIV", "200MS/DIV", "500MS/DIV",
												 "1S/DIV", "2S/DIV", "5S/DIV" };
static const teScopeMode aeScopeModes[] = { EN_SCOPE_MODE_STOP/*, EN_SCOPE_MODE_SINGLE*/, EN_SCOPE_MODE_RUN };

static teScopeState eScopeState = EN_SCOPE_STATE_INIT;
static uint8_t ucScopeUiTriggerEdgeIndex = 0;
static uint8_t ucScopeUiTriggerSourceIndex = 0;
static uint8_t ucScopeSamplerTimebaseIndex = 0;
static uint8_t ucScopeModeIndex = 0;


/*- Inline functions --------------------------------------------------------*/
static inline teTriggerEdge eGetScopeTriggerEdge(void)
{
    return aeScopeTriggerEdges[ucScopeUiTriggerEdgeIndex];
}

static inline teTriggerSrc eGetScopeTriggerSource(void)
{
    return aeScopeTriggerSources[ucScopeUiTriggerSourceIndex];
}

static inline teSamplerTimebase eGetScopeSamplerTimebase(void)
{
    return aeScopeSamplerTimebase[ucScopeSamplerTimebaseIndex];
}

static inline teScopeMode eGetScopeMode(void)
{
    return aeScopeModes[ucScopeModeIndex];
}


/*- Local prototypes --------------------------------------------------------*/
static void vScopeShowSplash(void);
static void vScopeMainUiInit(void);
static void vScopeMainUi(void);

void vScopeMainTouch_RunStop(tsUiCoords sTouchCoords);
void vScopeMainTouch_TriggerType(tsUiCoords sTouchCoords);
void vScopeMainTouch_TriggerEdge(tsUiCoords sTouchCoords);
void vScopeMainTouch_TimebaseSlower(tsUiCoords sTouchCoords);
void vScopeMainTouch_TimebaseFaster(tsUiCoords sTouchCoords);
void vScopeMainTouch_SetCursor1(tsUiCoords sTouchCoords);
void vScopeMainTouch_DisableCursors(tsUiCoords sTouchCoords);
void vScopeMainTouch_SetCursor2(tsUiCoords sTouchCoords);

/**
 *  @brief  Initialise scope modules and  peripherals
 *                                                                           */
void vScopeInit(void)
{
    /* Low-Level modules init                                                */
    vDmaInit();
    vDelayInit();
    vDisplayInit();
    vTouchInit();
    vAdcInit();
    vSamplerInit();
    vTriggerInit();
    vGraphicsInit();

    /* Init module variables                                                 */
    ucScopeModeIndex = 0;
    ucScopeUiTriggerSourceIndex = 0;
    ucScopeUiTriggerEdgeIndex = 0;
    vTriggerConfigure(aeScopeTriggerEdges[ucScopeUiTriggerEdgeIndex], aeScopeTriggerSources[ucScopeUiTriggerSourceIndex]);
    ucScopeSamplerTimebaseIndex = 0;
    vSamplerConfigure(aeScopeSamplerTimebase[ucScopeSamplerTimebaseIndex]);

    /* High-Level UI init                                                    */
    vUiTouchInit();

    /* Force Advance state machine to next state                             */
    eScopeState = EN_SCOPE_STATE_SPLASH;
    printf("[Scope]\tState changed:\tSplashScreen\r\n");
}

/**
 *  @brief  Application polling function to be run in the background task
 *                                                                           */
void vScopePoll(void)
{
    switch (eScopeState)
    {
    case EN_SCOPE_STATE_INIT:
        /* Should never be entered in normal program loop                    */
        vScopeInit();
        eScopeState = EN_SCOPE_STATE_SPLASH;
        printf("[Scope]\tState changed:\tSplashScreen\r\n");
        break;
    case EN_SCOPE_STATE_SPLASH:
        /* Display splash screen                                             */
        vScopeShowSplash();
        eScopeState = EN_SCOPE_STATE_MAINUIINIT;
        printf("[Scope]\tState changed:\tMainUiInit\r\n");
        break;
    case EN_SCOPE_STATE_MAINUIINIT:
        /* Initialise main UI (touch areas, texts etc.)                      */
        vScopeMainUiInit();
        eScopeState = EN_SCOPE_STATE_MAIN;
        printf("[Scope]\tState changed:\tMainUi\r\n");
        break;
    case EN_SCOPE_STATE_MAIN:
        /* Handle main UI                                                    */
        vScopeMainUi();
        break;
    default:
        /* Invalid state                                                     */
        assert(false);
    }
}

/**
 *  @brief  Display splash screen image and delay for specified wait time
 *                                                                           */
static void vScopeShowSplash(void)
{
    vDisplayClear();

    /* Display splash image                                                  */
    vTextPrintString("SCOPE   V1.0", (tsGraphicsCoord){195, 110}, GRAPHICS_WHITECOLOR);
    vTextPrintString("E6-MC/01 RMS", (tsGraphicsCoord){195, 120}, GRAPHICS_WHITECOLOR);
    vTextPrintString(" 06.12.2017 ", (tsGraphicsCoord){195, 140}, GRAPHICS_WHITECOLOR);
    vTextPrintString("EHLERS, KOCHAN, KROH", (tsGraphicsCoord){20, 250}, GRAPHICS_WHITECOLOR);

    vDelay_ms(SCOPE_SPLASH_DURATION * 1000);

    vDisplayClear();
}


/**
 *  @brief  Initialise main UI elements after splash screen is displayed
 *                                                                           */
static void vScopeMainUiInit(void)
{
    /* Add touch boxes                                                       */
    vUiTouchAddBox(0, asScopeUiCoords_RunStop[0], asScopeUiCoords_RunStop[1], vScopeMainTouch_RunStop);
    vUiTouchAddBox(1, asScopeUiCoords_TriggerType[0], asScopeUiCoords_TriggerType[1], vScopeMainTouch_TriggerType);
    vUiTouchAddBox(2, asScopeUiCoords_TriggerEdge[0], asScopeUiCoords_TriggerEdge[1], vScopeMainTouch_TriggerEdge);
    vUiTouchAddBox(3, asScopeUiCoords_TimebaseSlower[0], asScopeUiCoords_TimebaseSlower[1], vScopeMainTouch_TimebaseSlower);
    vUiTouchAddBox(4, asScopeUiCoords_TimebaseFaster[0], asScopeUiCoords_TimebaseFaster[1], vScopeMainTouch_TimebaseFaster);
    vUiTouchAddBox(5, asScopeUiCoords_SetCursor1[0], asScopeUiCoords_SetCursor1[1], vScopeMainTouch_SetCursor1);
    vUiTouchAddBox(6, asScopeUiCoords_DisableCursors[0], asScopeUiCoords_DisableCursors[1], vScopeMainTouch_DisableCursors);
    vUiTouchAddBox(7, asScopeUiCoords_SetCursor2[0], asScopeUiCoords_SetCursor2[1], vScopeMainTouch_SetCursor2);

    /* Initialise main UI (UiWaveform, UiText)                               */
    vUIWaveformInit();
    vUICurserInit();
    vTextInit();

    /* Print default settings 											  	 */
    vTextEraseLine(TEXT_STATUS_COORD, 5);
	vTextPrintString("STOP", TEXT_STATUS_COORD, GRAPHICS_REDCOLOR);
	vTextEraseLine(TEXT_TRIGSRC_COORD, 12);
	vTextPrintString("T:INTERNAL", TEXT_TRIGSRC_COORD, GRAPHICS_BLUECOLOR);
	vTextEraseLine(TEXT_TRIGEDGE_COORD, 2);
	vTextPrintString("[", TEXT_TRIGEDGE_COORD, GRAPHICS_BLUECOLOR);
	vTextPrintString("CH1", (tsGraphicsCoord){330,1}, GRAPHICS_YELLOWCOLOR);
	vTextPrintString("412MV/DIV", (tsGraphicsCoord){360,1}, GRAPHICS_YELLOWCOLOR);
	vTextEraseLine(TEXT_TIMEBASE_COORD, 10);
	vTextPrintString("1MS/DIV", TEXT_TIMEBASE_COORD, GRAPHICS_WHITECOLOR);
}

/**
 *  @brief  Main UI polling function
 *                                                                           */
static void vScopeMainUi(void)
{
    /* Handle touch input                                                    */
    vUiTouchPoll();

    /* Handle waveform update                                                */
    if (!bIsSamplerBusy() && !bIsTriggerArmed() && (eGetScopeMode() == EN_SCOPE_MODE_RUN))
    {
        /* Auto re-trigger when in RUN mode                                  */
        printf("Armed\r\n");
        vTriggerArm();
    }
    if (bIsSamplerDataReady())
    {
        /* Refresh waveform display                                          */
        printf("Update %4d\r\n", g_aiSampleBuffer[0]);
        vSamplerDataInvalidate();

        vUIWaveformUpdateWave(g_aiSampleBuffer);
        vUICurserUpdate(&curserA);
		vUICurserUpdate(&curserB);
    }
    else
    {
        /* Nothing to be done                                                */
        printf(".\r\n");
    }

    /* Limit refresh rate to avoid flicker                                   */
    vDelay_ms(SCOPE_REFRESH_DELAY);
}

void vScopeMainTouch_RunStop(tsUiCoords sTouchCoords)
{
    /* Select next mode in list                                              */
    ucScopeModeIndex = (ucScopeModeIndex + 1) % LENGTHOF(aeScopeModes);

    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tRUN/STOP (%d)\r\n", ucScopeModeIndex);

    /* Disarm trigger when switching to STOP                                 */
    switch (eGetScopeMode())
    {
    case EN_SCOPE_MODE_STOP:
    	vTextEraseLine(TEXT_STATUS_COORD,5);
    	vTextPrintString("STOP",TEXT_STATUS_COORD,GRAPHICS_REDCOLOR);
    	vTriggerDisarm();
    	break;
    case EN_SCOPE_MODE_SINGLE:
    	vTextEraseLine(TEXT_STATUS_COORD,5);
		vTextPrintString("SNGL",TEXT_STATUS_COORD,GRAPHICS_YELLOWCOLOR);
		vTriggerArm();
		break;
    case EN_SCOPE_MODE_RUN:
		vTextEraseLine(TEXT_STATUS_COORD,5);
		vTextPrintString("RUN",TEXT_STATUS_COORD,GRAPHICS_GREENCOLOR);
		vTriggerArm();
		break;
    }
}

void vScopeMainTouch_TriggerType(tsUiCoords sTouchCoords)
{
    ucScopeUiTriggerSourceIndex = (ucScopeUiTriggerSourceIndex + 1) % LENGTHOF(aeScopeTriggerSources);

    /* Reconfigure Trigger                                                   */
    vTriggerConfigure(eGetScopeTriggerEdge(), eGetScopeTriggerSource());

    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tTriggerType (%d)\r\n", ucScopeUiTriggerSourceIndex);

    /* Update UI															 */
    vTextEraseLine(TEXT_TRIGSRC_COORD,12);
    switch (eGetScopeTriggerSource())
    {
    case EN_TRIGGER_SRC_CONTINUOUS:
		vTextPrintString("T:INTERNAL",TEXT_TRIGSRC_COORD,GRAPHICS_BLUECOLOR);
    	break;
    case EN_TRIGGER_SRC_COMPARATOR:
    	vTextPrintString("T:COMPARAT",TEXT_TRIGSRC_COORD,GRAPHICS_BLUECOLOR);
		break;
    case EN_TRIGGER_SRC_EXTERNAL:
		vTextPrintString("T:EXTERNAL",TEXT_TRIGSRC_COORD,GRAPHICS_BLUECOLOR);
		break;
    }


    /* Arm trigger when in RUN / SINGLE mode                                 */
    if (aeScopeModes[ucScopeModeIndex] != EN_SCOPE_MODE_STOP)
    {
        vTriggerArm();
    }
}

void vScopeMainTouch_TriggerEdge(tsUiCoords sTouchCoords)
{
    ucScopeUiTriggerEdgeIndex = (ucScopeUiTriggerEdgeIndex + 1) % LENGTHOF(aeScopeTriggerEdges);

    /* Reconfigure Trigger                                                   */
    vTriggerConfigure(eGetScopeTriggerEdge(), eGetScopeTriggerSource());

    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tTriggerEdge (%d)r\n", ucScopeUiTriggerEdgeIndex);

    /* Update UI														 	 */
	vTextEraseLine(TEXT_TRIGEDGE_COORD, 2);
    switch (eGetScopeTriggerEdge())
    {
    case EN_TRIGGER_EDGE_RISING:
		vTextPrintString("[", TEXT_TRIGEDGE_COORD, GRAPHICS_BLUECOLOR);
		break;
    case EN_TRIGGER_EDGE_FALLING:
		vTextPrintString("\\", TEXT_TRIGEDGE_COORD, GRAPHICS_BLUECOLOR);
		break;
    case EN_TRIGGER_EDGE_BOTH:
		vTextPrintString(" ", TEXT_TRIGEDGE_COORD, GRAPHICS_BLUECOLOR);
		break;
    }

    /* Arm trigger when in RUN / SINGLE mode                                 */
    if (eGetScopeMode() != EN_SCOPE_MODE_STOP)
    {
        vTriggerArm();
    }
}

void vScopeMainTouch_TimebaseSlower(tsUiCoords sTouchCoords)
{
    ucScopeSamplerTimebaseIndex = ((ucScopeSamplerTimebaseIndex + 1) >= LENGTHOF(aeScopeSamplerTimebase)) ? (LENGTHOF(aeScopeSamplerTimebase) - 1) : (ucScopeSamplerTimebaseIndex + 1);

    /* Reconfigure sampler                                                   */
    vSamplerConfigure(eGetScopeSamplerTimebase());

    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tTimebaseSlow (%d)\r\n", ucScopeSamplerTimebaseIndex);

    /* Update UI															 */
    vTextEraseLine(TEXT_TIMEBASE_COORD, 10);
	vTextPrintString((uint8_t*)acScopeUiTimebaseString[ucScopeSamplerTimebaseIndex], TEXT_TIMEBASE_COORD, GRAPHICS_WHITECOLOR);

    /* Arm trigger when in RUN / SINGLE mode                                 */
    if (eGetScopeMode() != EN_SCOPE_MODE_STOP)
    {
        vTriggerArm();
    }
}

void vScopeMainTouch_TimebaseFaster(tsUiCoords sTouchCoords)
{
    ucScopeSamplerTimebaseIndex = ((ucScopeSamplerTimebaseIndex - 1) <= 0) ? 0 : (ucScopeSamplerTimebaseIndex - 1);

    /* Reconfigure sampler                                                   */
    vSamplerConfigure(eGetScopeSamplerTimebase());

    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tTimebaseFast (%d)\r\n", ucScopeSamplerTimebaseIndex);

    /* Update UI															 */
    vTextEraseLine(TEXT_TIMEBASE_COORD, 10);
	vTextPrintString((uint8_t*)acScopeUiTimebaseString[ucScopeSamplerTimebaseIndex], TEXT_TIMEBASE_COORD, GRAPHICS_WHITECOLOR);

    /* Arm trigger when in RUN / SINGLE mode                                 */
    if (aeScopeModes[ucScopeModeIndex] != EN_SCOPE_MODE_STOP)
    {
        vTriggerArm();
    }
}

void vScopeMainTouch_SetCursor1(tsUiCoords sTouchCoords)
{
    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tSetCursor1\r\n");

    curserA.uiNextxPosition = (unsigned)sTouchCoords.iX;
    curserA.state.stateSeperate.ucCurserEnabled = 1;
    vUICurserUpdate(&curserA);
}

void vScopeMainTouch_DisableCursors(tsUiCoords sTouchCoords)
{
    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tDisableCursors\r\n");

    curserA.state.stateSeperate.ucCurserEnabled = false;
    curserB.state.stateSeperate.ucCurserEnabled = false;
    vUICurserUpdate(&curserA);
	vUICurserUpdate(&curserB);
}

void vScopeMainTouch_SetCursor2(tsUiCoords sTouchCoords)
{
    /* Debug                                                                 */
    printf("[Scope]\tButton pressed:\tSetCursor2\r\n");

    curserB.uiNextxPosition = (unsigned)sTouchCoords.iX;
    curserB.state.stateSeperate.ucCurserEnabled = 1;
	vUICurserUpdate(&curserB);
}
