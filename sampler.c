/*- Header files ------------------------------------------------------------*/
#include <assert.h>                 /* Libc Assertions                       */
#include <stdbool.h>                /* Libc Standard boolean                 */
#include <stdint.h>                 /* Libc Standard integer                 */
#include <string.h>                 /* Libc String functions (Memset)        */
#include "tm4c1294ncpdt.h"          /* TivaWare Register Map                 */
#include "inc/hw_memmap.h"          /* TivaWare Memory Map                   */
#include "driverlib/gpio.h"         /* TivaWare GPIO DriverLib               */
#include "driverlib/pin_map.h"      /* TivaWare GPIO Pin Mapping             */
#include "driverlib/adc.h"          /* TivaWare ADC DriverLib                */
#include "driverlib/timer.h"        /* TivaWare GPTM DriverLib               */
#include "driverlib/sysctl.h"       /* TivaWare SysCtl DriverLib             */
#include "driverlib/interrupt.h"    /* TivaWare Interrupt Library            */
#include "adc.h"                    /* ADC Module                            */
#include "sampler.h"

#define ADC_INT_TIMING 1

/*- Macros ------------------------------------------------------------------*/
#define IS_SAMPLER_TIMEBASE(x)  ((x)>0x00 && (x)<EN_SAMPLER_TIMEBASE_MAX)
#define IS_SAMPLER_TRIGSRC(x)   (((x) == EN_SAMPLER_TRIGSRC_DISABLE)    ||    \
                                 ((x) == EN_SAMPLER_TRIGSRC_CONTINUOUS) ||    \
                                 ((x) == EN_SAMPLER_TRIGSRC_COMPARATOR) ||    \
                                 ((x) == EN_SAMPLER_TRIGSRC_EXTERNAL))
#define IS_SAMPLER_TRIGMODE(x)  (((x) == EN_SAMPLER_TRIGMODE_STOP)     ||     \
                                 ((x) == EN_SAMPLER_TRIGMODE_SINGLE)   ||     \
                                 ((x) == EN_SAMPLER_TRIGMODE_NORMAL))


/*- Global variables --------------------------------------------------------*/
global int16_t g_aiSampleBuffer[SAMPLER_BUF_LEN] = {[0 ... SAMPLER_BUF_LEN-1] = SAMPLER_SAMPLE_INVALID};


/*- Inline functions --------------------------------------------------------*/
/**
 *  @brief Clear sample buffer and reset each position to "invalid" value
 *                                                                           */
static inline void vSamplerClearBuffer(int16_t* p_aiBuffer)
{
    memset(p_aiBuffer, SAMPLER_SAMPLE_INVALID, SAMPLER_BUF_LEN * sizeof(*p_aiBuffer));
}


/*- Local prototypes --------------------------------------------------------*/
static void vSamplerStop(void);
static void vSamplerStartTimer(void);


/**
 *  @brief  Initialise the sampler module
 *
 *  Configures GPT1 as potential ADC trigger, and GPIO pin PE
 *                                                                           */
void vSamplerInit(void)
{
    /* Setup GPIO Port K Run Mode Clock                                      */
    SysCtlPeripheralEnable(SAMPLER_EXT_RCGC);
    /* Setup GPTM Timer 1 Run Mode Clock                                     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    asm("\tnop;\r\n\tnop;\r\n\tnop;\r\n");

    /* Prepare PK7 as trigger input                                          */
    /* not yet implemented                                                   */

    /* Prepare GPT1 as ADC trigger                                           */
    TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerPrescaleSet(TIMER1_BASE, TIMER_BOTH, 0);
    TimerMatchSet(TIMER1_BASE, TIMER_BOTH, 0);
    TimerLoadSet(TIMER1_BASE, TIMER_BOTH, EN_SAMPLER_TIMEBASE_1ms);
    TimerControlTrigger(TIMER1_BASE, TIMER_BOTH, true);

    /* Configure ADC Sample 0 (Vin) complete interrupt                       */
    IntEnable(INT_ADC0SS0);
}

/**
 *  @brief  Reconfigure Timer ILR value for selected timebase
 *
 *  @param[in]  eTimebase   Selected timebase
 *                                                                           */
void vSetSamplerTimebase(teSamplerTimebase eTimebase)
{
    assert(IS_SAMPLER_TIMEBASE(eTimebase));

    TimerLoadSet(TIMER1_BASE, TIMER_BOTH, (uint32_t)eTimebase);
}

/**
 *  @brief  Configure Sampler trigger setup
 *
 *  @param[in]  eTriggerSource  Trigger source
 *  @param[in]  eTriggerMode    Trigger mode
 *                                                                           */
void vSetSamplerTrigger(teSamplerTrigSrc eTriggerSource, teSamplerTrigMode eTriggerMode)
{
    assert(IS_SAMPLER_TRIGSRC(eTriggerSource));
    assert(IS_SAMPLER_TRIGMODE(eTriggerMode));

    switch (eTriggerSource)
    {
    case EN_SAMPLER_TRIGSRC_CONTINUOUS:
        /* Configure continuous triggering from timer                        */
        if (eTriggerMode == EN_SAMPLER_TRIGMODE_STOP)
        {
            vSamplerStop();
        }
        else
        {
            vSamplerStartTimer();
        }
        break;
    case EN_SAMPLER_TRIGSRC_COMPARATOR:
        /* Not yet implemented                                               */
        assert(false);
    case EN_SAMPLER_TRIGSRC_EXTERNAL:
        /* Not yet implemented                                               */
        assert(false);
    case EN_SAMPLER_TRIGSRC_DISABLE:
    default:
        vSamplerStop();
    }
}

/**
 *  @brief  Stop sample timer and reconfigure ADC sequencer for SSn-bit trig.
 *                                                                           */
void vSamplerStop(void)
{
    /* Disable ADC Sequence Complete Interrupt                               */
    ADCIntDisable(ADC0_BASE, ADC_SEQ_VIN);
    /* Reset ADC trigger to SS0-bit in PSSI register                         */
    ADCSequenceDisable(ADC0_BASE, ADC_SEQ_VIN);
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQ_VIN, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceEnable(ADC0_BASE, ADC_SEQ_VIN);
    /* Disable running timer                                                 */
    TimerDisable(TIMER1_BASE, TIMER_BOTH);
}

/**
 *  @brief  Configure ADC sequencer to trigger from Timer 1
 *                                                                           */
static void vSamplerStartTimer(void)
{
    /* Clear Sample bufffer                                                  */
    vSamplerClearBuffer(g_aiSampleBuffer);
    /* Enable timer                                                          */
    TimerEnable(TIMER1_BASE, TIMER_BOTH);
    /* Configure ADC sequencer to trigger from timer                         */
    ADCSequenceDisable(ADC0_BASE, ADC_SEQ_VIN);
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQ_VIN, ADC_TRIGGER_TIMER, 0);
    ADCSequenceEnable(ADC0_BASE, ADC_SEQ_VIN);
    /* Enable ADC Sequence Complete interrupt                                */
    ADCIntEnable(ADC0_BASE, ADC_SEQ_VIN);
}

/**
 *  ADC Sample Complete interrupt service routine
 *                                                                           */
void __attribute__((interrupt)) vISR_AdcVinSequencer(void)
{
    ADCIntClear(ADC0_BASE, ADC_SEQ_VIN);

    /* Not yet implemented                                                   */
    ;

    g_aiSampleBuffer[0] = ADC0_SSFIFO0_R;
}
