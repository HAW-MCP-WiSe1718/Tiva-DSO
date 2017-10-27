#ifndef SAMPLER_H_
#define SAMPLER_H_

/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard integer                 */
#include "doc_markup.h"             /* Documentation markup defines          */


/*- Defines -----------------------------------------------------------------*/
#define SAMPLER_EXT_RCGC    SYSCTL_PERIPH_GPIOK /* External Trig GPIO Periph */
#define SAMPLER_EXT_PORT    GPIO_PORTK_BASE     /* External Trig GPIO Port   */
#define SAMPLER_EXT_PIN     7                   /* External Trig GPIO Pin    */

#define SAMPLER_BUF_LEN     480     /* Number of samples to be stored        */
#define SAMPLER_SAMPLE_INVALID (-1) /* Do not draw this sample               */

                                    /* Screen redraw / Pixel draw threshold  */
#define SAMPLER_SLOW_THR    EN_SAMPLER_TIMEBASE_100ms



/*- Global variables --------------------------------------------------------*/
extern global int16_t g_aiSampleBuffer[SAMPLER_BUF_LEN]; /* Sample buffer    */


/*- Type definitions --------------------------------------------------------*/
typedef enum tag_teSampleTrigSrc {
    /*! Disable triggering          */
    EN_SAMPLER_TRIGSRC_DISABLE,

    /*! Untriggered timer sampling  */
    EN_SAMPLER_TRIGSRC_CONTINUOUS,

    /*! Analog comparator as source */
    EN_SAMPLER_TRIGSRC_COMPARATOR,

    /*! Digital input as source     */
    EN_SAMPLER_TRIGSRC_EXTERNAL
} teSamplerTrigSrc;

typedef enum tag_teSamplerTrigMode {
    /*! Stop triggering             */
    EN_SAMPLER_TRIGMODE_STOP,

    /*! Single capture (480 sampl.) */
    EN_SAMPLER_TRIGMODE_SINGLE,

    /*! Re-Arm after 480 samples    */
    EN_SAMPLER_TRIGMODE_NORMAL
} teSamplerTrigMode;

typedef enum tag_teSamplerTimebase {
    EN_SAMPLER_TIMEBASE_100us = 300,
    EN_SAMPLER_TIMEBASE_200us = 600,
    EN_SAMPLER_TIMEBASE_500us = 1500,
    EN_SAMPLER_TIMEBASE_1ms = 3000,
    EN_SAMPLER_TIMEBASE_2ms = 6000,
    EN_SAMPLER_TIMEBASE_5ms = 15000,
    EN_SAMPLER_TIMEBASE_10ms = 30000,
    EN_SAMPLER_TIMEBASE_20ms = 60000,
    EN_SAMPLER_TIMEBASE_50ms = 150000,
    EN_SAMPLER_TIMEBASE_100ms = 300000,
    EN_SAMPLER_TIMEBASE_200ms = 600000,
    EN_SAMPLER_TIMEBASE_500ms = 1500000,
    EN_SAMPLER_TIMEBASE_1s = 3000000,
    EN_SAMPLER_TIMEBASE_2s = 6000000,
    EN_SAMPLER_TIMEBASE_5s = 15000000,
    EN_SAMPLER_TIMEBASE_MAX = 0xFFFFFFFF
} teSamplerTimebase;


/*- Prototypes --------------------------------------------------------------*/
void vSamplerInit(void);
void vSetSamplerTimebase(teSamplerTimebase eTimebase);
void vSetSamplerTrigger(teSamplerTrigSrc eTriggerSource, teSamplerTrigMode eTriggerMode);

#endif /* SAMPLER_H_ */
