/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#ifndef __OC_H__
#define __OC_H__

#include "oc_config.h"
#include "oc_def.h"
#include "oc_log.h"
#include "board.h"
#include "cmsis_os.h"

oc_err_t oc_init(void);



typedef enum  {
  OC_TASK_PRIORITY_IDLE          = 1,          ///< priority: idle (lowest)
  OC_TASK_PRIORITY_LOW          = 8,          ///< priority: low
  OC_TASK_PRIORITY_BELOWNORMAL   = 16,          ///< priority: below normal
  OC_TASK_PRIORITY_NORMAL       =  24,          ///< priority: normal (default)
  OC_TASK_PRIORITY_aboveNormal   = 32,          ///< priority: above normal
  OC_TASK_PRIORITY_HIGH          = 40,          ///< priority: high
  OC_TASK_PRIORITY_REALTIME      = 48,          ///< priority: realtime (highest)
  OC_TASK_PRIORITY_ERROR         =  56        ///< system cannot determine priority or thread has illegal priority
} task_priority;

typedef enum  {
  OC_TIMER_ONEC     =0,
  OC_TIMER_PERIODIC =1 
}oc_timer_type;

oc_task_t oc_task_create(const char* name,\
                void (*oc_pthread) (void *parameter),\
                void* parameter,\
                oc_uint32_t stack_size,\
                oc_uint8_t priority);
oc_task_handle_t oc_task_get_id(void);
oc_err_t oc_task_delete(oc_task_t task);
oc_err_t oc_task_yield(void);
oc_err_t oc_task_set_priority(oc_task_t task, osPriority priority);
task_priority oc_task_get_priority(oc_task_t task);
oc_err_t oc_task_suspend(oc_task_t task);
oc_err_t oc_task_resume(oc_task_t task);
// oc_err_t oc_task_signal_send(oc_task_t task,oc_int32_t signal);
// oc_err_t oc_task_signal_wait(oc_int32_t signal,oc_tick_t time);


void oc_task_delay_us(oc_uint32_t us);
void oc_task_delay_ms(oc_uint32_t ms);



oc_timer_t oc_timer_create(const char* name,\
                          void (*timeout)(void* parameter),\
                          void* parameter,\
                          oc_tick_t time,\
                          oc_timer_type flag);
oc_err_t oc_timer_delete(oc_timer_t timer);
oc_err_t oc_timer_start(oc_timer_t timer,oc_tick_t time);
oc_err_t oc_timer_stop(oc_timer_t timer);


#endif //__OC_H__
