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



unsigned char virt_at_buffer[2048];
extern int virt_at_len;
extern void _drv_vir_at_clear_buffer(void);
extern unsigned char * _drv_vir_at_check_str(const char *str,int delay);
// void vir_at_init(void);

int oc_vir_at_send_cmd(oc_uint8_t *at_str )
{
    _drv_vir_at_clear_buffer();
   return cm_vir_at_send(at_str);
}
char * oc_vir_at_check_str(const char *str)
{
    int i,j,delay=25; 
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
    return OC_NULL;
}

void oc_vir_at_dump(void)
{
    oc_uint32_t i;
    for (i = 0; i < virt_at_len; i++)
    {
        oc_printf("%c", virt_at_buffer[i]);
    }
}
int oc_vir_at_exec(int argc, char *agrv[])
{
    oc_uint8_t p1;
    unsigned char p[100] = {0};
    if (argc != 2)
    {
        LOG_E("error, argc = %d, usage: at_exec AT", argc);
        return OC_ERROR;
    }
    else
    {
        sprintf(p, "%s\r\n", agrv[1]);
        oc_vir_at_send_cmd(p);
        oc_vir_at_check_str("OK");
        oc_vir_at_dump();   
      
    }
    return OC_EOK;
}

#include "shell.h"

SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN),at_exec,oc_vir_at_exec,at_cmd_send);
