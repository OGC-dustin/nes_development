/*
 * OGC Engineering
 *   Library
 *     Operating System
 *       Non Real-Timed Polled Scheduler ( NRTPS )
 */

#include "app.h"
#include "ogc_lib_os_nrtps.h"
#include "hal.h"

int main( void )
{
     /* configure watchdog, clocks, pins, and other things that do not require the scheduler */
    hal_config();

     /* initialize the scheduler in preparation for tasks to be scheduled */
    ogc_lib_os_nrtps_init();

#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS
    /* run the OS test BEFORE continuing with other operations */
    bool ret_val;
    ret_val = run_unit_tests__lib_os_nrtps( );
    if ( false == ret_val )
    {
        while ( 1U ); /* hold here because the core scheduler failed */
    }
    ogc_lib_os_nrtps_init(); /* reinit after test completes */
#endif

     /* schedule any hal level tasks that need the scheduler */
    hal_init();

     /* schedule any application level tasks that need the scheduler */
    app_init();

     /* loop logic of the scheduler that should never end */
    ogc_lib_os_nrtps_start();

     /* just in case the loop logic returns, do a final catch, last will, or other clean up */
    ogc_lib_os_nrtps_end();

    return ( 0U );
}
