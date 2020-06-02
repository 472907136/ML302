/*********************************************************
*  @file    onemo_vir_at_demo.c
*  @brief   ML302 OpenCPU virture AT channel example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/08
********************************************************/
#define DBG_TAG "vir_at"
#define DBG_LVL DBG_LOG
#include "oc_dbg.h"
#include "shell.h"
#include "main.h"
extern unsigned char virt_at_buffer[2048];
extern int virt_at_len = 0;
osThreadId DRV_AT_TaskHandle;

osMutexDef( DRV_VIR_AT_Mutex );
osMutexId drv_vir_at_mutex;

void _drv_vir_at_clear_buffer(void)
{
    osMutexWait( drv_vir_at_mutex, osWaitForever );
    memset(virt_at_buffer,0,virt_at_len);
    virt_at_len = 0;
    osMutexRelease( drv_vir_at_mutex );
}
unsigned char * _drv_vir_at_check_str(const char *str,int delay)
{
    int i,j; 
    unsigned char *p;
    j = 0;
    while(j<delay)
    {
        osDelay(100);
        j++;
        if(virt_at_len > 0)
        {
            for(i = 0; i < virt_at_len; i++) //将有效长度内所有含0的地方替换成'!'，以方便进行字符串检测
            {
                if(virt_at_buffer[i] == 0)
                {
                    virt_at_buffer[i] = '!';
                }
            }
            virt_at_buffer[virt_at_len] = 0;
            p = strstr((const char *)virt_at_buffer,str);
            if(p != 0)
                return p;
        }
    }
    return 0;
}
static void virture_at_task()
{
    while(1)
    {
        osSignalWait(0x0004, osWaitForever);
        if(virt_at_len >=2048)
        {
            osMutexWait( drv_vir_at_mutex, osWaitForever );
            virt_at_len = 0;
            osMutexRelease( drv_vir_at_mutex );
        }
        osMutexWait( drv_vir_at_mutex, osWaitForever );
        virt_at_len += cm_vir_at_get(virt_at_buffer+virt_at_len, 2048-virt_at_len);
        osMutexRelease( drv_vir_at_mutex );
    }   
}
                             

void _drv_vir_at_dump(int argc, char *agrv[])
{
    int i;
    oc_printf("enter send_at\n");
    LOG_I("[DUMP_AT]:Start\n");
    for (i = 0; i < virt_at_len; i++)
    {
        onemo_printf("%c", virt_at_buffer[i]);
    }
}

/*
    虚拟AT回调函数，当有AT命令返回或者URC消息时，此函数会被调用
*/
void vir_at_sys_callback(void *param)
{
    osSignalSet(DRV_AT_TaskHandle, 0x0004);
}
void oc_hw_vir_at_init(void)
{
    drv_vir_at_mutex = osMutexCreate(osMutex(DRV_VIR_AT_Mutex));
    osThreadDef(DRV_AT_Task, virture_at_task, osPriorityNormal, 0, 1024);
    DRV_AT_TaskHandle = osThreadCreate(osThread(DRV_AT_Task), 0);
    cm_vir_at_init(vir_at_sys_callback);
}
