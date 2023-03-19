/*
 * OGC Engineering
 * Non Real-Timed Polled Scheduler ( NRTPS )
 * library
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib_os_nrtps.h"
#include "hal.h"

/* =====================================================================================================================
 *                                                                                            PRIVATE GLOBAL EVENT ARRAY
 * =====================================================================================================================
 */
volatile struct s_lib_os_nrtps_event
event_array[ LIB_OS_NRTPS__EVENT_ARRAY_SIZE ] =
    { enum_LIB_OS_NRTPS_EVENT__EMPTY, 0U, NULL, NULL };

/* =====================================================================================================================
 *                                                                                                      Public Functions
 * =====================================================================================================================
 */
/*                                                                         INIT function to clear status/KPI tracking */
void
lib_os_nrtps_init(
    void
    )
{
    uint8_t init_index = 0U;
    for ( ; init_index < LIB_OS_NRTPS__EVENT_ARRAY_SIZE; init_index++ )
    {
        event_array[ init_index ].status = enum_LIB_OS_NRTPS_EVENT__EMPTY;
        event_array[ init_index ].timestamp = 0U;
        event_array[ init_index ].callback_function = NULL;
        event_array[ init_index ].argument_pointer = NULL;
    }
    return;
}

/*                                                                           SET a new scheduled event into the array */
enum e_lib_os_nrtps_status
lib_os_nrtps_set(
    uint64_t task_period_tick,
    void( *function )( void* ),
    void* arguments
    )
{
    uint8_t set_index = 0U;
    for ( ; set_index < LIB_OS_NRTPS__EVENT_ARRAY_SIZE; set_index++ )
    {
        if ( enum_LIB_OS_NRTPS_EVENT__EMPTY == event_array[ set_index ].status )
        {
            event_array[ set_index ].status = enum_LIB_OS_NRTPS_EVENT__PENDING;
            event_array[ set_index ].timestamp = hal_get_sys_tick() + task_period_tick;
            event_array[ set_index ].callback_function = function;
            event_array[ set_index ].argument_pointer = arguments;
            if ( set_index < ( LIB_OS_NRTPS__EVENT_ARRAY_SIZE - 1U ) )
            {
                return ( enum_LIB_OS_NRTPS_STATUS__OCCUPIED );
            }
            else
            {
                return ( enum_LIB_OS_NRTPS_STATUS__FULL );
            }
        }
    }
    return ( enum_LIB_OS_NRTPS_STATUS__FULL );
}

/*                                         CLEAR for those times a scheduled event needs to be removed from the array */
enum e_lib_os_nrtps_status
lib_os_nrtps_clear(
    void( *function )( void* ),
    void* arguments
    )
{
    bool task_found = false;
    uint8_t clear_index = 0U;
    for ( ; clear_index < LIB_OS_NRTPS__EVENT_ARRAY_SIZE; clear_index++ )
    {
        if ( ( enum_LIB_OS_NRTPS_EVENT__PENDING == event_array[ clear_index ].status )
             && ( function == event_array[ clear_index ].callback_function )
             && ( arguments == event_array[ clear_index ].argument_pointer )
           )
        {
            event_array[ clear_index ].status = enum_LIB_OS_NRTPS_EVENT__EMPTY;
            event_array[ clear_index ].timestamp = 0U;
            event_array[ clear_index ].callback_function = NULL;
            event_array[ clear_index ].argument_pointer = NULL;
            task_found = true;
        }
    }

    if ( true == task_found )
    {
        return ( enum_LIB_OS_NRTPS_STATUS__EMPTY );
    }
    else
    {
        return ( enum_LIB_OS_NRTPS_STATUS__FAILURE );
    }
}

/*                                                                            START scheduler loop, should never exit */
void
lib_os_nrtps_start(
    void
    )
{
    uint8_t start_index = 0U;
    while ( start_index < LIB_OS_NRTPS__EVENT_ARRAY_SIZE )
    {
        /* TODO: add tracking to detect if there are no pending tasks remaining...  there should always be something */
        if ( ( enum_LIB_OS_NRTPS_EVENT__PENDING == event_array[ start_index ].status )
             && ( event_array[ start_index ].timestamp < hal_get_sys_tick() )
           )
        {
            if ( NULL != event_array[ start_index ].callback_function )
            {
                event_array[ start_index ].callback_function( event_array[ start_index ].argument_pointer );
            }
            event_array[ start_index ].status = enum_LIB_OS_NRTPS_EVENT__EMPTY;
        }
        start_index++;
        if ( LIB_OS_NRTPS__EVENT_ARRAY_SIZE == start_index )
        {
            start_index = 0U;
        }
    }
    return;
}

/*                                                                                  END catch if scheduler ever exits */
void
lib_os_nrtps_end(
    void
    )
{
    /* TODO: report any statistics and clean up before continuing oeprations */
}

/* =====================================================================================================================
 *                                                                                                          Unit Testing
 * =====================================================================================================================
 */
#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS
void test_function( void* arguments )
{
    if ( NULL == arguments )
    {

    }
    else
    {

    }
}

bool run_unit_tests__lib_os_nrtps( void )
{
    /*                                     alter the event array, run init and see if everything clears appropriately */
    uint8_t test_index = 0U;
    for ( ; test_index < LIB_OS_NRTPS__EVENT_ARRAY_SIZE; test_index++ )
    {
        event_array[ test_index ].status = enum_LIB_OS_NRTPS_EVENT__PENDING;
        event_array[ test_index ].timestamp = test_index;
        event_array[ test_index ].callback_function = test_function;
        event_array[ test_index ].argument_pointer = ( void* )test_index;
    }
    lib_os_nrtps_init(); /* init function to clear event array */
    test_index = 0U;
    for ( ; test_index < LIB_OS_NRTPS__EVENT_ARRAY_SIZE; test_index++ )
    {
        if ( ( enum_LIB_OS_NRTPS_EVENT__EMPTY != event_array[ test_index ].status )
             || ( 0U != event_array[ test_index ].timestamp )
             || ( NULL != event_array[ test_index ].callback_function )
             || ( NULL != event_array[ test_index ].argument_pointer )
           )
        {
            return ( false );
        }
    }

    /*                          set enough events to fill the array and make sure it reports correctly after each set */
    test_index = 0U;
    volatile enum e_lib_os_nrtps_status ret_value;
    for ( ; test_index < ( LIB_OS_NRTPS__EVENT_ARRAY_SIZE - 1U ); test_index++ )
    {
        ret_value = lib_os_nrtps_set( ( uint64_t )test_index, test_function, ( void* )test_index );
        if ( enum_LIB_OS_NRTPS_STATUS__OCCUPIED != ret_value )
        {
            return ( false );
        }
    }
    ret_value = lib_os_nrtps_set( ( uint64_t )test_index, test_function, ( void* )test_index );
    if ( enum_LIB_OS_NRTPS_STATUS__FULL != ret_value )
    {
        return ( false );
    }
    /*    array should be full - clear a specific event that doesn't exist and make sure that it return correct value */
    ret_value = lib_os_nrtps_clear( test_function, ( void* )LIB_OS_NRTPS__EVENT_ARRAY_SIZE );
    if ( enum_LIB_OS_NRTPS_STATUS__FAILURE != ret_value )
    {
        return ( false );
    }
    /*                      array should be full now - clear a specific event and make sure that event is now missing */
    ret_value = lib_os_nrtps_clear( test_function, ( void* )( LIB_OS_NRTPS__EVENT_ARRAY_SIZE - 1U ) );
    if ( enum_LIB_OS_NRTPS_STATUS__EMPTY != ret_value )
    {
        return ( false );
    }

    /* TODO: add testing of actual schedule START and END loop somehow, use #ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS */

    return ( true ); /* all tests passed */
}
#endif /* DEPLOYMENT_OPTION_RUN_UNIT_TESTS */
