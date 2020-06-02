/*********************************************************
*  @file    cm_main.c
*  @brief   ML302 OpenCPU Entry File
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/08
********************************************************/

#include "main.h"

oc_task_t main_task_handle;

void oc_main_task(void *p)
{  
    unsigned char buf[50] = {0};
    int i;
    struct l_tm t;
    oc_init();
    oc_printf("ML302 OpenCPU Starts\r\n");
    
    oc_printf("Boot Cause:%d\r\n",cm_sys_get_boot_cause());
    cm_sys_get_sdk_swver(buf,50);
    oc_printf("SDK VERSION:%s\r\n",buf);
    cm_sys_get_base_swver(buf,50);
    oc_printf("BASELINE VERSION:%s\r\n",buf);
    cm_sys_get_hwver(buf,50);
    oc_printf("HW VERSION:%s\r\n",buf);  
    oc_printf("waiting for network...\r\n");
    cm_test_network_config();
    time_to_date(osiEpochSecond()+cm_get_timezone()* 15*60,&t);
    oc_printf("Now:%d-%d-%d:%d:%d:%d\r\n",t.tm_year,t.tm_mon,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    oc_printf("will enter get module fun\r\n");
    cm_test_get_imei();
    cm_test_get_imsi();
    cm_test_get_iccid();
    
    #ifdef cm_FOTA_SUPPORT   //启用OneNet FOTA不可删除
    char *version = NULL;
    onVersion(&version);
    oc_printf("OneNet FOTA version:%s\n", version);
    #endif
    while(1)
    {
        oc_task_delay_ms(1000);
    }
}


void ML302_OpenCPU_Entry()
{
    cm_test_alarm_init();
    main_task_handle=oc_task_create("main_task",oc_main_task,OC_NULL,8192,OC_TASK_PRIORITY_NORMAL);

#ifdef cm_FOTA_SUPPORT   //启用OneNet FOTA不可删除
    cm_onenet_fota_regcbex();
#endif
}
//SDK中调用，不可删除
int cm_wdt_config_cb(void)
{
    return 0;//关闭看门狗
    //return 1;//开启看门狗
}