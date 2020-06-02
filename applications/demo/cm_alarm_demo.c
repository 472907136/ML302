/*********************************************************
*  @file    cm_alarm_demo.c
*  @brief   ML302 OpenCPU alarm APIs example
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/11/6
********************************************************/
#include "main.h"
#define OPENCPU_ALARM ((unsigned)('O') | ((unsigned)('C') << 8) | ((unsigned)('A') << 16) | ((unsigned)('L') << 24))

#define SINGLE_ALARM_NAME  0
#define REPEATE_ALARM_NAME 1
void cm_alarm_cb(struct drvRtcAlarm *alarm, void *ctx)
{
    //cm_printf("owner:%d,index:%d\n",alarm->owner,alarm->name);
    if(alarm->name == REPEATE_ALARM_NAME)
    {
        cm_printf("[RP_ALARM]:EXPIRE\n");
    }
    if(alarm->name == SINGLE_ALARM_NAME)
    {
        cm_printf("[SG_ALARM]:EXPIRE\n");
    }
}

//如果要使用闹钟模块，必须在OpenCPU程序起始位置调用此函数
void  cm_test_alarm_init(void)
{
    drvRtcAlarmOwnerSetCB(OPENCPU_ALARM,0,cm_alarm_cb);
}
void cm_test_get_date(unsigned char **cmd,int len)
{
    struct l_tm t;
    
    time_to_date(osiEpochSecond()+cm_get_timezone()* 15*60,&t);
    cm_printf("[DATE]:%d-%d-%d:%d:%d:%d\n",t.tm_year,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
}
//测试单次的定时闹钟
void cm_test_single_time_alarm(unsigned char **cmd,int len)
{
    long curr = osiEpochSecond(); 
    drvRtcSetAlarm(OPENCPU_ALARM,SINGLE_ALARM_NAME,0,0,curr+char_to_int(cmd[2]),true);
    cm_printf("[SG_ALARM]:%ds\n",char_to_int(cmd[2]));
}
//测试某天某个时间的星期重复闹钟
void cm_test_set_repeate_alarm(unsigned char **cmd,int len)
{
    unsigned char day_mask = 0;// = (1)|(1<<6)|(1<<3); //星期天、星期六、星期三
    int i;
    int hour,min,sec;
    int total_sec;
    
    if((len > 10)||(len < 6)) //最多12个参数，最少6个参数，星期:时:分:秒
    {
        cm_printf("[RP_ALARM]param error\n");
        return;
    }
    for(i = 2;i< 2+len-5;i++)
    {
        day_mask |=(unsigned char) (1<< char_to_int(cmd[i]));
        cm_printf("[RP_ALARM]day %d set done\n",char_to_int(cmd[i]));
    }
    cm_printf("[RP_ALARM]day mask %d set done\n",day_mask);
    hour = char_to_int(cmd[len-3]);
    min = char_to_int(cmd[len-2]);
    sec = char_to_int(cmd[len-1]);
    total_sec = hour*60*60 + min*60 + sec;
    cm_printf("[RP_ALARM]hour:%d,min:%d,sec%d\n",hour,min,sec);
    drvRtcSetRepeatAlarm(OPENCPU_ALARM, REPEATE_ALARM_NAME, 0,0, day_mask,total_sec, cm_get_timezone() * 15*60, true);
}
void cm_test_get_repeate_alarm_info(unsigned char **cmd,int len)
{
    drvRtcAlarm_t alarms[2];
    memset(alarms,0,sizeof(drvRtcAlarm_t)*2);
    cm_printf("alarm count:%d\n",drvRtcGetAlarms(OPENCPU_ALARM, alarms, 2));
    cm_printf("index:%d,type:%d,mask:%d,expire:%d\n",alarms[0].name,alarms[0].type,alarms[0].wday_repeated,alarms[0].expire_sec);
    cm_printf("index:%d,type:%d,mask:%d,expire:%d\n",alarms[1].name,alarms[1].type,alarms[1].wday_repeated,alarms[1].expire_sec);
}