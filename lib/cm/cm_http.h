/*********************************************************
*  @file    cm_http.h
*  @brief   ML302 OpenCPU HTTP APIs header
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2020/04/24
********************************************************/
#ifndef __CM_HTTP_H__
#define __CM_HTTP_H__
#include "httpclient.h"
  /**

 * @defgroup HTTP_FUNCTIONS
 * 
   HTTP API，请参照SDK中示例文件使用
 *@{
 */

 
 /**
 *  \brief 执行Get方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 接收数据的对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_get(httpclient_t *client, char *url, httpclient_data_t *client_data);

 /**
 *  \brief 执行POST方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 接收数据的对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_post(httpclient_t *client, char *url, httpclient_data_t *client_data);

 /**
 *  \brief 执行PUT方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 接收数据对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_put(httpclient_t *client, char *url, httpclient_data_t *client_data);
 
 /**
 *  \brief 执行Delete方法，阻塞直到完成
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] client_data 客户端对象数据指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_delete(httpclient_t *client, char *url, httpclient_data_t *client_data);
 /**
 *  \brief HTTP连接
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_connect(httpclient_t *client, char *url);

 /**
 *  \brief 发送请求(GET或POST)
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] url 网址指针
 *  \param [in] method 方法
 *  \param [in] client_data 要发送的数据对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_send_request(httpclient_t *client, char *url, int method, httpclient_data_t *client_data);

 /**
 *  \brief 接收服务器返回
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] client_data 客户端数据对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
HTTPCLIENT_RESULT httpclient_recv_response(httpclient_t *client, httpclient_data_t *client_data);
 /**
 *  \brief 关闭HTTP连接
 *  
 *  \param [in] client 客户端对象指针
 *  \return 执行结果
 *  
 *  \details More details
 */
void httpclient_close(httpclient_t *client);
 /**
 *  \brief 获取返回码
 *  
 *  \param [in] client 客户端对象指针
 *  \return 返回码
 *  
 *  \details More details
 */
int httpclient_get_response_code(httpclient_t *client);
 /**
 *  \brief 设置头部
 *  
 *  \param [in] client 客户端对象指针
 *  \param [in] header 头部字符串
 *  \return 空
 *  
 *  \details More details
 */
void httpclient_set_custom_header(httpclient_t *client, char *header);
 /**
 *  \brief 获取返回头部值
 *  
 *  \param [in] header_buf 返回头部指针
 *  \param [in] name 返回头部名称
 *  \param [in] val_pos 返回头部中头部值的位置
 *  \param [in] val_len 头部值长度
 *  \return 0：正确 其他：错误 
 *  
 *  \details More details
 */
int httpclient_get_response_header_value(char *header_buf, char *name, int *val_pos, int *val_len);
/** @} */ 
#endif