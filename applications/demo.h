/*********************************************************
*  @file    onemo_main.h
*  @brief   ML302 OpenCPU main header file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/08
********************************************************/
#ifndef __DEMO_H__
#define __DEMO_H__

void cm_test_lcd(unsigned char **cmd,int len);
void cm_test_cjson(unsigned char **cmd,int len);
void cm_test_sha(unsigned char **cmd,int len);
void cm_test_base64(unsigned char **cmd,int len);
void cm_test_md5(unsigned char **cmd,int len);
void cm_printf(char *format,...);
void cm_test_udp(unsigned char **cmd,int len);
void cm_test_tcpclient_send(unsigned char **cmd,int len);
void cm_test_tcpclient(unsigned char **cmd,int len);
void cm_test_gpio_write(unsigned char **cmd,int len);
void cm_test_gpio_read(unsigned char **cmd,int len);
void cm_test_gpio_irq(unsigned char **cmd,int len);
void cm_force_download(unsigned char **cmd,int len);
void cm_test_i2c_bmp180(unsigned char **cmd,int len);
void cm_test_uart_init(void);
void cm_test_keypad_init(void);
void cm_test_alarm_init(void);
void cm_test_single_time_alarm(unsigned char **cmd,int len);
void cm_test_set_repeate_alarm(unsigned char **cmd,int len);
void cm_test_get_repeate_alarm_info(unsigned char **cmd,int len);
void cm_test_adc(unsigned char **cmd,int len);
void cm_test_vbat(void);
void cm_test_send_at(unsigned char **cmd,int len);
void cm_test_dump_at(unsigned char **cmd,int len);
void cm_test_reboot(unsigned char **cmd,int len);
#ifdef  CM_HTTP_SUPPORT
void cm_test_http(unsigned char **cmd,int len);
#endif
#ifdef  CM_EDP_SUPPORT
void cm_test_edp(unsigned char **cmd,int len);
#endif
#ifdef  CM_ALIYUN_SUPPORT
void cm_test_aliyun(unsigned char **cmd,int len);
#endif
#ifdef  CM_DM_SUPPORT
void cm_test_dm_register(unsigned char **cmd,int len);
#endif
#ifdef  CM_MQTT_SUPPORT
int cm_test_mqtt(unsigned char **cmd,int len) ;
#endif

#ifdef  CM_FOTA_SUPPORT
void cm_test_onenet_fota_setdevinfo(void);
void cm_test_onenet_fota(void);
void cm_test_onenet_fota_deletetask(void);
void cm_test_fota_update(unsigned char **cmd,int len);
#endif

#ifdef CM_GNSS_SUPPORT
void cm_test_lbs(void);
#endif

#ifdef CM_AUDIO_SUPPORT
void cm_test_audio(unsigned char **cmd,int len);
#endif
void cm_test_tts(unsigned char **cmd,int len);
void cm_test_write_file(unsigned char **cmd,int len);
void cm_test_read_file(unsigned char **cmd,int len);
void cm_test_get_date(unsigned char **cmd,int len);

#endif

