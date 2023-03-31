# OGC.Engineering
## ogc-lib-os-nrtps - Non Real-Time Polled Scheduler ( NRTPS )
Developer contact - dustin < at > ogc.engineering

---
- small footprint
    - Core logic      ( 438 Bytes )
    - Per Event Slot  ( 14 Bytes each ) /* adjustable depending on task count */
- cooperative
    - Tasks MUST yield for other tasks to be able to operate

Library provides main function with general system initialization calls and the nrtps loop call that scans a task array
checking for events that are ready.

This is a VERY cooperative system passing full system control over the each task.  It is up to each task to do the
smallest chunk of processing possible before yielding control so the scheduler can check for other tasks that are ready.

---
Requirements:

---

- the DEPLOYMENT/PROJECT layer(folder) must contain:
    - app.h header that redirects to the primary application ( because multiple applications are possible )

- the APP layer(folder) must define:
    - void app_init( void );
        - The function app_init() must schedule initial tasks to start operations
        - Task rescheduling can occur in serveal ways:
            - Tasks can schedule themselves or other tasks via a yield or timed ogc-lib-os-nrtps-set() call
            - Timer interrupts can schedule a task at regular intervals in the same manner

- the HAL layer(folder) must define:
    - void hal_config ( void );
        - configuration of the watchdog, clocks, pins, and other things that do not require the scheduler
    - void hal_init( void );
        - initialization of hal level nrtps tasks that need the scheduler ( i.e. background tasks )

---
Optional 

---
- define OGC_LIB_OS_NRTPS__EVENT_ARRAY_SIZE in the deployment header to override the default task array size
- define DEPLOYMENT_OPTION_RUN_UNIT_TESTS to test the library and other supported units

---
Usage

---

To schedule a task:
```
enum e_ogc_lib_os_nrtps_status status;
status = ogc_lib_os_nrtps_set( 1000U, main_application, NULL );
switch ( status )
{
    case E_OGC_LIB_OS_NRTPS_STATUS__OCCUPIED:
        /* normal response, task slot found with room remaining */
        break;
    case E_OGC_LIB_OS_NRTPS_STATUS__FULL:
        /* normal response, task slot found, but task array FULL */
        break;
    case E_OGC_LIB_OS_NRTPS_STATUS__FAILURE:
        /* error response, no roomm available to schedule new task */
        break;
}
```

To clear a task:
```
enum e_ogc_lib_os_nrtps_status status;
/* function name and a specific match to argument address needed */
/* if multiple functions and arguments match, I would expect all of them to be erased */
status = ogc_lib_os_nrtps_clear( main_application, &this-data1 );
switch ( status )
{
    case E_OGC_LIB_OS_NRTPS_STATUS__EMPTY:
        /* normal response, task slot found and removed */
        break;
    case E_OGC_LIB_OS_NRTPS_STATUS__FAILURE:
        /* error response, matching task not found */
        break;
}
```

To cast void* pointer to specific data type of interest:
```
/* data held in global struct */
struct data
{
    uint32_t member1;
    uint8_t memboer2;
}

struct data this_data1 = { 0 };
struct data this_data2 = { 0 };

void application_example( void* arg )
{
    enum e_ogc_lib_os_nrtps_status status;

    /* data comes in as a void* pointer, if NULL initialize it to something or change this to report error and exit */
    if ( NULL == args )
    {
        args = &this_app;
    }

    /* void* pointer needs to be cast as the data type of interest */
    struct cycle* this_data = ( struct cycle* )args;

    /* data can now be manipulated as normal */
    this_data->loop++;
    this_data->color++;

    /* when rescheduling, use the same arg passed in as it should now point to the data of interest */
    status = ogc_lib_os_nrtps_set( 1000U, main_application, args );
    switch ( status )
    {
        case E_OGC_LIB_OS_NRTPS_STATUS__OCCUPIED:
            /* normal response, task slot found with room remaining */
            break;
        case E_OGC_LIB_OS_NRTPS_STATUS__FULL:
            /* normal response, task slot found, but task array FULL */
            break;
        case E_OGC_LIB_OS_NRTPS_STATUS__FAILURE:
            /* error response, no roomm available to schedule new task */
            break;
    }
}
```
