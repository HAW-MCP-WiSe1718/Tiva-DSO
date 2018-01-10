#ifndef SAMPLER_H_
#define SAMPLER_H_

/*- Header files ------------------------------------------------------------*/
#include <stdint.h>                 /* Libc Standard integer                 */
#include <stdbool.h>                /* Libc Standard boolean                 */
#include "doc_markup.h"             /* Documentation markup defines          */


/*- Defines -----------------------------------------------------------------*/
#define SAMPLER_BUF_LEN     480     /* Number of samples to be stored        */
#define SAMPLER_SAMPLE_INVALID (-1) /* Do not draw this sample               */

                                    /* Screen redraw / Pixel draw threshold  */
#define SAMPLER_SLOW_THR    EN_SAMPLER_TIMEBASE_100ms


/*- Global variables --------------------------------------------------------*/
extern global int16_t g_aiSampleBuffer[SAMPLER_BUF_LEN]; /* Sample buffer    */


/*- Type definitions --------------------------------------------------------*/
typedef enum tag_teSamplerTimebase {
    /*! 100us/div: 400kSps          */
    EN_SAMPLER_TIMEBASE_100us = 300,

    /*! 200us/div: 200kSps          */
    EN_SAMPLER_TIMEBASE_200us = 600,

    /*! 500us/div: 80kSps           */
    EN_SAMPLER_TIMEBASE_500us = 1500,

    /*! 1ms/div: 40kSps             */
    EN_SAMPLER_TIMEBASE_1ms = 3000,

    /*! 2ms/div: 20kSps             */
    EN_SAMPLER_TIMEBASE_2ms = 6000,

    /*! 5ms/div: 8kSps              */
    EN_SAMPLER_TIMEBASE_5ms = 15000,

    /*! 10ms/div: 4kSps             */
    EN_SAMPLER_TIMEBASE_10ms = 30000,

    /*! 20ms/div: 2kSps             */
    EN_SAMPLER_TIMEBASE_20ms = 60000,

    /*! 50ms/div: 800Sps            */
    EN_SAMPLER_TIMEBASE_50ms = 150000,

    /*! 100ms/div: 400Sps           */
    EN_SAMPLER_TIMEBASE_100ms = 300000,

    /*! 200ms/div: 200Sps           */
    EN_SAMPLER_TIMEBASE_200ms = 600000,

    /*! 500ms/div: 80Sps            */
    EN_SAMPLER_TIMEBASE_500ms = 1500000,

    /*! 1s/div: 40Sps               */
    EN_SAMPLER_TIMEBASE_1s = 3000000,

    /*! 2s/div: 20Sps               */
    EN_SAMPLER_TIMEBASE_2s = 6000000,

    /*! 5s/div: 8Sps                */
    EN_SAMPLER_TIMEBASE_5s = 15000000,

    /*! Do not use                  */
    EN_SAMPLER_TIMEBASE_MAX = 0xFFFFFFFF
} teSamplerTimebase;


/*- Prototypes --------------------------------------------------------------*/
void vSamplerInit(void);
void vSamplerConfigure(teSamplerTimebase eTimebase);
void vSamplerStart(void);
void vSamplerStop(void);
bool bIsSamplerBusy(void);
bool bIsSamplerDataReady(void);
void vSamplerDataInvalidate(void);
uint16_t uiSamplerGetTimebasePerDiv(void);

#endif /* SAMPLER_H_ */
