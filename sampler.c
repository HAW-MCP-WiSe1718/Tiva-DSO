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
#include "driverlib/udma.h"         /* TivaWare uDMA DriverLib               */
#include "adc.h"                    /* ADC Module                            */
#include "sampler.h"

#define ADC_INT_TIMING 1

/*- Macros ------------------------------------------------------------------*/
#define IS_SAMPLER_TIMEBASE(x)  ((x)>0x00 && (x)<EN_SAMPLER_TIMEBASE_MAX)


/*- Global variables --------------------------------------------------------*/
global int16_t g_aiSampleBuffer[SAMPLER_BUF_LEN] = {[0 ... SAMPLER_BUF_LEN-1] = SAMPLER_SAMPLE_INVALID};
global volatile bool g_bSamplerDataReady;   /* Used by ISR                   */


/*- Module variables --------------------------------------------------------*/
static volatile bool bSamplerSlowTimebase;


/*- Inline functions --------------------------------------------------------*/
/**
 *  @brief Clear sample buffer and reset each position to "invalid" value
 *                                                                           */
static inline void vSamplerClearBuffer(int16_t* p_aiBuffer)
{
    memset(p_aiBuffer, SAMPLER_SAMPLE_INVALID, SAMPLER_BUF_LEN * sizeof(*p_aiBuffer));
}

static inline void vSamplerSetupDMA(void)
{
    uDMAChannelDisable(UDMA_CHANNEL_ADC0);
    uDMAChannelTransferSet(UDMA_CHANNEL_ADC0 | UDMA_PRI_SELECT,
                           UDMA_MODE_BASIC,
                           (void*)&ADC0_SSFIFO0_R,
                           g_aiSampleBuffer,
                           SAMPLER_BUF_LEN);
    uDMAChannelEnable(UDMA_CHANNEL_ADC0);
}

#if 0
/*- Local prototypes --------------------------------------------------------*/
static void vSamplerStop(void);
static void vSamplerStartTimer(void);
#endif

/**
 *  @brief  Initialise the sampler module
 *
 *  Configures GPT1 as potential ADC trigger, and GPIO pin PE
 *                                                                           */
void vSamplerInit(void)
{
    /* Setup GPTM Timer 1 Run Mode Clock                                     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    asm("\tnop;\r\n\tnop;\r\n\tnop;\r\n");

    /* Prepare GPT1 as ADC trigger                                           */
    TimerClockSourceSet(TIMER1_BASE, TIMER_CLOCK_SYSTEM);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerPrescaleSet(TIMER1_BASE, TIMER_BOTH, 0);
    TimerMatchSet(TIMER1_BASE, TIMER_BOTH, 0);
    /* Timer load value is set by vSamplerConfigure later on.                */
    TimerControlTrigger(TIMER1_BASE, TIMER_BOTH, true);

    /* Initialise status variables                                           */
    g_bSamplerDataReady = false;
    bSamplerSlowTimebase = false;

    /* Configure ADC sequencer to trigger from timer                         */
    ADCSequenceDisable(ADC0_BASE, ADC_SEQ_VIN);
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQ_VIN, ADC_TRIGGER_TIMER, 0);
    ADCSequenceEnable(ADC0_BASE, ADC_SEQ_VIN);

    /* Configure uDMA channel 14 for ADC0SS0                                 */
    ADCSequenceDMAEnable(ADC0_BASE, 0);
    uDMAChannelAttributeDisable(UDMA_CHANNEL_ADC0, UDMA_ATTR_ALL);
    uDMAChannelControlSet(UDMA_CHANNEL_ADC0 | UDMA_PRI_SELECT,
                          UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1);

    /* Configure default setup                                               */
    vSamplerConfigure(EN_SAMPLER_TIMEBASE_10ms);

    /* Enable ADC Sample Sequencer 0 Interrupt Handler                       */
    IntEnable(INT_ADC0SS0);
}

/**
 *  @brief  Reconfigure Timer ILR value for selected timebase
 *
 *  @param[in]  eTimebase   Selected timebase
 *                                                                           */
void vSamplerConfigure(teSamplerTimebase eTimebase)
{
    assert(IS_SAMPLER_TIMEBASE(eTimebase));

    /* Disable old interrupts                                                */
    ADCIntDisableEx(ADC0_BASE, ADC_INT_SS0 | ADC_INT_DMA_SS0);

    /* Stop any running timer                                                */
    vSamplerStop();

    /* Load new Timebase configuration into timer                            */
    TimerLoadSet(TIMER1_BASE, TIMER_BOTH, (uint32_t)eTimebase);

    /* Clear old buffer entries                                              */
    vSamplerClearBuffer(g_aiSampleBuffer);

    /* Force refresh on next UI cycle                                        */
    g_bSamplerDataReady = true;

    /* Determine screen update rate                                          */
    bSamplerSlowTimebase = eTimebase >= SAMPLER_SLOW_THR;

    /* Enable DMA transfer to buffer                                         */
    vSamplerSetupDMA();
}

/**
 *  @brief  Start sampling
 *
 *  Enables required interrupts and starts the Sample Timer, triggering the
 *  ADC.
 *                                                                           */
void vSamplerStart(void)
{
#if 0
    if (bIsSamplerBusy())
    {
        /* Already running                                                   */
        return;
    }
#endif

    /* Set update interrupt sources                                          */
    ADCIntEnableEx(ADC0_BASE, (bSamplerSlowTimebase) ? ADC_INT_SS0 | ADC_INT_DMA_SS0 : ADC_INT_DMA_SS0);

    /* Start Timer                                                           */
    TimerEnable(TIMER1_BASE, TIMER_BOTH);
}

/**
 *  @brief  Stop sampling
 *
 *  Interrupts the sampling process by disabling interrupts and stopping the
 *  Sample Timer.
 *  Resets the DMA transfer configuration to start at buffer index 0.
 *                                                                           */
void vSamplerStop(void)
{
    /* Disable interrupts                                                    */
    ADCIntDisableEx(ADC0_BASE, ADC_INT_SS0 | ADC_INT_DMA_SS0);

    /* Clear pending interrupts, only useful when called within ISR          */
    IntPendClear(INT_ADC0SS0);

    /* Stop timer                                                            */
    uDMAChannelDisable(UDMA_CHANNEL_ADC0);
    TimerDisable(TIMER1_BASE, TIMER_BOTH);

    /* Reset DMA transfer                                                    */
    vSamplerSetupDMA();
}

/**
 *  @brief  Check ADC Sampler timer status
 *
 *  @return (bool)  true, if sampler is running ("busy")
 *                                                                           */
bool bIsSamplerBusy(void)
{
    return TIMER0_CTL_R & TIMER_CTL_TAEN;
}

/**
 *  @brief  Check if sample data is ready for redrawing
 *
 *  @return (brief)     true, if sampler data is ready in the global buffer
 *                                                                           */
bool bIsSamplerDataReady(void)
{
    return g_bSamplerDataReady;
}

/**
 *  @brief  Invalidate currently buffered sampler data
 *
 *  Used to mark sampler data as processed
 *                                                                           */
void vSamplerDataInvalidate(void)
{
    g_bSamplerDataReady = false;
}

/**
 *  @brief  ADC Sequencer / DMA Complete interrupt
 *
 *  Handles stopping the sequencer after 480 samples and setting the signal
 *  bits to allow display redrawing.
 *
 *  @note   Interrupt Service Routine. Do not call from linear program flow.
 *                                                                           */
void __attribute__((interrupt)) vISR_AdcVinSequencer(void)
{
    if (ADCIntStatusEx(ADC0_BASE, false) & ADC_INT_DMA_SS0)
    {
        /* DMA transfer complete interrupt                                   */

        /* Clear interrupt flags                                             */
        ADCIntClearEx(ADC0_BASE, ADC_INT_SS0 | ADC_INT_DMA_SS0);

        /* Stop timer after last sample is saved -> "single-capture"         */
        vSamplerStop();
    }
    else
    {
        /* ADC sequencer interrupt                                           */

        /* Clear interrupt flags                                             */
        ADCIntClearEx(ADC0_BASE, ADC_INT_SS0);
    }

    /* Signal new data ready for processing                                  */
    g_bSamplerDataReady = true;
}
