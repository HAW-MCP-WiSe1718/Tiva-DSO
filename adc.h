#ifndef ADC_H_
#define ADC_H_

/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard integer                 */


/*- Defines -----------------------------------------------------------------*/
#define ADC_PORT_RCGC   SYSCTL_RCGCGPIO_R4      /* RCGC Index for GPIO       */
#if NO_USE_TIVA
#define ADC_PORT_AFSEL  GPIO_PORTE_AHB_AFSEL_R  /* GPIO AFSEL Register       */
#define ADC_PORT_AMSEL  GPIO_PORTE_AHB_AMSEL_R  /* GPIO AMSEL Register       */
#define ADC_PORT_DEN    GPIO_PORTE_AHB_DEN_R    /* GPIO DigEnable Register   */
#define ADC_PORT_DIR    GPIO_PORTE_AHB_DIR_R    /* GPIO Direction Register   */
#endif

#define ADC_VREFP       3.3F                    /* Positive voltage ref.     */
#define ADC_VREFN       0.0F                    /* Negative voltage ref.     */

#define ADC_SEQ_VIN     0                       /* Sequencer used for V_in   */
#define ADC_SEQ_VTRIG   1                       /* Sequencer used for V_trig */
#define ADC_SEQ_TEMP    2                       /* Sequencer used for Tempe. */

/*- Prototoypes -------------------------------------------------------------*/
void vAdcInit(void);
uint16_t uiGetAdcSampleVtrig(void);
int16_t iGetAdcChipTemperature(void);

#endif /* ADC_H_ */
