/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard Integer                 */
#include <stdbool.h>                /* Libc Standard Boolean                 */
#include <assert.h>                 /* Assertions                            */
#include <inc/hw_memmap.h>          /* TivaWare Hardware Memory Map          */
#include <driverlib/sysctl.h>       /* TivaWare SysCtl DriverLib             */
#include <driverlib/pin_map.h>      /* TivaWare GPIO Pin Mapping             */
#include <driverlib/gpio.h>         /* TivaWare GPIO DriverLib               */
#include <driverlib/interrupt.h>    /* TivaWare Interrupt Library            */
#include <inc/hw_ints.h>            /* TivaWare Interrupt macros             */
#include <driverlib/comp.h>         /* TivaWare Comparator DriverLib         */
#include "sampler.h"                /* Sampler module                        */
#include "doc_markup.h"             /* Documentation                         */
#include "trigger.h"


/*- Global variables --------------------------------------------------------*/
global bool g_bTriggerArmed;        /* Used by ISR                           */


/*- Module variables --------------------------------------------------------*/
static teTriggerSrc eTriggerSrc;


/*- Module prototypes -------------------------------------------------------*/
void vTriggerContinuous(void);


/**
 *  @brief  Initialise Trigger module
 *
 *  Configures peripherals for trigger functionality
 *                                                                           */
void vTriggerInit(void)
{
    /* Enable peripheral clock for GPIOP and Comparator                      */
    SysCtlPeripheralEnable(TRIGGER_EXT_RCGC);
    SysCtlPeripheralEnable(TRIGGER_COMP_RCGC);
    SysCtlPeripheralEnable(TRIGGER_COMP_G_RC);
    asm("\tnop;\r\n\tnop;\r\n\tnop;\r\n");

    /* Configure GPIOP0 as input                                             */
    GPIOPinTypeGPIOInput(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN);
    IntEnable(INT_GPIOP0);

    /* Configure GPIOC4/5 as Comparator inputs                               */
    GPIOPinTypeComparator(TRIGGER_COMP_G_PORT, TRIGGER_COMP_G_PIN);
    IntEnable(INT_COMP0);

    /* Default settings                                                      */
    vTriggerConfigure(EN_TRIGGER_EDGE_RISING, EN_TRIGGER_SRC_CONTINUOUS);
}

/**
 *  @brief  Configure Trigger
 *
 *  @param[in]  eTrigEdge   Edge type (rising / falling / both)
 *  @param[in]  eTrigSrc    Trigger source
 *                                                                           */
void vTriggerConfigure(teTriggerEdge eTrigEdge, teTriggerSrc eTrigSrc)
{
    assert(IS_TRIGGER_EDGE(eTrigEdge));
    assert(IS_TRIGGER_SRC(eTrigSrc));

    /* Stop any active trigger                                               */
    vTriggerDisarm();

    /* Save trigger source for vTriggerArm                                   */
    eTriggerSrc = eTrigSrc;

    /* Configure selected peripheral                                         */
    switch (eTrigSrc)
    {
    case EN_TRIGGER_SRC_CONTINUOUS:
        /* Retriger on vTriggerArm()                                         */
        break;
    case EN_TRIGGER_SRC_COMPARATOR:
        /* Use Comparator inputs PC4/5 as trigger source                     */
        switch (eTrigEdge)
        {
        case EN_TRIGGER_EDGE_RISING:
            /* Trigger on comparator rising edge                             */
            ComparatorConfigure(COMP_BASE, TRIGGER_COMP_IN, COMP_INT_RISE);
            break;
        case EN_TRIGGER_EDGE_FALLING:
            /* Trigger on comparator falling edge                            */
            ComparatorConfigure(COMP_BASE, TRIGGER_COMP_IN, COMP_INT_FALL);
            break;
        case EN_TRIGGER_EDGE_BOTH:
            /* Trigger on both edges                                         */
            ComparatorConfigure(COMP_BASE, TRIGGER_COMP_IN, COMP_INT_BOTH);
            break;
        default:
            /* Invalid configuration                                         */
            assert(false);
        }
        break;
    case EN_TRIGGER_SRC_EXTERNAL:
        /* Use GPIO Pin P0 as external trigger input                         */
        switch (eTrigEdge)
        {
        case EN_TRIGGER_EDGE_RISING:
            /* Trigger on P0 rising edge                                     */
            GPIOIntTypeSet(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN, GPIO_DISCRETE_INT | GPIO_RISING_EDGE);
            break;
        case EN_TRIGGER_EDGE_FALLING:
            /* Trigger on P0 falling edge                                    */
            GPIOIntTypeSet(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN, GPIO_DISCRETE_INT | GPIO_FALLING_EDGE);
            break;
        case EN_TRIGGER_EDGE_BOTH:
            /* Trigger on P0 on both edges                                   */
            GPIOIntTypeSet(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN, GPIO_DISCRETE_INT | GPIO_BOTH_EDGES);
            break;
        default:
            /* Invalid configuration                                         */
            assert(false);
        }
        break;
    default:
        /* Invalid configuration                                             */
        assert(false);
    }
}

/**
 *  @brief  Arm trigger
 *
 *  Prepares trigger interrupt
 *                                                                           */
void vTriggerArm(void)
{
    /* Set status bit                                                        */
    g_bTriggerArmed = true;

    /* Configure interrupt sources                                           */
    switch (eTriggerSrc)
    {
    case EN_TRIGGER_SRC_CONTINUOUS:
        /* Instantly retrigger                                               */
        vTriggerContinuous();
        break;
    case EN_TRIGGER_SRC_COMPARATOR:
        /* Enable Comparator interrupts                                      */
        ComparatorIntClear(COMP_BASE, TRIGGER_COMP_IN);
        ComparatorIntEnable(COMP_BASE, TRIGGER_COMP_IN);
        break;
    case EN_TRIGGER_SRC_EXTERNAL:
        /* Enable GPIO Interrupts                                            */
        GPIOIntClear(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN);
        GPIOIntEnable(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN);
        break;
    default:
        /* Invalid configuration                                             */
        assert(false);
    }
}

/**
 *  @brief  Disarm trigger
 *
 *  Disables trigger interrupt sources
 *                                                                           */
void vTriggerDisarm(void)
{
    /* Disable interrupt generation for both peripherals                     */
    GPIOIntDisable(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN);
    ComparatorIntDisable(COMP_BASE, TRIGGER_COMP_IN);

    /* Clear any pending interrupt request                                   */
    IntPendClear(INT_GPIOP0);
    IntPendClear(INT_COMP0);

    /* Clear status bit                                                      */
    g_bTriggerArmed = false;
}

/**
 *  @brief  Return trigger module armed status
 *
 *  @return (bool)  true, if trigger is armed
 *                                                                           */
bool bIsTriggerArmed(void)
{
	return g_bTriggerArmed;
}

/**
 *  @brief  Continuous trigger handler
 *
 *  Used instead of ISR when continuous trigger source is selected
 *                                                                           */
void vTriggerContinuous(void)
{
    /* Abort, if trigger was disarmed                                        */
    if (!bIsTriggerArmed())
        return;

    /* Disarm trigger                                                        */
    vTriggerDisarm();

    /* Start sampling                                                        */
    vSamplerStart();
}

/**
 *  @brief  Trigger source interrupt service routines.
 *
 *  Handles Comparator and GPIO P0 interrupts
 *                                                                           */
void __attribute__((interrupt)) vISR_TriggerInterrupt(void)
{
    ComparatorIntClear(COMP_BASE, TRIGGER_COMP_IN);
    GPIOIntClear(TRIGGER_EXT_PORT, TRIGGER_EXT_PIN);

    /* Disarm trigger                                                        */
    vTriggerDisarm();

    /* Start sampling                                                        */
    vSamplerStart();
}
