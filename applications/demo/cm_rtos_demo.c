/*********************************************************
*  @file    cm_rtos_demo.c
*  @brief   ML302 OpenCPU rtos example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/11/12
********************************************************/
#include "main.h"

static int cnt = 0;
static oc_timer_t timer;

static void timerHandler(void *parameter)
{
   cnt++;
   oc_printf("enter timer handler  cnt:%d\r\n",cnt);
   oc_timer_start(timer, 2000 );
}


void cm_test_os_timer()
{
    timer=oc_timer_create("timer",timerHandler,OC_NULL,2000,OC_TIMER_PERIODIC);
    if(timer!=OC_NULL)
    {
        oc_timer_start(timer,2000);
        oc_printf("timer start ok\r\n");   
    }
}