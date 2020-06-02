/*********************************************************
*  @file    cm_mqtt_demo.c
*  @brief   ML302 DM example File
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by ZhangTianxing 2019/11/28
********************************************************/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "CM_MQTTClient.h"

#ifdef CM_MQTT_SUPPORT

#define CMMQTT_MAX_TIMES 100
#define CMMQTT_CHECK_NORMAL_TIMES 100

CMQTT_ERR_STAT cmmqtt_err_stat;
CMMQTT_Mutex *g_pubMutex = NULL;

static int connflag = 0;
volatile u32_t cmmqtt_max_connecttimes = 3;

static char *cmmqtt_glob_subtopic = "testabcdefg";  //此处填入sub主题
static int cmmqtt_glob_subqos = 1;

static char *cmmqtt_glob_pubtopic = "testabcdefg";  //此处填入pub主题
static int cmmqtt_glob_pubqos = 1;
static int cmmqtt_glob_pubretain = 0;
static int cmmqtt_glob_pubdup = 0;
static char *cmmqtt_glob_pubmessage = " !?#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvw xyz{|}~end1234";
static char cmqtt_glob_cfgserver[129] = "testabcdefg";  //此处填入服务器地址
static int cmqtt_glob_cfgport = 6688;   //此处填port号，非加密
static int cmqtt_glob_tls_cfgport = 16688;   //此处填port号，加密
static char cmqtt_glob_cfgclient_id[129] = "11111";   //此处填client_id，加密
static int cmqtt_glob_cfgkeepAlive = 60;

static char cmqtt_glob_cfguser[129] = "test";    //此处填用户名

static char cmqtt_glob_cfgpasswd[129] = "test";	   //此处填密码
static int cmqtt_glob_cfgclean = 1;

static char *cmqtt_glob_cfgversion = "2018-10-31";
static int cmqtt_glob_cfgencrypt = 0;

typedef struct mqtt_CPUB
{
	unsigned int qos;
	unsigned int retain;
	unsigned int dup;
	unsigned int state;
	int mqtt_error;
	char *topic;
}mqtt_CPUB;

static mqtt_CPUB mqtt_cpub_tmp;

extern CMMQTT_PUBCB cmmqtt_recpubcb[CMMQTT_PUBCB_NUM];

/*+MQTTCFG="183.230.40.39",6002,"541100010",100,"269374","dOELqt6s35CMKigZg117glPO=kU=",0*/

int cm_test_mqtt(unsigned char **cmd,int len) 
{
   	CMMQTTClient *myclient = cmmqtt_getclient();
    int cmmqtt_check_times = 0;
    int cmmqtt_check_times_1 = 0;
	int i = 0;
	
	if(char_to_int(cmd[2]) == 1)
	{
		cmqtt_glob_cfgencrypt = 1;
		cm_printf("[MQTT]Encrypt\n");
	}
	if(char_to_int(cmd[2]) == 0)
	{
		cmqtt_glob_cfgencrypt = 0;
		cm_printf("[MQTT]Not Encrypt\n");
	}


    int bufsize = 512;
	int connect_flag = -1;

	cm_printf("\r\n[MQTT]CM MQTT TEST BEGAIN!!!\r\n");
    
	

	if(mqtt_con_demo() == 0)
	{
		cm_printf("[MQTT]MQTTCONNECT: OK\r\n");
		connect_flag = 1;
	}
	else
	{
		cm_printf("[MQTT]MQTTCONNECT: ERROR\r\n");
		connect_flag = -1;
	}
	osDelay(1000);

	mqtt_state_demo();
	
	if(connect_flag == 1)
	{
		if(mqtt_sub_demo() == 0)
		{
			cm_printf("[MQTT]MQTTSUB: OK\r\n");
		}
		else
		{
			cm_printf("[MQTT]MQTTSUB: ERROR\r\n");
		}
		osDelay(1000);
		
		if(mqtt_pub_demo() == 0)
		{
			cm_printf("[MQTT]MQTT PUB: OK\r\n");
		}
		else
		{
			cm_printf("[MQTT]MQTT PUB: ERROR\r\n");
		}
		osDelay(5000);
	
		if(mqtt_unsub_demo() == 0)
		{
			cm_printf("[MQTT]MQTTUNSUB TOPIC: OK\r\n");
		}
		else
		{
			cm_printf("[MQTT]MQTTUNSUB TOPIC: ERROR\r\n");
		}
		
		osDelay(1000);
		if(0 == mqtt_disc_demo())
		{
			cm_printf("[MQTT]MQTTDISC: OK\r\n");
		}
		else
		{
			cm_printf("[MQTT]MQTTDISC: ERROR\r\n");
		}

	}
	
	if(mqtt_del_demo() == 0)
	{	
		cm_printf("[MQTT]MQTTDEL: OK\r\n");
	}
	else
	{
		cm_printf("[MQTT]MQTTDEL: ERROR\r\n");
	}

	cm_printf("\r\n[MQTT]CM MQTT TEST END!!!\r\n");

}


void mqtt_state_demo()
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	uint8 mystat = 0;
	 
	//TODO:Defined by the macro of different module version,such as the M5311
	switch(myclient->state)//To adapt to the M8321 state value.
	{
		case CONNECTED:
			cm_printf("[MQTT]MQTTSTAT: CONNECTED\r\n");
			break;
		
		case NET_CONNECTING:
			cm_printf("[MQTT]MQTTSTAT: NET_CONNECTING\r\n");
			break;
		case RECONNECTING:
			cm_printf("[MQTT]MQTTSTAT: RECONNECTING\r\n");
			break;

		case NET_CONNECTED:
			cm_printf("[MQTT]MQTTSTAT: NET_CONNECTED\r\n");
			break;

		case CONNECTING:
			cm_printf("[MQTT]MQTTSTAT: CONNECTING\r\n");
			break;

		default:
			cm_printf("[MQTT]MQTTSTAT: UNINITIALED\r\n");
			break;
	}

 }


int mqtt_con_demo()
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	int cmmqtt_check_times = 0;
	int cmmqtt_check_times_1 = 0;
	int i = 0;

	char *client_id = cmqtt_glob_cfgclient_id;

	char *user = cmqtt_glob_cfguser;

	char *passwd = cmqtt_glob_cfgpasswd;


	char *version = cmqtt_glob_cfgversion;

	char *ca = "";

	char *server = cmqtt_glob_cfgserver;
	
	
	int keepAlive = cmqtt_glob_cfgkeepAlive;
	
	int clean = cmqtt_glob_cfgclean;
	
	int bufsize = 512;
	
	int encrypt = cmqtt_glob_cfgencrypt;


    int64_t expire_time = 4102416000;
    
    int mqtt_error = CMMQTT_AT_OK;

    
    int mqtt_error1;
	int port;
	if(cmqtt_glob_cfgencrypt ==0)
		port = cmqtt_glob_cfgport;
	else
		port = cmqtt_glob_tls_cfgport;
    
    if(myclient->state == CONNECTED)
    {
        cm_printf("[MQTT][%s][%d] ERROR OneNET MQTT ALREADY CONNECTED\r\n",__func__,__LINE__);
		return 0;
    }
    
    mqtt_error1 = cmmqtt_init_cfg(server, port, client_id, keepAlive * 1000, user, passwd, clean, bufsize, encrypt);
    if(0 > mqtt_error1)
    {
        cm_printf("[MQTT][%s][%d] ERROR cmmqtt_init_cfg error!\r\n",__func__,__LINE__);
        return 1;
    }
    
    
    
    //AT+MQTTOPEN=1,1,0,0,0,"",""
    int user_flag = 1;
    
    int pwd_flag = 1;
    
    int will_flag = 0;
    
    if(myclient->state == INITIALED)
    {
        CMMQTTConmsg *conmsg = (CMMQTTConmsg *)cmmqtt_malloc(sizeof(CMMQTTConmsg));
        memset(conmsg,0,sizeof(CMMQTTConmsg));
        if(user_flag == 1)
        {
            if(strlen((char*)myclient->usr) > 0)
                conmsg->connect_data.username.cstring = myclient->usr;
            else
                mqtt_error = CMMQTT_AT_PARAMETER_ERROR;
            
        }
        if(pwd_flag == 1)
        {
            if(strlen((char*)myclient->password) > 0)
                conmsg->connect_data.password.cstring = myclient->password;
            else
                mqtt_error = CMMQTT_AT_PARAMETER_ERROR;
        }
        conmsg->connect_data.MQTTVersion = 4;
        conmsg->connect_data.clientID.cstring = myclient->client_id;
        conmsg->connect_data.keepAliveInterval = myclient->keepAliveInterval/1000;
        conmsg->connect_data.cleansession = myclient->cleansession;
        conmsg->connect_data.willFlag = will_flag;

        conmsg->queueout_ackcb = cmmqtt_conack_cb;
        if(myclient->cmmqtt_conmsg != NULL)
        {
            cmmqtt_free_conmsg(myclient->cmmqtt_conmsg);
            myclient->cmmqtt_conmsg = NULL;
        }
        myclient->cmmqtt_conmsg = conmsg;
        myclient->connectnum = 1;

        
        if(mqtt_error == CMMQTT_AT_OK)
        {
            cmmqtt_err_stat.cmmqtt_connect_error = 0;
            myclient->state = NET_CONNECTING;
            
            while(cmmqtt_check_times < CMMQTT_MAX_TIMES)
            {
                if(CMMQTT_CONMQTTSERVER_OK == cmmqtt_err_stat.cmmqtt_connect_error)
                {
                    while(cmmqtt_check_times_1 < CMMQTT_MAX_TIMES)
                    {
                        if(cmmqtt_err_stat.cmmqtt_response_error == CMMQTT_CMD_RESPONSE_FAIL)
                        {	
                            cm_printf("[MQTT][%s][%d] ERR_MQTT_UNKNOWN_DATA\r\n",__func__,__LINE__);
							return -1;
                        }
                        
                        switch(cmmqtt_err_stat.cmmqtt_conpasswd_error)
                        {
                            case CMMQTT_CONPASSWD_OK:
                                return 0;

                            case CMMQTT_CONPASSWD_FAIL:
                                cm_printf("[MQTT][%s][%d] ERR_MQTT_USERPASSWORD_ERR\r\n",__func__,__LINE__);
								return -1;

                            case -1:
                                cm_printf("[MQTT][%s][%d] ERR_MQTT_CONNECT_FAIL\r\n",__func__,__LINE__);
								return -1;
                        }

                        cmmqtt_check_times_1++;
                        osDelay(50);
                    }
                }
                else if(CMMQTT_CONMQTTSERVER_FAIL == cmmqtt_err_stat.cmmqtt_connect_error)
                {	
                    cm_printf("[MQTT][%s][%d] ERR_MQTT_CONNECT_FAIL\r\n",__func__,__LINE__);
					return -1;
                }
                else if(CMMQTT_CONMQTTSERVER_TIMEOUT == cmmqtt_err_stat.cmmqtt_connect_error)
                {	
                    cm_printf("[MQTT][%s][%d] ERR_MQTT_CONNECT_TIMEOUT\r\n",__func__,__LINE__);
					return -1;
                }
                else if(CMMQTT_SENDPACK_ERROR == cmmqtt_err_stat.cmmqtt_connect_error)
                {
                    cm_printf("[MQTT][%s][%d] ERR_MQTT_SENDPACK_FAIL\r\n",__func__,__LINE__);
					return -1;
                }
                cmmqtt_check_times++;
                osDelay(50);
            }
        }
    
        return 0;
    }
}


int mqtt_pub_demo()
{
	int cmmqtt_check_times = 0;
	unsigned int msg_len = 0;
	CMMQTTClient *myclient = cmmqtt_getclient();

	char *msg = malloc(strlen(cmmqtt_glob_pubmessage));
	unsigned int pub__str_size = strlen(cmmqtt_glob_pubmessage);
	MEMCPY(msg, cmmqtt_glob_pubmessage, strlen(cmmqtt_glob_pubmessage));
	
	mqtt_cpub_tmp.qos = cmmqtt_glob_pubqos;
	mqtt_cpub_tmp.mqtt_error = CMMQTT_AT_OK;
	mqtt_cpub_tmp.dup = cmmqtt_glob_pubdup;
	mqtt_cpub_tmp.retain = cmmqtt_glob_pubretain;
	mqtt_cpub_tmp.topic = cmmqtt_glob_pubtopic;
		
	if(myclient->state == CONNECTED)
	{
		CMMQTTPubmsg *pubmsg = (CMMQTTPubmsg *)cmmqtt_malloc(sizeof(CMMQTTPubmsg));
		pubmsg->topicName.lenstring.len = 0;
		pubmsg->topicName.lenstring.data = NULL;
		pubmsg->topicName.cstring = (char*)cmmqtt_malloc(strlen(mqtt_cpub_tmp.topic) + 1);
		if(NULL == pubmsg->topicName.cstring)
		{
			if(NULL != msg)
			{
				free(msg);
				msg = NULL;
			}
			cm_printf("[MQTT][%s][%d] ERR_AT_CME_NO_MEMORY\r\n",__func__,__LINE__);
			return -1;
		}
		memset(pubmsg->topicName.cstring,0,strlen(mqtt_cpub_tmp.topic) + 1);
		strcpy(pubmsg->topicName.cstring,mqtt_cpub_tmp.topic);
		pubmsg->dup = mqtt_cpub_tmp.dup;
		pubmsg->qos = mqtt_cpub_tmp.qos;
		pubmsg->retained = mqtt_cpub_tmp.retain;
		if(msg_len == 0)// text mode
		{
			pubmsg->payloadlen = pub__str_size;
			pubmsg->payload = (unsigned char *)cmmqtt_malloc(pubmsg->payloadlen+1);
			memset(pubmsg->payload,0,pubmsg->payloadlen+1);
			memcpy(pubmsg->payload,msg,pubmsg->payloadlen);
			if(NULL != msg)
			{
				free(msg);
				msg = NULL;
			}
		}
		else // hex mode
		{
            //é•¿åº¦å¿…é¡»åŒ¹é…
            if(msg_len * 2 != strlen(msg))
            {
				cmmqtt_free_pubmsg(pubmsg);
				return -1;
            }
			pubmsg->payloadlen = msg_len;
			pubmsg->payload = (unsigned char *)cmmqtt_malloc(pubmsg->payloadlen + 1);
			cmmqtt_hex_to_bin(pubmsg->payload,msg,pubmsg->payloadlen);
			if(NULL != msg)
			{
				free(msg);
				msg = NULL;
			}
		}
		int i = 0;
		for(i = 0;i < 3;i++)
		{
			cmmqtt_err_stat.cmmqtt_pub_error[i] = 0;
		}
		pubmsg->queueout_ackcb = cmmqtt_puback_cb;
      	mqtt_cpub_tmp.mqtt_error = cmmqtt_publish(pubmsg);
		if(mqtt_cpub_tmp.qos == 0 || mqtt_cpub_tmp.mqtt_error != CMMQTT_AT_OK)
		{
			cmmqtt_free_pubmsg(pubmsg);
		}
		
		if(mqtt_cpub_tmp.mqtt_error != CMMQTT_AT_OK)
		{
			cm_printf("[MQTT][%s][%d] ERR_MQTT_SENDPACK_FAIL\r\n",__func__,__LINE__);				
			return -1;
		}
		
		if(mqtt_cpub_tmp.qos == QOS1)//QoS 1
		{
		
			if(mqtt_cpub_tmp.mqtt_error == CMMQTT_AT_OK)
			{
				while(cmmqtt_check_times < CMMQTT_CHECK_NORMAL_TIMES)
				{
					if(cmmqtt_err_stat.cmmqtt_response_error == CMMQTT_CMD_RESPONSE_FAIL)
					{	
						cm_printf("[MQTT][%s][%d] ERR_MQTT_UNKNOWN_DATA\r\n",__func__,__LINE__);				
						return -1;
					}
					if(cmmqtt_err_stat.cmmqtt_pub_error[0] == CMMQTT_PUBACK_FAIL)
					{
						cm_printf("[MQTT][%s][%d] ERR_MQTT_PUBACK_FAIL\r\n",__func__,__LINE__);				
						return -1;
					}
					else if(cmmqtt_err_stat.cmmqtt_pub_error[0] == CMMQTT_PUB_OK)
					{
						//cm_printf("[%s][%d] pub OK\r\n",__func__,__LINE__);				
						return 0;
					}
				
					cmmqtt_check_times++;
					osDelay(50);
				}
			}
		}

		if(mqtt_cpub_tmp.qos == QOS2)
		{
			if(mqtt_cpub_tmp.mqtt_error == CMMQTT_AT_OK)
			{
					while(cmmqtt_check_times < CMMQTT_CHECK_NORMAL_TIMES)
					{
				
						if(cmmqtt_err_stat.cmmqtt_response_error == CMMQTT_CMD_RESPONSE_FAIL)
						{	
							cm_printf("[MQTT][%s][%d] ERR_MQTT_UNKNOWN_DATA\r\n",__func__,__LINE__);				
							return -1;
						}

						if(((cmmqtt_err_stat.cmmqtt_pub_error[0]) == CMMQTT_PUBACK_FAIL ||
							(cmmqtt_err_stat.cmmqtt_pub_error[1]) == CMMQTT_PUBACK_FAIL ||
							(cmmqtt_err_stat.cmmqtt_pub_error[2]) == CMMQTT_PUBACK_FAIL)||
							((cmmqtt_err_stat.cmmqtt_pub_error[0]) == CMMQTT_PUB_OK &&
							(cmmqtt_err_stat.cmmqtt_pub_error[1]) == CMMQTT_PUB_OK &&
							(cmmqtt_err_stat.cmmqtt_pub_error[2]) == CMMQTT_PUB_OK))
						{
							break;
						}

						cmmqtt_check_times++;
						osDelay(50);
					}

					if(cmmqtt_err_stat.cmmqtt_pub_error[0] == CMMQTT_PUBACK_FAIL)
					{
 						cm_printf("[MQTT][%s][%d] ERR_MQTT_PUBREC_FAIL\r\n",__func__,__LINE__);				
						return -1;
					}
					
					if(cmmqtt_err_stat.cmmqtt_pub_error[1] == CMMQTT_PUBACK_FAIL)
					{
  						cm_printf("[MQTT][%s][%d] ERR_MQTT_SENDPACK_FAIL\r\n",__func__,__LINE__);				
						return -1;
					}

					if(cmmqtt_err_stat.cmmqtt_pub_error[2] == CMMQTT_PUBACK_FAIL)
					{
   						cm_printf("[MQTT][%s][%d] ERR_MQTT_PUBCOMP_FAIL\r\n",__func__,__LINE__);				
						return -1;
					}

				}

		}	
	}
	else
		mqtt_cpub_tmp.mqtt_error = CMMQTT_AT_NOTCONN;

	if(mqtt_cpub_tmp.mqtt_error == CMMQTT_AT_OK)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int mqtt_sub_demo()
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	int mqtt_error = CMMQTT_AT_OK;
	volatile int cmmqtt_check_times = 0;

	if(myclient->state == CONNECTED)
	{
		CMMQTTSubmsg *submsg = (CMMQTTSubmsg *)cmmqtt_malloc(sizeof(CMMQTTSubmsg));
		cmmqtt_err_stat.cmmqtt_sub_error = 0;
		submsg->topicName.lenstring.len = 0;
		submsg->topicName.lenstring.data = NULL;
		submsg->topicName.cstring = (char*)cmmqtt_malloc(strlen(cmmqtt_glob_subtopic)+1);
		if(NULL == submsg->topicName.cstring)
		{
			return -1;
		}
		
		strcpy(submsg->topicName.cstring, cmmqtt_glob_subtopic);
		submsg->dup = 0;
		submsg->qos = cmmqtt_glob_subqos;
		submsg->queueout_ackcb = cmmqtt_suback_cb;
		submsg->topic_matchcb = cmmqtt_recpubcb[0];
      	mqtt_error = cmmqtt_subscribe(submsg);
		
		if(mqtt_error != CMMQTT_AT_OK)
			cmmqtt_free_submsg(submsg);
	}
	else
		mqtt_error = CMMQTT_AT_NOTCONN;

	if(mqtt_error == CMMQTT_AT_TOPIC_SUB_DONE)
	{
		cm_printf("[MQTT][%s][%d] ERR_MQTT_TOPIC_SUB_DONE\r\n",__func__,__LINE__);
		return -1;
	}
	else if(mqtt_error == CMMQTT_AT_NOTCONN)
	{
		cm_printf("[MQTT][%s][%d] ERR_MQTT_GPRS_NOTACT\r\n",__func__,__LINE__);
		return -1;
	}
	else if(mqtt_error == CMMQTT_AT_OK)
	{
		while(cmmqtt_check_times < CMMQTT_CHECK_NORMAL_TIMES)
		{
	
			if(cmmqtt_err_stat.cmmqtt_response_error == CMMQTT_CMD_RESPONSE_FAIL)
			{	
				cm_printf("[MQTT][%s][%d] ERR_MQTT_UNKNOWN_DATA\r\n",__func__,__LINE__);
				return -1;
			}
			if(cmmqtt_err_stat.cmmqtt_sub_error == CMMQTT_SUBACK_FAIL)
			{
				cm_printf("[MQTT][%s][%d] ERR_MQTT_SUBACK_FAIL\r\n",__func__,__LINE__);
				return -1;
			}
			else if(cmmqtt_err_stat.cmmqtt_sub_error == CMMQTT_SUB_OK)
			{
				return 0;
			}
			
			cmmqtt_check_times++;
			osDelay(50);
		}
	
	}
	
	return -1;
	
}

int mqtt_unsub_demo()
{
	 CMMQTTClient *myclient = cmmqtt_getclient();
	 int mqtt_error = CMMQTT_AT_OK;
	 volatile int cmmqtt_check_times = 0;
	 
	 if(myclient->state == CONNECTED)
	 {
		CMMQTTUnsubmsg *unsubmsg = (CMMQTTUnsubmsg *)cmmqtt_malloc(sizeof(CMMQTTUnsubmsg));
		unsubmsg->topicName.lenstring.len = 0;
		unsubmsg->topicName.lenstring.data = NULL;
		unsubmsg->topicName.cstring = (char*)cmmqtt_malloc(strlen(cmmqtt_glob_subtopic)+1);
		strcpy(unsubmsg->topicName.cstring,cmmqtt_glob_subtopic);
		unsubmsg->dup = 0;
		unsubmsg->queueout_ackcb = cmmqtt_unsuback_cb;
		cmmqtt_err_stat.cmmqtt_unsub_error = 0;

		cmmqtt_entry *topic = cmmqtt_find_topic(&myclient->topictable,unsubmsg->topicName.cstring,cmmqtt_entry,hash_head);
		if(NULL == topic)
		{				
			cm_printf("[MQTT][%s][%d] TOPIC IS NOT SUBBED\r\n",__func__,__LINE__);
			return -2;
		}
      	mqtt_error = cmmqtt_unsubscribe(unsubmsg);
		
		if (mqtt_error != CMMQTT_AT_OK)
		{
			cmmqtt_free_unsubmsg(unsubmsg);
			cm_printf("[MQTT][%s][%d] SEND UNSUB PACKET ERROR\r\n",__func__,__LINE__);
			return -1;
		}
	 }
	 else
		 mqtt_error = CMMQTT_AT_NOTCONN;

	if(mqtt_error == CMMQTT_AT_OK)
	{
		while(cmmqtt_check_times < CMMQTT_CHECK_NORMAL_TIMES)
		{
		
			if(cmmqtt_err_stat.cmmqtt_response_error == CMMQTT_CMD_RESPONSE_FAIL)
			{	
				cm_printf("[MQTT][%s][%d] MQTT RECEIVED ERROR PACKET\r\n",__func__,__LINE__);
			}

			switch(cmmqtt_err_stat.cmmqtt_unsub_error)
			{
				case CMMQTT_UNSUBACK_FAIL:
					cm_printf("[MQTT][%s][%d] MQTT UNSUB PACKET ERROR\r\n",__func__,__LINE__);
					return -1;
					break;
				case CMMQTT_UNSUB_OK:
					return 0;
					break;
				default:
					break;
			}
			
			cmmqtt_check_times++;
			osDelay(50);
		}
	}
	 
	 if(mqtt_error == CMMQTT_AT_OK)
	 {
		 return 0;
	 }
	 else
	 {			
		 return -1;
	 }

}

int mqtt_disc_demo()
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	int ret_value = 0;
	volatile int cmmqtt_check_times = 0;
	 
	if(myclient->state == CONNECTED)
	{	
		ret_value = cmmqtt_disc();

		if(myclient->cmmqtt_conmsg != NULL)
		{
			cmmqtt_free_conmsg(myclient->cmmqtt_conmsg);
			myclient->cmmqtt_conmsg = NULL;
		}
		myclient->state = DISCONNECTED;

		if(CMMQTT_AT_OK != ret_value)
		{
			cm_printf("[MQTT][%s][%d] ERR_MQTT_SENDPACK_FAIL\r\n",__func__,__LINE__);
			return -1;
		}
		
		while((INITIALED != myclient->state)&&(cmmqtt_check_times < CMMQTT_CHECK_NORMAL_TIMES))
		{
			osDelay(50);
			cmmqtt_check_times++;
		}			

		return 0;
	}
	else if(myclient->state == DISCONNECTED)
	{
		return -1;
	}

	return -1;

 }


int mqtt_del_demo()
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	 
	if(myclient->state == INITIALED )
	{	
		cmmqtt_deinit();
		return 0;
	}
	else
	{
		return -1;
	}
 }


#endif