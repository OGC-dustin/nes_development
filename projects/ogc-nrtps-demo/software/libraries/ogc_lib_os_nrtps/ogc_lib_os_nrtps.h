#ifndef OGC_LIB_OS_NRTPS_H
#define OGC_LIB_OS_NRTPS_H

#include <stdint.h>

/* =====================================================================================================================
 *                                                                                                   DEFAULT DEFINITIONS
 * =====================================================================================================================
 */
/* define LIB_OS_NRTPS__EVENT_ARRAY_SIZE in deployment header to change the number of spaces available to schedule new
 * events.  Must be at least the maximum number of tasks that will be scheduled at any one time. A good rule of thumb is
 * to have one slot per task ( including those that are called more than once and run as separate instances )
 */
#ifndef OGC_LIB_OS_NRTPS__EVENT_ARRAY_SIZE
#define OGC_LIB_OS_NRTPS__EVENT_ARRAY_SIZE ( ( uint8_t )5 )
#warning "DEFAULT value used in lib_os_nrtps.h: OGC_LIB_OS_NRTPS__EVENT_ARRAY_SIZE"
#endif

/* =====================================================================================================================
 *                                                                                           Enumerations and Structures
 * =====================================================================================================================
 */
/*                                                                                                     Library Status */
enum e_ogc_lib_os_nrtps_status
{
    E_OGC_LIB_OS_NRTPS_STATUS__FAILURE = -1,       /* failure in desired task */
    E_OGC_LIB_OS_NRTPS_STATUS__EMPTY = 0,              /* no events scheduled */
    E_OGC_LIB_OS_NRTPS_STATUS__OCCUPIED,   /* at least one event is scheduled */
    E_OGC_LIB_OS_NRTPS_STATUS__FULL,                   /* event array is full */
    E_OGC_LIB_OS_NRTPS_STATUS__LIMIT
};

struct s_ogc_lib_os_nrtps_status
{
    enum e_ogc_lib_os_nrtps_status status;     /* status for state of library */
    uint8_t kpi_highwatermark_usage;    /* track maximum number of slots used */
};

/*                                                                                                       Event Status */
enum e_ogc_lib_os_nrtps_event
{
    E_OGC_LIB_OS_NRTPS_EVENT__EMPTY = 0,      /* this event slot is available */
    E_OGC_LIB_OS_NRTPS_EVENT__PENDING,/* this event slot is pending execution */
    E_OGC_LIB_OS_NRTPS_EVENT__LIMIT
};

struct s_ogc_lib_os_nrtps_event
{
    enum e_ogc_lib_os_nrtps_event status;     /* general status of this entry */
    uint32_t timestamp;                  /* timestamp this event should occur */
    void( *callback_function )( void* );              /* callback to function */
    void* argument_pointer;      /* pointer to arguments required by callback */
};

/* =====================================================================================================================
 *                                                                                            Public Function Prototypes
 * =====================================================================================================================
 */
/*                                                                         INIT function to clear status/KPI tracking */
void
ogc_lib_os_nrtps_init(
    void
    );

/*                                                                           SET a new scheduled event into the array */
enum e_ogc_lib_os_nrtps_status
ogc_lib_os_nrtps_set(
    uint32_t period_tick,            /* timestamp to call associated function */
    void( *function )( void* ),                        /* associated function */
    void* arguments                  /* arguments, if any, needed by function */
    );

/*                                         CLEAR for those times a scheduled event needs to be removed from the array */
enum e_ogc_lib_os_nrtps_status
ogc_lib_os_nrtps_clear(
    void( *function )( void* ),                         /* match the function */
    void* arguments /* match arguments, if any, in case of multiple instances */
    );

/*                                                                            START scheduler loop, should never exit */
void
ogc_lib_os_nrtps_start(
    void
    );

/*                                                                                  END catch if scheduler ever exits */
void
ogc_lib_os_nrtps_end(
    void
    );

/* =====================================================================================================================
 *                                                                                                          Unit Testing
 * =====================================================================================================================
 */
#ifdef DEPLOYMENT_OPTION_RUN_UNIT_TESTS
#include <stdbool.h>
bool
run_unit_tests__ogc_lib_os_nrtps(
    void
    );
#endif /* DEPLOYMENT_OPTION_RUN_UNIT_TESTS */

#endif /* OGC_LIB_OS_NRTPS_H */
