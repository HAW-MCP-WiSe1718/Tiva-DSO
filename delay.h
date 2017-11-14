#ifndef DELAY_H_
#define DELAY_H_

/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard Integer                 */


/*- Prototypes --------------------------------------------------------------*/
void vDelayInit(void);
void vDelay_us(uint16_t uiMicroseconds);
void vDelay_ms(uint16_t uiMilliseconds);

#endif /* DELAY_H_ */
