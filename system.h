#ifndef SYSTEM_H_
#define SYSTEM_H_

/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* LibC Standard Integer                 */
#include "driverlib/sysctl.h"       /* TivaWare SysCtl Library               */


/*- Inline functions --------------------------------------------------------*/
static inline void vSystemClkInit(void)
{
    /* Configure System clock                                                */
    /* - Use 25MHz external main oscillator
     * - Use PLL with f_vco=480MHz
     * - SysClk = 120MHz
     *
     * Reference [SPMU298D] Section 26.2.2.2                                 */
    SysCtlClockFreqSet(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480, 120000000);
}

#endif /* SYSTEM_H_ */
