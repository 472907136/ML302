/*********************************************************
*  @file    cm_keypad_demo.c
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by WangPeng 2020/03/31
********************************************************/
#include "main.h"
#include "cm_keypad.h"

void cm_keypad_callback(keyMap_t id, keyState_t evt)
{
    /* 用户在此处实现逻辑，本初为Demo示例，使用串口打印keypad事件*/
    cm_printf("key id %d,status %d\n", id, evt);
}

void cm_test_keypad_init()
{
    cm_keypad_init(cm_keypad_callback);
}

