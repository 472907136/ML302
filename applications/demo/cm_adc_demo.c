/*********************************************************
*  @file    cm_adc_demo.c
*  @brief   ML302 OpenCPU ADC APIs header
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/21
********************************************************/
#include "main.h"


void cm_test_adc(unsigned char **cmd,int len)
{
    if(strcmp(cmd[2],"1") == 0)
        cm_printf("[ADC][%d]:%d\n",1,cm_adc_get_vol(CM_ADC_CHANNEL_1));
    else
        cm_printf("[ADC][%d]:%d\n",2,cm_adc_get_vol(CM_ADC_CHANNEL_2));
}

void cm_test_vbat(void)
{
    cm_printf("[vbat]:%d\n",cm_get_vbat(CM_ADC_CHANNEL_2));
}