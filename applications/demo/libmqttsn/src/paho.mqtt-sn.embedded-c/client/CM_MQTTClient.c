/*******************************************************************************
 by liyanlun @2018
 *******************************************************************************/

#include "CM_MQTTClient.h"
#include "string.h"
#include "cm_ssl.h"
#include "cm_sys.h"
//log_create_module(CMMQTT, PRINT_LEVEL_INFO);

static CMMQTTClient g_mqttclient ;
extern u32_t cmmqtt_max_connecttimes;
extern char *g_atCmdEngine;
extern CMQTT_ERR_STAT cmmqtt_err_stat;
extern CMMQTT_Mutex *g_pubMutex;

osMutexDef(mqtt_queuemutex);
osMutexDef(mqtt_taskmonitormutex);
osMutexDef(mqtt_taskprocessmutex);
osMutexDef(mqtt_g_pubMutex);

//MQTTS链接时候替换ca_cert
unsigned char * ca_cert = "-----BEGIN CERTIFICATE-----\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"-----END CERTIFICATE-----";

//MQTTS链接时候替换client_cert
unsigned char * client_cert = "-----BEGIN CERTIFICATE-----\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"-----END CERTIFICATE-----";

//MQTTS链接时候替换client_key
unsigned char * client_key = "-----BEGIN RSA PRIVATE KEY-----\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\r\n"\
"-----END RSA PRIVATE KEY-----";
//added by xgl
static ssl_ctx_t cm_ssl_ctx;
static WOLFSSL *cm_ssl;
CMMQTTClient *cmmqtt_getclient()
{
	return &g_mqttclient;
}

int32_t cmmqtt_init_cfg(const char *server, const uint16_t port, const char *client_id,//uint32_t command_timeout_s,
							   uint32_t keep_alive,const char*user_name, const char*password, uint32_t clean, uint32_t bufsize,uint32_t encrypt)
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	int ret = FAILURE;
	myclient->next_packetid = 1;
	myclient->ping_echo = 0;
	myclient->connectnum = 0;
	myclient->pinglostnum = 0;
	myclient->command_timeout_ms = MQTT_DEFAULT_CMD_TIMEOUT * 1000;
	myclient->keepAliveInterval = keep_alive;
	myclient->cleansession = clean;
	myclient->encrypt = encrypt;
	if(bufsize <= CMMQTT_MINRECV_BUFSIZE)
	{
		myclient->bufsize = CMMQTT_MINRECV_BUFSIZE;
	}
	else
		myclient->bufsize = bufsize;
	if(myclient->readbuf != NULL)
	{
		cmmqtt_free(myclient->readbuf);
		myclient->readbuf = NULL;
	}
	myclient->readbuf = (unsigned char*)cmmqtt_malloc(sizeof(unsigned char)*myclient->bufsize);
    if(myclient->readbuf == NULL)
    {
        return ret;
    }
    if(myclient->at_buf != NULL)
	{
		cmmqtt_free(myclient->at_buf);
		myclient->at_buf = NULL;
	}
	myclient->at_buf = (unsigned char*)cmmqtt_malloc(sizeof(unsigned char)*myclient->bufsize*2 + CMMQTT_AT_RESPONSE_DATA_LEN);
    if(myclient->at_buf == NULL)
    {
        cmmqtt_free(myclient->readbuf);
		myclient->readbuf = NULL;
        return FAILURE;
    }
	cmmqtt_init_network(&(myclient->ipstack),server, port);
	if(myclient->client_id != NULL)
	{
		cmmqtt_free(myclient->client_id);
		myclient->client_id = NULL;
	}
	if(client_id != NULL)
	{
		myclient->client_id = (char *)cmmqtt_malloc(strlen(client_id)+1);
		strcpy(myclient->client_id,(char*)client_id);
	}
	//cmmqtt_log("client_id : %s",myclient->client_id);
   	if(myclient->usr != NULL)
   	{
		cmmqtt_free((void *)myclient->usr);
		myclient->usr = NULL;
	}
	if(user_name != NULL)
	{
		myclient->usr = (char *)cmmqtt_malloc(strlen(user_name)+1);
		strcpy(myclient->usr,(char*)user_name);
	}
	if(myclient->password!= NULL)
	{
		cmmqtt_free(myclient->password);
		myclient->password = NULL;
	}
	if(password != NULL)
	{
		myclient->password = (char *)cmmqtt_malloc(strlen(password)+1);
		strcpy(myclient->password,(char*)password);
	}
	if(myclient->queuemutex != NULL)
	{
		cmmqtt_mutex_destory(myclient->queuemutex);
		cmmqtt_free(myclient->queuemutex);
		myclient->queuemutex = NULL;
	}
	myclient->queuemutex = (CMMQTT_Mutex *)cmmqtt_malloc(sizeof(CMMQTT_Mutex));
	if(cmmqtt_mutex_create(osMutex(mqtt_queuemutex), myclient->queuemutex)!= SUCCESS)
		return ret;

	if(g_pubMutex != NULL)
	{
		cmmqtt_mutex_destory(g_pubMutex);
		cmmqtt_free(g_pubMutex);
		g_pubMutex = NULL;
	}
	
	g_pubMutex = (CMMQTT_Mutex *)cmmqtt_malloc(sizeof(CMMQTT_Mutex));
	if(cmmqtt_mutex_create(osMutex(mqtt_g_pubMutex), g_pubMutex)!= SUCCESS)
		return ret;

	if(myclient->taskmonitormutex != NULL)
	{
		cmmqtt_mutex_destory(myclient->taskmonitormutex);
		cmmqtt_free(myclient->taskmonitormutex);
		myclient->taskmonitormutex = NULL;
	}
	myclient->taskmonitormutex = (CMMQTT_Mutex *)cmmqtt_malloc(sizeof(CMMQTT_Mutex));
	if(cmmqtt_mutex_create(osMutex(mqtt_taskmonitormutex), myclient->taskmonitormutex)!= SUCCESS)
		return ret;

	if(myclient->taskprocessmutex != NULL)
	{
		cmmqtt_mutex_destory(myclient->taskprocessmutex);
		cmmqtt_free(myclient->taskprocessmutex);
		myclient->taskprocessmutex = NULL;
	}
	myclient->taskprocessmutex = (CMMQTT_Mutex *)cmmqtt_malloc(sizeof(CMMQTT_Mutex));
	if(cmmqtt_mutex_create(osMutex(mqtt_taskprocessmutex), myclient->taskprocessmutex)!= SUCCESS)
		return ret;


	if(myclient->topictable.buckets != NULL)
		cmmqtt_topictable_destory(&myclient->topictable);
	if(cmmqtt_topictable_init(&myclient->topictable,NULL,8) != true)
		return ret;
	cmmqtt_queue_remove_all(&myclient->sendqueue);
	cmmqtt_queue_init(&myclient->sendqueue);
	
	if(myclient->cmmqtt_task_monitor.pvCreatedTask != CMMQTT_TASK_NULL )
	{
	
		if(cmmqtt_mutex_take(myclient->taskmonitormutex) == SUCCESS)
		{
			cmmqtt_deltask(&myclient->cmmqtt_task_monitor);
			myclient->cmmqtt_task_monitor.pvCreatedTask = CMMQTT_TASK_NULL;
			cmmqtt_mutex_give(myclient->taskmonitormutex);
		}

	}

	if(cmmqtt_mutex_take(myclient->taskmonitormutex) == SUCCESS)
	{
		myclient->cmmqtt_task_monitor.pcName = CMMQTT_TASK_MONITOR_NAME;
		myclient->cmmqtt_task_monitor.usStackDepth = CMMQTT_TASK_MONITOR_STACK_SIZE;
		myclient->cmmqtt_task_monitor.uxPriority =  CMMQTT_TASK_MONITOR_PRIORITY;
		myclient->cmmqtt_task_monitor.pvTaskCode =  cmmqtt_task_monitor;

		osThreadDef(CMMQTT_TASK_MONITOR, myclient->cmmqtt_task_monitor.pvTaskCode, myclient->cmmqtt_task_monitor.uxPriority, 0, 2*1024);
		myclient->cmmqtt_task_monitor.pvCreatedTask = osThreadCreate(osThread(CMMQTT_TASK_MONITOR), 0);
		
		cmmqtt_mutex_give(myclient->taskmonitormutex);

	}


	if(myclient->cmmqtt_task_process.pvCreatedTask != CMMQTT_TASK_NULL )
	{	
		if(cmmqtt_mutex_take(myclient->taskprocessmutex) == SUCCESS)
		{
			cmmqtt_deltask(&myclient->cmmqtt_task_process);
			myclient->cmmqtt_task_process.pvCreatedTask = CMMQTT_TASK_NULL;
			cmmqtt_mutex_give(myclient->taskprocessmutex);
		}
	}

	if(cmmqtt_mutex_take(myclient->taskprocessmutex) == SUCCESS)
	{
		myclient->cmmqtt_task_process.pcName = CMMQTT_TASK_PROCESS_NAME;
		myclient->cmmqtt_task_process.usStackDepth = CMMQTT_TASK_PROCESS_STACK_SIZE;
		myclient->cmmqtt_task_process.uxPriority =  CMMQTT_TASK_PROCESS_PRIORITY;
		myclient->cmmqtt_task_process.pvTaskCode =  cmmqtt_task_processing;

		osThreadDef(CMMQTT_TASK_PROCESS, myclient->cmmqtt_task_process.pvTaskCode, myclient->cmmqtt_task_process.uxPriority, 0, 4*1024);
		myclient->cmmqtt_task_process.pvCreatedTask = osThreadCreate(osThread(CMMQTT_TASK_PROCESS), 0);
		
		if(NULL != myclient->cmmqtt_task_process.pvCreatedTask)
			cmmqtt_suspendtask(&myclient->cmmqtt_task_process);
		
		cmmqtt_mutex_give(myclient->taskprocessmutex);
	}

	//cmmqtt_printf("task :%u,%u",myclient->cmmqtt_task_monitor.pvCreatedTask,myclient->cmmqtt_task_process.pvCreatedTask);
	ret = SUCCESS;
	myclient->state = INITIALED;
	return ret;
}
void cmmqtt_deinit()
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	cmmqtt_queue_remove_all(&myclient->sendqueue);
	cmmqtt_topictable_clean(&myclient->topictable);
	if(myclient->client_id != NULL)
	{
		cmmqtt_free(myclient->client_id);
		myclient->client_id = NULL;
	}
	if(myclient->usr != NULL)
   	{
		cmmqtt_free(myclient->usr);
		myclient->usr = NULL;
	}
	if(myclient->password!= NULL)
	{
		cmmqtt_free(myclient->password);
		myclient->password = NULL;
	}
	if(myclient->cmmqtt_conmsg != NULL)
	{
		cmmqtt_free_conmsg(myclient->cmmqtt_conmsg);
		myclient->cmmqtt_conmsg = NULL;
	}
	
	cmmqtt_deinit_network(&myclient->ipstack);
	
	
	if(myclient->cmmqtt_task_monitor.pvCreatedTask != CMMQTT_TASK_NULL)
	{

		while(myclient->mutex_protect != 0)
		{
			osDelay(50);   
		}

		if(cmmqtt_mutex_take(myclient->taskmonitormutex) == SUCCESS)
		{
			if(NULL != myclient->cmmqtt_task_monitor.pvCreatedTask)
				cmmqtt_suspendtask(&myclient->cmmqtt_task_monitor);
			if(NULL != myclient->cmmqtt_task_monitor.pvCreatedTask)
				cmmqtt_deltask(&myclient->cmmqtt_task_monitor);
			myclient->cmmqtt_task_monitor.pvCreatedTask = CMMQTT_TASK_NULL;
			cmmqtt_mutex_give(myclient->taskmonitormutex);
		}

	}


	if(myclient->cmmqtt_task_process.pvCreatedTask != CMMQTT_TASK_NULL)
	{	
		if(cmmqtt_mutex_take(myclient->taskprocessmutex) == SUCCESS)
		{
			if(NULL != myclient->cmmqtt_task_process.pvCreatedTask)
				cmmqtt_suspendtask(&myclient->cmmqtt_task_process);	
			if(NULL != myclient->cmmqtt_task_process.pvCreatedTask)
				cmmqtt_deltask(&myclient->cmmqtt_task_process);
			myclient->cmmqtt_task_process.pvCreatedTask = CMMQTT_TASK_NULL;

			cmmqtt_mutex_give(myclient->taskprocessmutex);

		}

	}


	if(myclient->queuemutex != NULL)
	{
		cmmqtt_mutex_destory(myclient->queuemutex);
		cmmqtt_free(myclient->queuemutex);
		myclient->queuemutex = NULL;
	}

	if(myclient->taskmonitormutex != NULL)
	{
		cmmqtt_mutex_destory(myclient->taskmonitormutex);
		cmmqtt_free(myclient->taskmonitormutex);
		myclient->taskmonitormutex = NULL;
	}

	if(myclient->taskprocessmutex != NULL)
	{
		cmmqtt_mutex_destory(myclient->taskprocessmutex);
		cmmqtt_free(myclient->taskprocessmutex);
		myclient->taskprocessmutex = NULL;
	}

	
	if(myclient->readbuf != NULL)
	{
		cmmqtt_free(myclient->readbuf);
		myclient->readbuf = NULL;
	}
	
    if(myclient->at_buf != NULL)
	{
		cmmqtt_free(myclient->at_buf);
		myclient->at_buf = NULL;
	}
	
	if(myclient->topictable.buckets != NULL)
		cmmqtt_topictable_destory(&myclient->topictable);
	
	if(myclient->ca != NULL)
	{
		cmmqtt_free(myclient->ca);
		myclient->ca = NULL;
	}
	if(myclient->client_ca != NULL)
	{
		cmmqtt_free(myclient->client_ca);
		myclient->client_ca = NULL;
	}
	if(myclient->client_key != NULL)
	{
		cmmqtt_free(myclient->client_key);
		myclient->client_key = NULL;
	}
	myclient->keepAliveInterval = 0;
	myclient->cleansession = 0;
	myclient->encrypt = 0;
	myclient->state = UNINITIALED;
}
void cmmqtt_task_processing(void *arg)
{
	CMMQTT_Timer timer;
	cmmqtt_init_timer(&timer);
	CMMQTTClient *c = cmmqtt_getclient();
	int mqtt_error;
	while(1)
	{
		//cmmqtt_log("task run ");
		//if(c->state == CONNECTED || c->state == NET_CONNECTED)
		{
		cmmqtt_countdown_ms(&timer, CMMQTT_RECV_TIME);
		while(!cmmqtt_timer_expired(&timer))
		{
			c->sock_protect = 1;
			if(cmmqtt_recvPacket(&timer) < SUCCESS)
				break;
			else
				cmmqtt_countdown_ms(&c->ping_timer, c->keepAliveInterval);
		}
		c->sock_protect = 0;
		}
		if(c->state == CONNECTED)
		{
			c->sock_protect = 1;
			mqtt_error = cmmqtt_neterror(&c->ipstack,NULL,0,500);
			if( mqtt_error < 0)
			{
				c->state = DISCONNECTED;
				c->sock_protect = 0;
				cm_printf("[MQTT]MQTTDISCONNECTED:%d",mqtt_error);
			}
			else
			{
				cmmqtt_clientalive();
				c->sock_protect = 0;
			}
		}
		c->processalive = 1;
		osDelay(500);
	}

}
void cmmqtt_task_monitor(void *arg)
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	int mqtt_error = CMMQTT_AT_OK;
	while(1)
	{
		myclient->mutex_protect = 1;
		cmmqtt_queue_checktimeout(&myclient->sendqueue);
		myclient->mutex_protect = 0;
		switch(myclient->state)
		{
			case NET_CONNECTING:
			{
			
			mqtt_error = cmmqtt_connect_network(&myclient->ipstack,myclient->ipstack.server,myclient->ipstack.port);
			if(myclient->encrypt != 0) //added by xgl
			{
				cm_sys_log("%s before ssl process", __func__);
				set_ssl_ctx_default(&cm_ssl_ctx);
				cm_ssl_ctx.sec_lvl = 2;
				cm_ssl_ctx.ca_cert = ca_cert;
				cm_ssl_ctx.client_cert = client_cert;
				cm_ssl_ctx.client_key = client_key;
			
				
				wolf_ssl_init(&cm_ssl_ctx); 
				cm_ssl = wolfSSL_new(cm_ssl_ctx.wolf_ctx);

				//INT32 no_block_flag = 0;
				if(SSL_set_fd(cm_ssl, myclient->ipstack.my_socket)!=WOLFSSL_SUCCESS)
				{
					cm_sys_log("%s SSL_set_fd fail", __func__);
					myclient->state = INITIALED;
					cmmqtt_printf("+QMTOPEN: 0,-1");
					wolfSSL_free(cm_ssl);
					//myclient->ipstack.ssl = NULL;
					myclient->ipstack.disconnect(&myclient->ipstack);
					//break;
				}
				if ((SSL_connect(cm_ssl)) == WOLFSSL_SUCCESS)
				{
					//no_block_flag = 1;			
					//CFW_TcpipSocketIoctl(socket, FIONBIO, &no_block_flag);
					cm_sys_log("%s SSL_connect success", __func__);
					cm_sys_log("SSL_connect myclient->state=%d\n",myclient->state);
					myclient->ipstack.wlfssl = cm_ssl;
				}
				else
				{
					cm_sys_log("%s SSL_connect fail", __func__);
					//no_block_flag = 1;
					//CFW_TcpipSocketIoctl(socket, FIONBIO, &no_block_flag);
					wolfSSL_free(cm_ssl);
					//myclient->ipstack->ssl = NULL;
					myclient->state = INITIALED;
					cmmqtt_printf("+QMTOPEN: 0,-1");
					myclient->ipstack.disconnect(&myclient->ipstack);
					//break;
				}
			}
			else
			{
				myclient->ipstack.wlfssl = 0; //added by xgl
			}
			if(mqtt_error == 0)
			{
				myclient->state = NET_CONNECTED;
				cmmqtt_resumetask(&myclient->cmmqtt_task_process);
			}
			else
			{
				
				myclient->ipstack.disconnect(&myclient->ipstack);
				myclient->state = RECONNECTING;
				//AT_CMD_RETURN(atCmdRespCmeError(g_atCmdEngine, ERR_MQTT_CONNECT_FAIL));//TODO:Connect MQTT server error code ERR_MQTT_CONNECT_FAIL,it seems that the value can not return.
				//cmmqtt_printf("+MQTTOPEN:FAIL");
				//cm_printf("[MQTT]cmmqtt_task_monitor +MQTTOPEN:FAIL!\r\n");
			}

		}
			break;
			case NET_CONNECTED:
		{
			if(myclient->cmmqtt_conmsg != NULL)
			{
				myclient->state = CONNECTING;
				mqtt_error = cmmqtt_connect(myclient->cmmqtt_conmsg);
				if (mqtt_error != CMMQTT_AT_OK)
				{
					cmmqtt_err_stat.cmmqtt_connect_error = CMMQTT_SENDPACK_ERROR;
					myclient->state = DISCONNECTED;
				}
				else
				{
					cmmqtt_err_stat.cmmqtt_connect_error = CMMQTT_CONMQTTSERVER_OK;
				}
				
			}
			else
			{
				myclient->state = DISCONNECTED;
			}
		}
			break;
			case DISCONNECTED:
		{
			cmmqtt_queue_remove_all(&myclient->sendqueue);
			myclient->ipstack.neterror = 0;
				if(myclient->cmmqtt_conmsg != NULL )
				{
			while(myclient->sock_protect != 0)
			{
				osDelay(50);   //�ȴ�cmmqtt_task_processing ��scoket select ��ʱ�˳� 
			}
			myclient->ipstack.disconnect(&myclient->ipstack);
					cmmqtt_suspendtask(&myclient->cmmqtt_task_process);
					myclient->state = RECONNECTING;
				}
				else
				{
					while(myclient->sock_protect != 0)
					{
						//usleep(50*1000);
						osDelay(50);   //�ȴ�cmmqtt_task_processing ��scoket select ��ʱ�˳� 
					}
										myclient->ipstack.disconnect(&myclient->ipstack);
					if(myclient->connectnum < cmmqtt_max_connecttimes)
			if(myclient->cmmqtt_task_process.pvCreatedTask != NULL)
				cmmqtt_suspendtask(&myclient->cmmqtt_task_process);
					myclient->connectnum = 0;
					myclient->ipstack.disconnected_cb(&myclient->ipstack);
			myclient->state = INITIALED;
		}
			}
			break;
			case RECONNECTING:
			{
			cmmqtt_reconnect(myclient);
		}
			break;
		/*	case CONNECTED:
		if(myclient->state == CONNECTED)
		{
			
			mqtt_error = cmmqtt_neterror(&myclient->ipstack,NULL,0,500);
			if(myclient->encrypt == 0 && mqtt_error < 0)
			{
				myclient->state = DISCONNECTED;
				cmmqtt_log("net error %d",mqtt_error);
			}
			//cmmqtt_printf("net error %d",mqtt_error);
			break;*/
			default:
				break;
		}
		osDelay(1000);
	}

}


int cmmqtt_sendPacket( unsigned char *buf,int length, CMMQTT_Timer* timer)
{
	CMMQTTClient *c = cmmqtt_getclient();
	int rc = FAILURE;
    int sent = 0;
    //cmmqtt_log("sendPack enter");
    while (sent < length && !cmmqtt_timer_expired(timer))
    {
		c->sock_protect = 1;
		rc = c->ipstack.mqttwrite(&c->ipstack, &buf[sent], length, cmmqtt_left_ms(timer));
		c->sock_protect = 0;
        if (rc < 0)  // there was an error writing the data
            break;
        sent += rc;
    }
    if (sent == length)
    {
       cmmqtt_countdown_ms(&c->ping_timer,c->keepAliveInterval); // record the fact that we have successfully sent the packet
       rc = SUCCESS;
    }
    else
       rc = FAILURE;
   // cmmqtt_log("sendPack exit:sent=%d,rc=%d",sent,rc);
    return rc;

}
int cmmqtt_decodePacket(CMMQTTClient* c, uint32* value, int timeout)
{
    unsigned char i;
    int multiplier = 1;
    int len = 0;
    const int MAX_NO_OF_REMAINING_LENGTH_BYTES = 4;
   // cmmqtt_log("decodePacket enter");
    *value = 0;
    do
    {
        int rc = MQTTPACKET_READ_ERROR;

        if (++len > MAX_NO_OF_REMAINING_LENGTH_BYTES)
        {
            rc = MQTTPACKET_READ_ERROR; /* bad data */
            goto exit;
        }
        rc = c->ipstack.mqttread(&c->ipstack, &i, 1, timeout);
        if (rc != 1)
            goto exit;
        *value += (i & 127) * multiplier;
        multiplier *= 128;
    } while ((i & 128) != 0);
exit:
    cmmqtt_log("decodePacket exit,value=%d,len=%d",*value,len);
    return len;
}

int cmmqtt_readPacket(CMMQTTClient* c, CMMQTT_Timer* timer, int *recvlength)
{
	c->sock_protect = 1;
	int rc = FAILURE;
    MQTTHeader header = {0};
    int len = 0;
	int recv_len = 0;
    unsigned int rem_len = 0;
	int loop = 6;
    if (c->ipstack.mqttread(&c->ipstack, c->readbuf, 1, cmmqtt_left_ms(timer)) != 1)
        goto exit;
   // cmmqtt_log("readPacket decodePacket enter");
    len = 1;
    cmmqtt_decodePacket(c, &rem_len, cmmqtt_left_ms(timer));
    len += MQTTPacket_encode(c->readbuf + 1, rem_len);
   // cmmqtt_log("readPacket mqttread enter");
	if(rem_len > c->bufsize -len)
		rem_len = c->bufsize -len;				//����ʣ��buf����ʱ�ضϣ����������ڻص������м�������
	while(recv_len < rem_len && loop > 0)
	{
		rc = c->ipstack.mqttread(&c->ipstack, c->readbuf + len + recv_len, rem_len - recv_len, cmmqtt_left_ms(timer)); 
		if(rc <= 0)
			//cmmqtt_countdown_ms(timer, CMMQTT_RECV_TIME);	
			break;
		else
			recv_len = recv_len + rc;
		loop--;
	}
    header.byte = c->readbuf[0];
    rc = header.bits.type;
	*recvlength = recv_len;
exit:
    cmmqtt_log("readPacket exit rc=%d recv =%d",rc,recv_len);
	c->sock_protect = 0;
    return rc;
}

int cmmqtt_recvPacket(CMMQTT_Timer *timer)
{
		int packet_type = -1 ;
		int recv_len = 0;
		CMMQTTClient *c = cmmqtt_getclient();
		memset(c->readbuf,0,c->bufsize);
		packet_type = cmmqtt_readPacket(c,timer,&recv_len);
	    //cmmqtt_log("packet_type=%d",packet_type);
	    cmmqtt_err_stat.cmmqtt_response_error = 0;
		switch (packet_type)
		{
			case CONNACK:
				cmmqtt_deconack(c->readbuf);
				break;
			case PUBACK:
				cmmqtt_depuback(c->readbuf);
				break;
			case PUBREC:
				cmmqtt_depuback(c->readbuf);
				break;
			case PUBCOMP:
				cmmqtt_depuback(c->readbuf);
				break;
			case SUBACK:
				//cmmqtt_interace[1]((char*)c->readbuf);
				cmmqtt_desuback(c->readbuf);
				break;
			case UNSUBACK:
				cmmqtt_deunsuback(c->readbuf);
				break;
			case PUBLISH:
			 	cmmqtt_depublish(c->readbuf,recv_len);
			 	break;
			case PUBREL:
				cmmqtt_depubrel(c->readbuf);
				break;
			case PINGRESP:
				cmmqtt_depingresp();
				break;
			default:
				if(FAILURE != packet_type)
				{
					cmmqtt_err_stat.cmmqtt_response_error = CMMQTT_CMD_RESPONSE_FAIL;
				}
				else
				{
					cmmqtt_err_stat.cmmqtt_response_error = CMMQTT_CMD_RESPONSE_OK;
				}
				return packet_type;
				break;
		}
		cmmqtt_err_stat.cmmqtt_response_error = CMMQTT_CMD_RESPONSE_OK;
		//cmmqtt_mutex_give(c->bufmutex);
		return packet_type;
}

void cmmqtt_clientalive()
{
	CMMQTTClient *myclient = cmmqtt_getclient();
	if(myclient->state == CONNECTED)
	{
		if(cmmqtt_timer_expired(&myclient->ping_timer))
		{
			if(myclient->pinglostnum != 0)
				//apb_proxy_urc_string(APB_PROXY_INVALID_CMD_ID,"+MQTTTO: %d", MQTT_TO_PING);
				cmmqtt_printf("+MQTTTO: %d",MQTT_TO_PING);
			if(myclient->pinglostnum < CMMQTT_MAX_PINGLOST	)
			{
				cmmqtt_ping();
				cmmqtt_countdown_ms(&myclient->ping_timer,myclient->keepAliveInterval);
				myclient->pinglostnum++;
			}
			else
			{
				cmmqtt_log("ping timeout");
				myclient->state = DISCONNECTED;
			}
		}
	}
}

void cmmqtt_reconnect(CMMQTTClient *myclient)
{
	char str_rsp[50] = {0};
	if(myclient->connectnum < cmmqtt_max_connecttimes)
	{
		myclient->connectnum++;
        myclient->state = NET_CONNECTING ; 	
	}
	else
	{

		if(myclient->cmmqtt_conmsg != NULL)
		{
			cmmqtt_free_conmsg(myclient->cmmqtt_conmsg);
			myclient->cmmqtt_conmsg = NULL;
		}
		myclient->state = DISCONNECTED;
	}
}






