/*- Header files ------------------------------------------------------------*/
#include <stdbool.h>                /* Libc Standard boolean                 */
#include <stdint.h>                 /* Libc Standard integer                 */
#include "tm4c1294ncpdt.h"          /* TivaWare Hardware Register Map        */
#include "inc/hw_memmap.h"          /* TivaWare Hardware Memory Map          */
#include "driverlib/gpio.h"         /* TivaWare GPIO DriverLib               */
#include "driverlib/pin_map.h"      /* TivaWare GPIO Pin Mappings            */
#include "driverlib/adc.h"          /* TivaWare ADC DriverLib                */
#include "driverlib/sysctl.h"       /* TivaWare SysCtl DriverLib             */
#include "adc.h"


/*- Local Prototypes --------------------------------------------------------*/
static uint16_t uiGetAdcSampleVtemp(void);


/**
 *  @brief  Initialise ADC module and analog inputs on GPIO
 *
 *  Initialise GPIO registers for PORTE, configure PE0 and PE1 as analog in-
 *  puts.
 *
 *  Sequencer 0: sampling PE0 (V_in), triggered by Timer (deferred setup)
 *  Sequencer 1: sampling PE1 (V_trig), triggered by software
 *  Sequencer 2: sampling Temperature, triggered continuously when idle
 *                                                                           */
void vAdcInit(void)
{
    /* Reference [TM4C1294NCPDT Datasheet] Section 15.4.1                    */
    /* Enable clock to GPIO PORTE                                            */
    SYSCTL_RCGCGPIO_R |= ADC_PORT_RCGC;
    /* Enable clock to ADC0 peripheral                                       */
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;
    asm("\tnop;\r\n\tnop;\r\n\tnop;");

    /* Configure PE0 (V_in) and PE1 (V_trig) as analog inputs                */
    //GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_ANALOG);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Configure ADC sample clock source                                     */
    /* Sample clock frequency: f_ADC = f_PLL / 24 = 480MHz / 24 = 20MHz      */
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 24);

    /* Configure ADC internal voltage reference to use internal 3.3V Ref.    */
    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);

    /* Disable all ADC sequencers for configuration                          */
    ADCSequenceDisable(ADC0_BASE, ADC_SEQ_VIN);
    ADCSequenceDisable(ADC0_BASE, ADC_SEQ_VTRIG);
    ADCSequenceDisable(ADC0_BASE, ADC_SEQ_TEMP);
    ADCSequenceDisable(ADC0_BASE, 3);

    /* Configure ADC Sequencer 0 for V_in (PE0, AIN3) sampling               */
    /* Sample started by processor in default configuration!                 *
     * Reconfigured to use Timer by Sampler module                           */
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQ_VIN, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQ_VIN, 0, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);

    /* Configure ADC Sequencer 1 for V_trig (PE1, AIN2) sampling             */
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQ_VTRIG, ADC_TRIGGER_PROCESSOR, 1);
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQ_VTRIG, 0, ADC_CTL_CH2 | ADC_CTL_END);

    /* Configure ADC Sequencer 2 for Temperature sampling                    */
    ADCSequenceConfigure(ADC0_BASE, ADC_SEQ_TEMP, ADC_TRIGGER_PROCESSOR, 2);
    ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQ_TEMP, 0, ADC_CTL_TS | ADC_CTL_END);

    /* Enable Sequencers                                                     */
    ADCSequenceEnable(ADC0_BASE, ADC_SEQ_VIN);
    ADCSequenceEnable(ADC0_BASE, ADC_SEQ_VTRIG);
    ADCSequenceEnable(ADC0_BASE, ADC_SEQ_TEMP);
}

/**
 *  @brief  Read ADC value for V_trig
 *
 *  Will busy-wait until sample is complete and stored in FIFO.
 *
 *  @return uint16_t    ADC value (12 bit resolution)
 *                                                                           */
uint16_t uiGetAdcSampleVtrig(void)
{
    /* Trigger Vtrig Sequencer sampling                                      */
    ADCProcessorTrigger(ADC0_BASE, ADC_SEQ_VTRIG | ADC_TRIGGER_SIGNAL);

    /* Wait for sample ready                                                 */
    while(ADC0_SSFSTAT1_R & ADC_SSFSTAT1_EMPTY);

    /* Read sample from FIFO                                                 */
    return ADC0_SSFIFO1_R;
}

/**
 *  @brief  Read ADC value for chip die temperature
 *
 *  Will busy-wait until sample is complete and stored in FIFO.
 *
 *  @return uint16_t    ADC value (12 bit resolution)
 *                                                                           */
uint16_t uiGetAdcSampleVtemp(void)
{
    /* Trigger Vtrig Sequencer sampling                                      */
    ADCProcessorTrigger(ADC0_BASE, ADC_SEQ_TEMP | ADC_TRIGGER_SIGNAL);

    /* Wait for sample ready                                                 */
    while(ADC0_SSFSTAT2_R & ADC_SSFSTAT2_EMPTY);

    /* Read sample from FIFO                                                 */
    return ADC0_SSFIFO2_R;
}

/**
 *  @brief  Read chip die temp value and convert to °C
 *
 *  @return int16_t     Chip die temperature in °C
 *                                                                           */
int16_t iGetAdcChipTemperature(void)
{
    register uint16_t uiTemperatureValue = uiGetAdcSampleVtemp();

    /* Convert ADC value to temperature                                      */
    /* Reference [TM4C1294NCPDT Datasheet] Section 15.3.6                    */
    return 147.5 - ((75 * (ADC_VREFP - ADC_VREFN) * uiTemperatureValue) / 4096);
}
