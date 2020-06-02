/*********************************************************
*  @file    cm_network_demo.c
*  @brief   ML302 OpenCPU network example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/15
********************************************************/
#include "main.h"
#include "oc_device.h"

struct   in_addr test_remote_addr;
uint16_t test_remote_port;
char test_msg[] = "network test\n";
char test_rx_buf[100];
int cm_global_socket_id = -1;
osThreadId cm_socket_TaskHandle = 0;
unsigned char cm_socket_buf[1024] = {0};
static void cm_socket_select_task(void *not_used);
/*
网络初始化配置
*/
void cm_test_network_config(void)
{
    int i;
    /**************************************************/
    for(i = 0;i< 100;i++)
    {
        oc_vir_at_send_cmd("AT+CPIN?\r\n");
        if(oc_vir_at_check_str("READY") != 0)
        {
            break;
            
        }   
    }        
    if(i>=100)
    {
        cm_printf("sim card error\n");
        oc_vir_at_dump();
        return;
    }
    /**************************************************/
    oc_vir_at_send_cmd("AT+CFUN=1\r\n");
    if(oc_vir_at_check_str("OK") == 0)
    {
        cm_printf("cfun error\n");
        return;
    }    
    /**************************************************/
    for(i = 0;i< 30;i++)
    { 
        oc_vir_at_send_cmd("AT+CEREG?\r\n");
        if(oc_vir_at_check_str("1,1") != 0)
        {
            break;       
        }

        oc_vir_at_send_cmd("AT+CEREG?\r\n");
        if(oc_vir_at_check_str("0,1") != 0)
        {
            break;       
        }        
    }        
    if(i>=30)
    {
        cm_printf("cereg error\n");
        oc_vir_at_dump();
        return;
    }
    oc_vir_at_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
    if(oc_vir_at_check_str("OK") == 0)
    {
        cm_printf("CGDCONT error\n");
        return;
    }    
    oc_vir_at_send_cmd("AT+CGACT=1,1\r\n");
    if(oc_vir_at_check_str("OK") == 0)
    {
        cm_printf("cgact error\n");
        return;
    }    
    cm_printf("network ready\n");
    
}
/*
   测试UDP
*/
void cm_test_udp(unsigned char **cmd,int len)
{
    struct sockaddr_in server_addr;  
    int sock_fd; 
    struct sockaddr_in from;
    int data_len;
	
    inet_aton(cmd[2], &test_remote_addr);  
    test_remote_port = char_to_int(cmd[3]);
	socklen_t fromlen = sizeof(struct sockaddr_in);
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd == -1)
	{
		cm_printf ( "[UDP]socket create error\n");
		return;
	}
	memset(&server_addr, 0, sizeof(server_addr)); 
	server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = test_remote_addr.s_addr;  
    server_addr.sin_port = htons(test_remote_port);  

	connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
	cm_printf ("[UDP]data sending\n");
	send(sock_fd, (char *)test_msg, sizeof(test_msg), 0);
	 memset(test_rx_buf,0,100);
	/*data_len = recvfrom(sock_fd, test_rx_buf,
                    100, MSG_TRUNC | MSG_DONTWAIT, (struct sockaddr*)&from, &fromlen);*/
    cm_printf ( "[UDP]waiting...\n");
    data_len = recvfrom(sock_fd, test_rx_buf,
                    100, MSG_TRUNC, (struct sockaddr*)&from, &fromlen);
    cm_printf("[UDP]waiting end\n");                   
	if(data_len >0)
		{
		    cm_printf ( "[UDP]get:%s\n",test_rx_buf);
			cm_printf ( "[UDP]len:%d\n",data_len);			 
		}
		
	close(sock_fd);	
}
/*
    测试TCP
*/
void cm_test_tcpclient_send(unsigned char **cmd,int len)
{
    int ret;
    ret = send(cm_global_socket_id, cmd[2], strlen(cmd[2]), 0);
    cm_printf("[Send]:%d\r\n",ret);
}
void cm_test_tcpclient(unsigned char **cmd,int len)
{
    struct sockaddr_in server_addr;  
    int data_len;
    test_remote_port = char_to_int(cmd[3]);
    inet_aton(cmd[2], &test_remote_addr);  
    if(cm_global_socket_id != -1)
    {
        return;
    }
	cm_global_socket_id = socket(AF_INET, SOCK_STREAM, 0);  
	if (cm_global_socket_id == -1) {  
        cm_printf ("[TCPCLIENT]socket create error\n");
		 return;
    }  
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = test_remote_addr.s_addr;
    server_addr.sin_port = htons(test_remote_port);
    if(connect(cm_global_socket_id, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)))
	{
		cm_printf ( "[TCPCLIENT]tcp connect error\n");
		return;
	}
    if(cm_socket_TaskHandle == 0)
    {
        osThreadDef(OC_Socket_Task, cm_socket_select_task, osPriorityNormal, 0, 1024);
        cm_socket_TaskHandle = osThreadCreate(osThread(OC_Socket_Task), 0);  
    }   
	cm_printf ( "[TCPCLIENT]Socket init done\n");	
    
}
void cm_socket_callback()
{
    int data_len = 0;
    int i;
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(struct sockaddr_in);
    data_len = recvfrom(cm_global_socket_id, cm_socket_buf,1024, MSG_TRUNC | MSG_DONTWAIT, (struct sockaddr*)&from, &fromlen);
    if(data_len <=0)
    {
        cm_printf("[TCPCLIENT]socket closed\n",data_len);
        close(cm_global_socket_id);
        cm_global_socket_id = -1;
        return;
    }
    cm_printf("[TCPCLIENT]Data Arrives:%d\n",data_len);
    for(i=0;i<data_len;i++)
        cm_printf("%c",cm_socket_buf[i]);
    cm_printf("\n");
    
}

static void cm_socket_select_task(void *not_used)
{
    int result, i;
    int maxfd;
    uint32_t resp_len = 0;
    uint32_t resp_reserve_len = 0; //输出信息提示头长度
    int error;
    socklen_t socket_code_size = sizeof(error);
    fd_set readfds, errorfds, writefds;

	
    while(1)
    {   
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&errorfds);
        FD_SET(cm_global_socket_id,&readfds);						
        FD_SET(cm_global_socket_id,&errorfds);

        //wait forever
        struct timeval tv;
        tv.tv_sec  = 5;
        tv.tv_usec = 0;
        result = select(cm_global_socket_id + 1, &readfds, &writefds, &errorfds, &tv);		
        if (result > 0)
        {
            uint8_t* outbuf = NULL;

            //检测socket可读性
            if ((cm_global_socket_id!= -1)&& FD_ISSET(cm_global_socket_id, &readfds))
            {
                cm_socket_callback();
            }

            //检测connect
            if((cm_global_socket_id!= -1)&&FD_ISSET(cm_global_socket_id, &writefds))
            {
                getsockopt(cm_global_socket_id, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&socket_code_size);
                if(error == 0)
                {
                    cm_printf("[TCPCLIENT]CONNECT OK\n");
                }
                else
                {
                    cm_printf("[TCPCLIENT]CONNECT FAIL");
                }
            }

            //检测socket错误
            if ((cm_global_socket_id!= -1)&&FD_ISSET(cm_global_socket_id, &errorfds))
            {

                getsockopt(cm_global_socket_id, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&socket_code_size);
                if(error != 0)
                {
                    cm_printf("[TCPCLIENT]socket closed\n");
                    close(cm_global_socket_id);
                    cm_global_socket_id = -1;
                }
            }
        }
    }
}
