#ifndef TRIGGER_H_
#define TRIGGER_H_
/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard Integer                 */
#include <stdbool.h>


/*- Definitions -------------------------------------------------------------*/
#define TRIGGER_EXT_RCGC    SYSCTL_PERIPH_GPIOP
#define TRIGGER_EXT_PORT    GPIO_PORTP_BASE
#define TRIGGER_EXT_PIN     (1 << 0)                /* GPIOP 0               */

#define TRIGGER_COMP_RCGC   SYSCTL_PERIPH_COMP0
#define TRIGGER_COMP_IN     1                       /* PC5/4                 */
#define TRIGGER_COMP_G_RC   SYSCTL_PERIPH_GPIOC
#define TRIGGER_COMP_G_PORT GPIO_PORTC_BASE
#define TRIGGER_COMP_G_PIN  ((1 << 4)|(1 << 5))


/*- Macros ------------------------------------------------------------------*/
#define IS_TRIGGER_SRC(x)   (((x) == EN_TRIGGER_SRC_CONTINUOUS) ||            \
                             ((x) == EN_TRIGGER_SRC_COMPARATOR) ||            \
                             ((x) == EN_TRIGGER_SRC_EXTERNAL))
#define IS_TRIGGER_EDGE(x)  (((x) == EN_TRIGGER_EDGE_RISING)    ||            \
                             ((x) == EN_TRIGGER_EDGE_FALLING)   ||            \
                             ((x) == EN_TRIGGER_EDGE_BOTH))


/*- Type definitions --------------------------------------------------------*/
typedef enum tag_teTriggerSrc {
    /*! Untriggered timer sampling  */
    EN_TRIGGER_SRC_CONTINUOUS,

    /*! Analog comparator as source */
    EN_TRIGGER_SRC_COMPARATOR,

    /*! Digital input as source     */
    EN_TRIGGER_SRC_EXTERNAL
} teTriggerSrc;

typedef enum tag_teTriggerEdge {
    /*! Trigger on rising edge      */
    EN_TRIGGER_EDGE_RISING,

    /*! Trigger on falling edge     */
    EN_TRIGGER_EDGE_FALLING,

    /*! Trigger on both edges       */
    EN_TRIGGER_EDGE_BOTH
} teTriggerEdge;


/*- Function prototypes -----------------------------------------------------*/
void vTriggerInit(void);
void vTriggerConfigure(teTriggerEdge eTrigEdge, teTriggerSrc eTrigSrc);
void vTriggerArm(void);
void vTriggerDisarm(void);
bool bIsTriggerArmed(void);

#endif /* TRIGGER_H_ */
