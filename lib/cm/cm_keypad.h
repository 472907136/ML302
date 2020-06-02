/*********************************************************
*  @file    cm_keypad.h
*  @brief   ML302 keypad  header File
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by WangPeng 2020/03/25
********************************************************/
#ifndef __CM_KEYPAD_H__
#define __CM_KEYPAD_H__

typedef enum
{
    KEY_STATE_PRESS = (1 << 0),
    KEY_STATE_RELEASE = (1 << 1),
} keyState_t;

typedef uint32_t keyMap_t;
typedef void (*cm_key_event_callback_t)(keyMap_t key, keyState_t evt);

/**
 *  \brief keypad keypad初始化
 *  \param [in] cb 回调函数
 *  \return ：无
 *  
 *  \details More details
 */
void cm_keypad_init(cm_key_event_callback_t cb);

#endif
