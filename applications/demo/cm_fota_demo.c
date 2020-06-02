/*********************************************************
*  @file    cm_fota_demo.c
*  @brief   ML302 OpenCPU FOTA demo file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by yuyangyi 2020/01/08
********************************************************/

#include "main.h"
#include "cm_fota.h"

#ifdef CM_FOTA_SUPPORT

void onError_cb(int error_code)
{
    cm_printf("+FOTA ERROR:%d\n",error_code);
    return;
}

void onEvent_cb(int state, int event)
{
    cm_printf("+FOTA:%d,%d\n", state, event);
    return;
}

void onDownload_cb(int state, int event, int current, int total, int percent)
{
    cm_printf("+FOTA:%d,%d,%d,%d,%d\n", state, event, current, total, percent);
    return;
}

void onVersion(char **version)
{
    *version = "1.2";  //用户输入当前版本号
    return;
}

//请根据实际情况修改如下信息
#define product_ID ("322377")   //产品ID，OneNet平台网页查询
#define APIkey ("AuHlEw==h939Ny9=JQVCNxJvCdk=")		//Master-APIkey，OneNet平台网页查询
#define access_key ("ZxaQ4Rca3/yxfaHoHCt6h2cxR985eNAI56dbboPHZ0M=")   //access_key，OneNet平台网页查询
void cm_test_onenet_fota_setdevinfo(void)
{
    cm_onenet_fota_regcbex();
    
    cm_onenet_fota_setdevinfo(product_ID, APIkey, access_key);

    unsigned char global_imei[30]= {0};
    unsigned char *p1,*p2;
    cm_clear_virt_at_buffer();
    cm_vir_at_send("AT+CGSN=1\r\n");
    p1= cm_check_at_str("CGSN",10);
    if(p1 == 0)
    {
        cm_printf("imei read error\n");
        return;
    }        
    if(virt_at_len < 30) 
    {
        cm_printf("imei read error");
    }
    p2 = strstr(p1+10,"\r\n");
    *p2 =  0;
    strcpy(global_imei,p1+10);

    cm_sys_log("global_imei :%s", global_imei);
    
    if(!cm_onenet_fota_getdevid(global_imei))
    {
        cm_printf("\nonenet fota setdevinfo error\n");
    }

    return;
}

void cm_onenet_fota_regcbex()
{
    oc_cb_t callback;
    callback.onError = onError_cb;
    callback.onEvent = onEvent_cb;
    callback.onDownload = onDownload_cb;
    callback.onVersion = onVersion;
    cm_onenet_fota_regcb(&callback);

    return;
}

void cm_test_onenet_fota(void)
{
    cm_onenet_fota_regcbex();

    cm_sys_log("cm_test_onenet_fota");
    if(!cm_onenet_fota_exe())
    {
        cm_printf("\nonenet fota error\n");
    }

    return;
}

void cm_test_onenet_fota_deletetask(void)
{
    cm_onenet_fota_deletetask();

    cm_sys_log("cm_test_onenet_fota_deletetask");
    return;
}

void cm_test_fota_update(unsigned char **cmd,int len)
{
    cm_sys_log("cm_test_fota_update");
    cm_fota_saverom(cmd[2], char_to_int(cmd[3]));
    cm_fota_update();
    
    return;
}

#endif