/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#ifndef __OC_CONFIG_H__
#define __OC_CONFIG_H__

#define OC_NAME_MAX 10
#define OC_TICK 10
#define OC_USING_HW_PIN
#define OC_USING_USART
#define OC_MAIN_UART HAL_UART_0

#define OC_DEBUG
#define OC_DEBUG_COLOR

#define OC_USING_SHELL

#define OC_USING_VIR_AT
#define OC_USING_AT_CLIENT
#define AT_USING_CLIENT
#define AT_CLIENT_NUM_MAX 1
#define AT_USING_CLI
#define AT_CMD_MAX_LEN 1100
#define AT_SW_VERSION_NUM 0x10300


#define CM_DEMO_SUPPORT
#define CM_HTTP_SUPPORT
#define CM_HTTPCLIENT_SSL_ENABLE
#define CM_HTTPS_SUPPORT
#define CM_EDP_SUPPORT
#define CM_DM_SUPPORT
#define CM_MQTT_SUPPORT
#define AT_MQTTSN_SUPPORT
#define CM_ALIYUN_SUPPORT
#endif //__OC_CONFIG_H__
