/*- Headerfiles -------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard Integer                 */
#include <stdbool.h>                /* Libc Standard Boolean                 */
#include <driverlib/udma.h>         /* TivaWare uDMA DriverLib               */
#include <driverlib/sysctl.h>       /* TivaWare SysCtl DriverLib             */
#include <inc/hw_memmap.h>          /* TivaWare Hardware Memory Map          */
#include "dma.h"


/*- Module variables --------------------------------------------------------*/
static uint8_t aucDmaControlTable[1024] __attribute__((aligned(1024)));


/**
 *  @brief  Enable the uDMA Controller peripheral and setup control table
 */
void vDmaInit(void)
{
    /* Enable uDMA Controller peripheral                                     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    asm("\tnop;\r\n\tnop;\r\n\tnop;\r\n");

    /* Enable the uDMA controller                                            */
    uDMAEnable();

    /* Configure uDMA control table                                          */
    uDMAControlBaseSet(aucDmaControlTable);
}
