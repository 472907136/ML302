/*********************************************************
*  @file    cm_gnss_demo.c
*  @brief   ML302 OpenCPU GNSS demo file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by yuyangyi 2020/01/08
********************************************************/

#include "main.h"
#include "cm_gnss.h"

#ifdef CM_GNSS_SUPPORT

void cm_test_lbs(void)
{
    char *cellloc = NULL;
    cm_lbs_getlocation(&cellloc);
    cm_printf("LBS:%s\n", cellloc);
    cm_sys_log("LBS:%s", cellloc);

    return;
}

#endif