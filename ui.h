#ifndef UI_H_
#define UI_H_

/*- Headerfiles -------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard Integer                 */


/*- Type definitions --------------------------------------------------------*/
typedef struct __attribute__((packed)) tag_tsUiCoords {
    int16_t iX;
    int16_t iY;
} tsUiCoords;


/*- Prototypes --------------------------------------------------------------*/
void vUiInit(void);
void vUiPoll(void);

#endif /* UI_H_ */
