#include <stdlib.h>
#include <stdint.h>

#include "ogc_lib_os_nrtps.h"

struct cycle
{
    uint32_t loop;
    uint16_t color;
} this_app =
{
    0,
    0,
};

void main_application( void* cycle );

void app_init( void )
{
    enum e_ogc_lib_os_nrtps_status status;

    status = ogc_lib_os_nrtps_set( 1000U, main_application, &this_app );
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

void main_application( void* args )
{
    enum e_ogc_lib_os_nrtps_status status;
    struct cycle* this_data;
    volatile uint32_t bob;

    if ( NULL == args )
    {
        args = &this_app;
    }
    this_data = ( struct cycle* )args;

    bob = 50;

    switch ( this_data->loop )
    {
    case 0:
        this_data->loop++;
        status = ogc_lib_os_nrtps_set( 0U, main_application, args );
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
        break;
    case 1:
        this_data->loop++;
        bob += 10;
        status = ogc_lib_os_nrtps_set( 0U, main_application, args );
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
        break;
    case 2:
        this_data->loop = 0U;
        this_data->color++;
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
        break;
    }
}
