/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#ifndef __OC_DEF_H__
#define __OC_DEF_H__
#include "cmsis_os.h"

#ifdef RT_TICK_PER_SECOND
#define OC_BASE_OS_USING_RT
#else
#define OC_BASE_OS_USING_FT
#endif 

typedef signed   char                   oc_int8_t;      /**<  8bit integer type */
typedef signed   short                  oc_int16_t;     /**< 16bit integer type */
typedef signed   long                   oc_int32_t;     /**< 32bit integer type */
typedef signed long long                oc_int64_t;     /**< 64bit integer type */
typedef unsigned char                   oc_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  oc_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned long                   oc_uint32_t;    /**< 32bit unsigned integer type */
typedef unsigned long long              oc_uint64_t;    /**< 64bit unsigned integer type */

typedef int                             oc_bool_t;      /**< boolean type */
typedef long                            oc_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   oc_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef oc_base_t                       oc_err_t;       /**< Type for error number */
typedef oc_uint32_t                     oc_time_t;      /**< Type for time stamp */
typedef oc_uint32_t                     oc_tick_t;      /**< Type for tick count */
typedef oc_base_t                       oc_flag_t;      /**< Type for flags */
typedef oc_ubase_t                      oc_size_t;      /**< Type for size number */
typedef oc_ubase_t                      oc_dev_t;       /**< Type for device */
typedef oc_base_t                       oc_off_t;       /**< Type for offset */


/* boolean type */
#define OC_TRUE                         1               /**< boolean true  */
#define OC_FALSE                        0               /**< boolean fails */

/* OC-OS error code definitions */
#define OC_EOK                          0               /**< There is no error */
#define OC_ERROR                        1               /**< A generic error happens */
#define OC_ETIMEOUT                     2               /**< Timed out */
#define OC_EFULL                        3               /**< The resource is full */
#define OC_EEMPTY                       4               /**< The resource is empty */
#define OC_ENOMEM                       5               /**< No memory */
#define OC_ENOSYS                       6               /**< No system */
#define OC_EBUSY                        7               /**< Busy */
#define OC_EIO                          8               /**< IO error */
#define OC_EINTR                        9               /**< Interrupted system call */
#define OC_EINVAL                       10              /**< Invalid argument */

/**
 * @def OC_NULL
 * Similar as the \c NULL in C library.
 */
#define OC_NULL                         (0)    

#define OC_WAIT_FOREVER     0xFFFFFFFF     ///< wait forever timeout value
#define OC_TASK_RUNNING               0x01                /**< Running status */
#define OC_TASK_SUSPEND               0x02                /**< Suspend status */
#define OC_TASK_CLOSE                 0x04                /**< Closed status */

typedef void *  oc_task_handle_t;

struct oc_task
{
    /* oc task */
    char        name[OC_NAME_MAX];                      /**< the name of task */
    oc_task_handle_t    id;
    osThreadDef_t      thread;
    /* stack point */
    oc_uint32_t stack_size;                             /**< stack size */


    oc_uint8_t  stat;                                   /**< thread status */

    /* priority */
    oc_uint8_t  current_priority;                       /**< current priority */
    oc_uint8_t  init_priority;                          /**< initialized priority */
};
typedef struct oc_task *oc_task_t;



typedef void *  oc_timer_handle_t;

struct oc_timer
{
    char        name[OC_NAME_MAX];                      /**< the name of timer */
    oc_timer_handle_t   id;                             /**< the id of timer */
    osTimerDef_t    opt;
    void            *parameter;                         /**< timeout function's parameter */

    oc_tick_t        init_tick;                         /**< timer timeout tick */
    oc_tick_t        timeout_tick;                      /**< timeout tick */
};
typedef struct oc_timer *oc_timer_t;



#endif //__OC_DEF_H__S



