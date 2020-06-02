/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */
#include "oc.h"
#include "oc_dbg.h"
#include "cmsis_os.h"
#include "oc_device.h"


static void oc_show_version(void)
{
    oc_printf("     .__. __ .___.__..  ..__. __.\r\n");
    oc_printf("     |  |/  `[__ [__]|\\ ||  |(__ \r\n");
    oc_printf("     |__|\\__.[___|  || \\||__|.__)\r\n");
    oc_printf("\r\nWelcome to use Ocean OS\r\n");
    oc_printf("Build:       "__DATE__" "__TIME__"\r\n");
    oc_printf(" 2020 - 2030 Copyright by longmain team\r\n\r\n\r\n");
}


oc_err_t oc_init(void)
{
    oc_hw_device_init();
#ifndef OC_USING_SHELL
    oc_show_version();
#endif // OC_USING_USART
}





oc_task_t oc_task_create(const char* name,\
                void (*oc_pthread) (void *parameter),\
                void* parameter,\
                oc_uint32_t stack_size,\
                oc_uint8_t priority)
{
    oc_uint8_t size_len;
    oc_task_t task=OC_NULL;
    size_len=sizeof(struct oc_task);
    task=(oc_task_t)pvPortMalloc(size_len);
    if(task==OC_NULL)
    {
        LOG_E("task malloc failed\r\n");
        return OC_NULL;
    }
    task->init_priority=priority;
    task->current_priority=priority;
    memset(task->name,0,OC_NAME_MAX);
    strncpy(task->name,name,strlen(name));
    task->thread.name=task->name;
    task->thread.pthread=oc_pthread;
    task->thread.stacksize=stack_size;
    task->thread.tpriority=priority;
    task->thread.instances=0;
    task->thread.buffer=NULL;
    task->thread.controlblock=NULL;
    task->id=osThreadCreate(&task->thread,parameter);
    if(task->id==OC_NULL)
    {   
        LOG_E("thread cerate failed\r\n");
        vPortFree(task);
        return OC_NULL;
    }
    task->stat=OC_TASK_RUNNING;
    return task;
}


oc_task_handle_t oc_task_get_id(void)
{
  return osThreadGetId();
}


oc_err_t oc_task_delete(oc_task_t task)
{
    OC_ASSERT(task);
    oc_task_handle_t task_handle=task->id;
    vPortFree(task);
    if(osThreadTerminate(task_handle)==osOK)
        return OC_EOK;
    return OC_ERROR;
}


oc_err_t oc_task_yield(void)
{
    if(osThreadYield()==osOK)
        return OC_EOK;
    return OC_ERROR;
}

oc_err_t oc_task_set_priority(oc_task_t task, osPriority priority)
{
    OC_ASSERT(task);
    if(osThreadSetPriority(task->id,priority)==osOK)
    {
        task->current_priority=priority;
        return OC_EOK;
    }
    return OC_ERROR;
}
task_priority oc_task_get_priority(oc_task_t task)
{
    OC_ASSERT(task);
    return task->current_priority;
}
oc_err_t oc_task_suspend(oc_task_t task)
{
    OC_ASSERT(task);
    if(osThreadSuspend(task->id)==osOK)
    {
        task->stat=OC_TASK_SUSPEND;
        return OC_EOK;
    }
    return OC_ERROR;
}

oc_err_t oc_task_resume(oc_task_t task)
{
    OC_ASSERT(task);
    if(osThreadResume(task->id)==osOK)
    {
        task->stat=OC_TASK_RUNNING;
        return OC_EOK;
    }
    return OC_ERROR;
}
// oc_err_t oc_task_signal_send(oc_task_t task,oc_int32_t signal)
// {
//     if(osSignalSet(task->id,signal)!=0)
//     {
//         return OC_ERROR;
//     }
//     return OC_EOK;
// }

// oc_err_t oc_task_signal_wait(oc_int32_t signal,oc_tick_t time)
// {
//     if(osSignalWait(signal,OC_WAIT_FOREVER))



// }

void oc_task_delay_us(oc_uint32_t us)
{
    int i;
    i=us/1000;
    if(i)
    {
        osDelay(us/1000);
        cm_sys_delay_us(us%1000); 
    }
    else
    {
        cm_sys_delay_us(us);  
    }
}

void oc_task_delay_ms(oc_uint32_t ms)
{
    osDelay(ms);
}



oc_timer_t oc_timer_create(const char* name,\
                          void (*timeout)(void* parameter),\
                          void* parameter,\
                          oc_tick_t time,\
                          oc_timer_type flag)
{
    oc_timer_t timer=OC_NULL;
    timer=(oc_timer_t)pvPortMalloc(sizeof(struct oc_timer));
    if(timer==OC_NULL)
    {
        LOG_E("timer(%s) create failed\r\n",name);
        return OC_NULL;
    }
    
    memset(timer->name,0,OC_NAME_MAX);
    strncpy(timer->name,name,strlen(name));
    timer->init_tick=1;
    timer->opt.ptimer=timeout;
    timer->timeout_tick=time;
    timer->opt.controlblock=OC_NULL;
    timer->id=osTimerCreate(&timer->opt,flag,parameter);
    if(timer->id==OC_NULL)
    {
        LOG_E("thread cerate failed\r\n");
        vPortFree(timer);
        return OC_NULL;
    }

    return timer;
}


oc_err_t oc_timer_delete(oc_timer_t timer)
{
    OC_ASSERT(timer);
    if(osTimerDelete(timer->id)==osOK)
    {
        vPortFree(timer);
        return OC_EOK;
    }
    LOG_E("timer(%s) delete failed\r\n",timer->name);
    return OC_ERROR;
}

oc_err_t oc_timer_start(oc_timer_t timer,oc_tick_t time)
{
    OC_ASSERT(timer);
    if(time==OC_NULL)
    {
        if(osTimerStart(timer->id,timer->timeout_tick)!=osOK)
        {
            LOG_E("timer(%s) start failed\r\n",timer->name);
            return OC_ERROR;
        }
    }
    else
    {
        if(osTimerStart(timer->id,time)!=osOK)
        {
            LOG_E("timer(%s) start failed\r\n",timer->name);
            return OC_ERROR;
        }
    }
    

    return OC_EOK;
}

oc_err_t oc_timer_stop(oc_timer_t timer)
{
    OC_ASSERT(timer);
    if(osTimerStop(timer->id)!=osOK)
    {
        LOG_E("timer(%s) stop failed\r\n",timer->name);
        return OC_ERROR;
    }
    return OC_EOK;
}





