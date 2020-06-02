/*********************************************************
*  @file    cm_sys_demo.c
*  @brief   ML302 OpenCPU network example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/15
********************************************************/
#include "main.h"
#include "oc_device.h"
unsigned char global_imei[30]= {0};
unsigned char global_imsi[30] = {0};
unsigned char global_iccid[30] = {0};
void cm_force_download(unsigned char **cmd,int len)
{
    oc_printf("[FORCEDL]go to download mode\n");
    oc_vir_at_send_cmd("AT^FORCEDNLD\r\n");
}
void cm_test_get_imei()
{
    unsigned char *p1,*p2;
    oc_vir_at_send_cmd("AT+CGSN=1\r\n");
    p1= oc_vir_at_check_str("CGSN");
    if(p1 == 0)
    {
        oc_printf("imei read error\n");
        return;
    }        
    p2 = strstr(p1+10,"\r\n");
    *p2 =  0;
    strcpy(global_imei,p1+10);
    oc_printf("IMEI:%s\n",global_imei);
}
void cm_test_get_imsi()
{
    unsigned char *p1,*p2;
    oc_vir_at_send_cmd("AT+CIMI\r\n");
    p1= oc_vir_at_check_str("CIMI");
    if(p1 == 0)
    {
        oc_printf("imsi read error\n");
        return;
    }        
    p2 = strstr(p1+8,"\r\n");
    *p2 =  0;
    strcpy(global_imsi,p1+8);
    oc_printf("IMSI:%s\n",global_imsi);
}



void cm_test_get_iccid()
{
    unsigned char *p1,*p2;
    oc_vir_at_send_cmd("AT+CCID\r\n");
    p1= oc_vir_at_check_str("+CCID:");
    if(p1 == 0)
    {
        oc_printf("iccid read error\n");
        return;
    }        
    p2 = strstr(p1+7,"\r\n");
    *p2 =  0;
    strcpy(global_iccid,p1+7); 
    oc_printf("ICCID:%s\n",global_iccid);
}

void cm_test_reboot(unsigned char **cmd,int len)
{
    
    oc_printf("[REBOOT]rebooting...\n");
    cm_sys_reboot();   
}
