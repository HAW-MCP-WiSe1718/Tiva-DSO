#ifndef SCOPE_H_
#define SCOPE_H_

/*- Header files ------------------------------------------------------------*/


/*- Defines -----------------------------------------------------------------*/
#define SCOPE_SPLASH_DURATION   5   /* Splash screen duation in seconds      */
#define SCOPE_REFRESH_DELAY   100   /* Redraw delay in ms                    */


/*- Type definitions --------------------------------------------------------*/
typedef enum tag_teScopeMode {
    /*! Acquisition is stopped      */
    EN_SCOPE_MODE_STOP,

    /*! Re-trigger automatically    */
    EN_SCOPE_MODE_RUN,

    /*! Single-shot capture         */
    EN_SCOPE_MODE_SINGLE
} teScopeMode;

typedef enum tag_teScopeState {
    EN_SCOPE_STATE_INIT,
    EN_SCOPE_STATE_SPLASH,
    EN_SCOPE_STATE_MAINUIINIT,
    EN_SCOPE_STATE_MAIN
} teScopeState;


/*- Prototypes --------------------------------------------------------------*/
void vScopeInit(void);
void vScopePoll(void);              /* Background task                       */


#endif /* SCOPE_H_ */
