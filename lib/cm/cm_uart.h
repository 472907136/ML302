/*********************************************************
*  @file    cm_uart.h
*  @brief   ML302 OpenCPU uart APIs header
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/11
********************************************************/
#ifndef __CM_UART_H__
#define __CM_UART_H__
  /**

 * @defgroup UART_FUNCTIONS
 * 
   串口API，请参照SDK中示例文件使用
 *@{
 */
 
typedef enum
{
    CM_UART_EVENT_RX_ARRIVED = (1 << 0),  ///< Received new data
    CM_UART_EVENT_RX_OVERFLOW = (1 << 1), ///< Rx fifo overflowed
    CM_UART_EVENT_TX_COMPLETE = (1 << 2)  ///< All data had been sent
}cm_uart_event_t;
/**
 * @brief 串口ID，一共支持三路串口
 */
typedef enum{
    CM_UART_0,//引脚29:TXD 引脚30:RXD
    CM_UART_1,//引脚97:TXD 引脚98:RXD
    CM_UART_2,//引脚15:TXD 引脚14:RXD
}cm_uart_id_t;

/**
 *  @brief 串口支持的波特率，列表中115200及以下的波特率可使用自适应机制
 */
typedef enum{
    CM_UART_BAUD_AUTO   = 0, 
    CM_UART_BAUD_2400   = 2400,
    CM_UART_BAUD_4800   = 4800,
    CM_UART_BAUD_9600   = 9600,
    CM_UART_BAUD_14400  = 14400,
    CM_UART_BAUD_19200  = 19200,
    CM_UART_BAUD_28800  = 28800,
    CM_UART_BAUD_33600  = 33600,
    CM_UART_BAUD_38400  = 38400,
    CM_UART_BAUD_57600  = 57600,
    CM_UART_BAUD_115200 = 115200,
    CM_UART_BAUD_230400 = 230400,
    CM_UART_BAUD_460800 = 460800,
    CM_UART_BAUD_921600 = 921600,
}cm_uart_baud_t;

/**
 *  @brief 串口数据位
 */
typedef enum {
    CM_UART_DATA_BITS_7 = 7,
    CM_UART_DATA_BITS_8 = 8,
}cm_uart_data_bits_t;
/**
 *  @brief 串口停止位
 */
typedef enum {
    CM_UART_STOP_BITS_1 = 1,
    CM_UART_STOP_BITS_2 = 2,
}cm_uart_stop_bits_t;

/**
 *  @brief 串口校验
 */
typedef enum {
    CM_UART_NO_PARITY,
    CM_UART_ODD_PARITY,
    CM_UART_EVEN_PARITY,
}cm_uart_parity_t;

/**
 *  \brief 串口中断回调函数类型
 */
typedef void (*cm_uart_cb)(void *param, unsigned int evt);
/**
 *  @brief 串口初始化结构体
 */
typedef struct {
    cm_uart_id_t id;
    cm_uart_baud_t baud;
    cm_uart_data_bits_t databit;
    cm_uart_stop_bits_t stopbit;
    cm_uart_parity_t parity;
    unsigned int event;
    cm_uart_cb cb;
    void *param;
    int rx_buf_size;
    int tx_buf_size;
}cm_uart_cfg_t;
/**
 *  \brief 串口初始化函数
 *  
 *  \param [in] cfg 串口初始化结构体
 *  \return 0:初始化成功 -1:初始化失败
 *  
 *  \details More details
 */
int cm_uart_init(cm_uart_cfg_t *cfg);
/**
 *  \brief 串口发送函数，数据可能被内部缓存后再发送
 *  
 *  \param [in] id 串口号
 *  \param [in] buf 发送数据的buffer指针
 *  \param [in] len 发送数据的长度
 *  \return 实际发送或缓存的长度
 *  
 *  \details More details
 */
int cm_uart_send_cache(cm_uart_id_t id,unsigned char *buf,int len);
/**
 *  \brief 串口发送函数，数据会被立即发送
 *  
 *  \param [in] id 串口号
 *  \param [in] buf 发送数据的buffer指针
 *  \param [in] len 发送数据的长度
 *  \param [in] timeout_ms 最大的发送等待时间，单位ms
 *  \return 实际发送的长度
 *  
 *  \details More details
 */
int cm_uart_send_no_cache(cm_uart_id_t id,unsigned char *buf,int len,int timeout_ms);
/**
 *  \brief 获取当前已接收到缓冲区的数据长度
 *  
 *  \param [in] id 串口号
 *  \return 当前接收到的数据长度
 *  
 *  \details More details
 */
int cm_uart_get_available_receive_bytes(cm_uart_id_t id);
/**
 *  \brief 串口接收数据
 *  
 *  \param [in] id 串口号
 *  \param [in] buf 接收数据的buffer指针
 *  \param [in] len buffer的长度
 *  \return 实际接收到的数据长度
 *  
 *  \details More details
 */
int cm_uart_receive(cm_uart_id_t id,unsigned char * buf,int len);
/**
 *  \brief 串口关闭
 *  
 *  \param [in] id 串口号
 *  \return 空
 *  
 *  \details More details
 */
void cm_uart_deinit(cm_uart_id_t id);
/** @} */ 
#endif