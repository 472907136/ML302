
/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */
#include "drv_usart.h"
#include "oc.h"
#include "oc_device.h"

#ifdef OC_USING_USART
#include "ml302_hal.h"

#ifdef OC_USING_SHELL

#include "shell.h"

#define DBG_UART_GET_DATA (1 << 0)


osThreadId OC_Uart_TaskHandle;
unsigned int data_arrived = 0;
unsigned char shell_buf[512];
unsigned char uart_buf[1024];
int uart_buf_len = 0;


Shell shell;

osMutexDef(cmdMutex);
osMutexId cmd_mutex;


// 接收回调函数
static void test_oc_uart_cb(void *param, unsigned int evt)
{
    data_arrived = 1;
    osSignalSet(OC_Uart_TaskHandle, DBG_UART_GET_DATA);
}
static uart_recv_task(void *param)
{
    unsigned char buf[64] = {0};
    unsigned char *p;
    int len;
    osEvent ret;
    while (1)
    {
        ret = osSignalWait(DBG_UART_GET_DATA, osWaitForever);
        if (data_arrived == 1)
        {
            uart_buf_len = cm_uart_receive(OC_MAIN_UART, uart_buf, 1024);

            while (uart_buf_len < 1024)
            {
                len = cm_uart_receive(OC_MAIN_UART, uart_buf + uart_buf_len, 1024 - uart_buf_len);

                if (len == 0)
                    break;
                uart_buf_len += len;
            }
            for (len = 0; len < uart_buf_len; len++)
            {
                shellHandler(&shell, uart_buf[len]);
            }

            data_arrived = 0;
        }
    }
}

static void shell_write_cb(const char data)
{
    cm_uart_send_no_cache(OC_MAIN_UART, &data, 1, 100);
}
#endif


void oc_printf(char *str, ...)
{

    static  char s[600]; //This needs to be large enough to store the string TODO Change magic number
    va_list args;
    int len;
    if ((str == NULL) || (strlen(str) == 0))
    {
        return;
    }
    va_start (args, str);
    len = vsprintf ((char*)s, str, args);
    va_end (args);
    cm_uart_send_no_cache(OC_MAIN_UART,s,len,100);
  
}

void oc_hw_usart_init(void)
{
    cm_uart_cfg_t cfg;
    
    osThreadDef(OC_Uart_Task, uart_recv_task, osPriorityNormal, 0, 1024);
    OC_Uart_TaskHandle = osThreadCreate(osThread(OC_Uart_Task), 0);
    cmd_mutex = osMutexCreate( osMutex(cmdMutex) );
    cfg.id = OC_MAIN_UART;
    cfg.baud = CM_UART_BAUD_115200;
    cfg.databit = CM_UART_DATA_BITS_8;
    cfg.stopbit = CM_UART_STOP_BITS_1;
    cfg.parity = CM_UART_NO_PARITY;
    cfg.cb = test_oc_uart_cb;
    cfg.param = "oc uart";
    cfg.event = CM_UART_EVENT_RX_ARRIVED;
    cfg.rx_buf_size = 4*1024;
    cfg.tx_buf_size = 4*1024;
    cm_uart_init(&cfg);
    oc_printf("uart init success\r\n");
#ifdef OC_USING_SHELL
    shell.write = shell_write_cb;
    shellInit(&shell, shell_buf, 512);
#endif // OC_USING_SHELL
    oc_printf("uart init success\r\n");
}


#endif //OC_USING_USART

