// /*********************************************************
// *  @file    onemo_vir_at_demo.c
// *  @brief   ML302 OpenCPU virture AT channel example file
// *  Copyright (c) 2019 China Mobile IOT.
// *  All rights reserved.
// *  created by XieGangLiang 2019/10/08
// ********************************************************/
// #define DBG_TAG "at_client"
// #define DBG_LVL DBG_LOG
// #include "oc_dbg.h"
// #include "shell.h"
// #include "main.h"

// #include "at_client.h"

// #ifdef AT_USING_CLIENT

// #include "stdlib.h"
// #define AT_RESP_END_OK                 "OK"
// #define AT_RESP_END_ERROR              "ERROR"
// #define AT_RESP_END_FAIL               "FAIL"
// #define AT_END_CR_LF                   "\r\n"

// static struct at_client at_client_table[AT_CLIENT_NUM_MAX] = { 0 };


// extern oc_size_t at_vprintfln( const char *format, va_list args);
// extern void at_print_raw_cmd(const char *type, const char *cmd, oc_size_t size);
// extern const char *at_get_last_cmd(oc_size_t *cmd_size);

// /**
//  * Create response object.
//  *
//  * @param buf_size the maximum response buffer size
//  * @param line_num the number of setting response lines
//  *         = 0: the response data will auto return when received 'OK' or 'ERROR'
//  *        != 0: the response data will return when received setting lines number data
//  * @param timeout the maximum response time
//  *
//  * @return != RT_NULL: response object
//  *          = RT_NULL: no memory
//  */
// at_response_t at_create_resp(oc_size_t buf_size, oc_size_t line_num, oc_int32_t timeout)
// {
//     at_response_t resp = OC_NULL;

//     resp = (at_response_t) calloc(1, sizeof(struct at_response));
//     if (resp == OC_NULL)
//     {
//         LOG_E("AT create response object failed! No memory for response object!");
//         return OC_NULL;
//     }

//     resp->buf = (char *) calloc(1, buf_size);
//     if (resp->buf == OC_NULL)
//     {
//         LOG_E("AT create response object failed! No memory for response buffer!");
//         free(resp);
//         return OC_NULL;
//     }

//     resp->buf_size = buf_size;
//     resp->line_num = line_num;
//     resp->line_counts = 0;
//     resp->timeout = timeout;

//     return resp;
// }

// /**
//  * Delete and free response object.
//  *
//  * @param resp response object
//  */
// void at_delete_resp(at_response_t resp)
// {
//     if (resp && resp->buf)
//     {
//         free(resp->buf);
//     }

//     if (resp)
//     {
//         free(resp);
//         resp = OC_NULL;
//     }
// }

// /**
//  * Set response object information
//  *
//  * @param resp response object
//  * @param buf_size the maximum response buffer size
//  * @param line_num the number of setting response lines
//  *         = 0: the response data will auto return when received 'OK' or 'ERROR'
//  *        != 0: the response data will return when received setting lines number data
//  * @param timeout the maximum response time
//  *
//  * @return  != RT_NULL: response object
//  *           = RT_NULL: no memory
//  */
// at_response_t at_resp_set_info(at_response_t resp, oc_size_t buf_size, oc_size_t line_num, oc_int32_t timeout)
// {
//     OC_ASSERT(resp);

//     if (resp->buf_size != buf_size)
//     {
//         resp->buf_size = buf_size;

//         resp->buf = (char *) realloc(resp->buf, buf_size);
//         if (!resp->buf)
//         {
//             LOG_D("No memory for realloc response buffer size(%d).", buf_size);
//             return OC_NULL;
//         }
//     }

//     resp->line_num = line_num;
//     resp->timeout = timeout;

//     return resp;
// }

// /**
//  * Get one line AT response buffer by line number.
//  *
//  * @param resp response object
//  * @param resp_line line number, start from '1'
//  *
//  * @return != RT_NULL: response line buffer
//  *          = RT_NULL: input response line error
//  */
// const char *at_resp_get_line(at_response_t resp, oc_size_t resp_line)
// {
//     char *resp_buf = resp->buf;
//     char *resp_line_buf = OC_NULL;
//     oc_size_t line_num = 1;

//     OC_ASSERT(resp);

//     if (resp_line > resp->line_counts || resp_line <= 0)
//     {
//         LOG_E("AT response get line failed! Input response line(%d) error!", resp_line);
//         return OC_NULL;
//     }

//     for (line_num = 1; line_num <= resp->line_counts; line_num++)
//     {
//         if (resp_line == line_num)
//         {
//             resp_line_buf = resp_buf;

//             return resp_line_buf;
//         }

//         resp_buf += strlen(resp_buf) + 1;
//     }

//     return OC_NULL;
// }

// /**
//  * Get one line AT response buffer by keyword
//  *
//  * @param resp response object
//  * @param keyword query keyword
//  *
//  * @return != RT_NULL: response line buffer
//  *          = RT_NULL: no matching data
//  */
// const char *at_resp_get_line_by_kw(at_response_t resp, const char *keyword)
// {
//     char *resp_buf = resp->buf;
//     char *resp_line_buf = OC_NULL;
//     oc_size_t line_num = 1;

//     OC_ASSERT(resp);
//     OC_ASSERT(keyword);

//     for (line_num = 1; line_num <= resp->line_counts; line_num++)
//     {
//         if (strstr(resp_buf, keyword))
//         {
//             resp_line_buf = resp_buf;

//             return resp_line_buf;
//         }

//         resp_buf += strlen(resp_buf) + 1;
//     }

//     return OC_NULL;
// }

// /**
//  * Get and parse AT response buffer arguments by line number.
//  *
//  * @param resp response object
//  * @param resp_line line number, start from '1'
//  * @param resp_expr response buffer expression
//  *
//  * @return -1 : input response line number error or get line buffer error
//  *          0 : parsed without match
//  *         >0 : the number of arguments successfully parsed
//  */
// int at_resp_parse_line_args(at_response_t resp, oc_size_t resp_line, const char *resp_expr, ...)
// {
//     va_list args;
//     int resp_args_num = 0;
//     const char *resp_line_buf = OC_NULL;

//     OC_ASSERT(resp);
//     OC_ASSERT(resp_expr);

//     if ((resp_line_buf = at_resp_get_line(resp, resp_line)) == OC_NULL)
//     {
//         return -1;
//     }

//     va_start(args, resp_expr);

//     resp_args_num = vsscanf(resp_line_buf, resp_expr, args);

//     va_end(args);

//     return resp_args_num;
// }

// /**
//  * Get and parse AT response buffer arguments by keyword.
//  *
//  * @param resp response object
//  * @param keyword query keyword
//  * @param resp_expr response buffer expression
//  *
//  * @return -1 : input keyword error or get line buffer error
//  *          0 : parsed without match
//  *         >0 : the number of arguments successfully parsed
//  */
// int at_resp_parse_line_args_by_kw(at_response_t resp, const char *keyword, const char *resp_expr, ...)
// {
//     va_list args;
//     int resp_args_num = 0;
//     const char *resp_line_buf = OC_NULL;

//     OC_ASSERT(resp);
//     OC_ASSERT(resp_expr);

//     if ((resp_line_buf = at_resp_get_line_by_kw(resp, keyword)) == OC_NULL)
//     {
//         return -1;
//     }

//     va_start(args, resp_expr);

//     resp_args_num = vsscanf(resp_line_buf, resp_expr, args);

//     va_end(args);

//     return resp_args_num;
// }

// /**
//  * Send commands to AT server and wait response.
//  *
//  * @param client current AT client object
//  * @param resp AT response object, using RT_NULL when you don't care response
//  * @param cmd_expr AT commands expression
//  *
//  * @return 0 : success
//  *        -1 : response status error
//  *        -2 : wait timeout
//  *        -7 : enter AT CLI mode
//  */
// int at_obj_exec_cmd(at_client_t client, at_response_t resp, const char *cmd_expr, ...)
// {
//     va_list args;
//     oc_size_t cmd_size = 0;
//     oc_err_t result = OC_EOK;
//     const char *cmd = OC_NULL;

//     OC_ASSERT(cmd_expr);

//     if (client == OC_NULL)
//     {
//         LOG_E("input AT Client object is NULL, please create or get AT Client object!");
//         return -OC_ERROR;
//     }

//     /* check AT CLI mode */
//     if (client->status == AT_STATUS_CLI && resp)
//     {
//         return -OC_EBUSY;
//     }

//     osMutexWait(client->lock, osWaitForever);

//     client->resp_status = AT_RESP_OK;
//     client->resp = resp;

//     if (resp != OC_NULL)
//     {
//         resp->buf_len = 0;
//         resp->line_counts = 0;
//     }

//     va_start(args, cmd_expr);
//     at_vprintfln(cmd_expr, args);
//     va_end(args);

//     if (resp != OC_NULL)
//     {
//         if (osSemaphoreWait(client->resp_notice, resp->timeout) != OC_EOK)
//         {
//             cmd = at_get_last_cmd(&cmd_size);
//             LOG_D("execute command (%.*s) timeout (%d ticks)!", cmd_size, cmd, resp->timeout);
//             client->resp_status = AT_RESP_TIMEOUT;
//             result = -OC_ETIMEOUT;
//             goto __exit;
//         }
//         if (client->resp_status != AT_RESP_OK)
//         {
//             cmd = at_get_last_cmd(&cmd_size);
//             LOG_E("execute command (%.*s) failed!", cmd_size, cmd);
//             result = -OC_ERROR;
//             goto __exit;
//         }
//     }

// __exit:
//     client->resp = OC_NULL;

//     osMutexRelease(client->lock);

//     return result;
// }

// /**
//  * Waiting for connection to external devices.
//  *
//  * @param client current AT client object
//  * @param timeout millisecond for timeout
//  *
//  * @return 0 : success
//  *        -2 : timeout
//  *        -5 : no memory
//  */
// int at_client_obj_wait_connect(at_client_t client, oc_uint32_t timeout)
// {
//     oc_err_t result = OC_EOK;
//     at_response_t resp = OC_NULL;
//     oc_tick_t start_time = 0;

//     if (client == OC_NULL)
//     {
//         LOG_E("input AT client object is NULL, please create or get AT Client object!");
//         return -OC_ERROR;
//     }

//     resp = at_create_resp(64, 0, rt_tick_from_millisecond(300));
//     if (resp == OC_NULL)
//     {
//         LOG_E("no memory for AT client(vit_at) response object.");
//         return -OC_ENOMEM;
//     }

//     osMutexWait(client->lock, osWaitForever);
//     client->resp = resp;

//     start_time = rt_tick_get();

//     while (1)
//     {
//         /* Check whether it is timeout */
//         if (rt_tick_get() - start_time > rt_tick_from_millisecond(timeout))
//         {
//             LOG_E("wait AT client(vit_at) connect timeout(%d tick).", timeout);
//             result = -OC_ETIMEOUT;
//             break;
//         }

//         /* Check whether it is already connected */
//         resp->buf_len = 0;
//         resp->line_counts = 0;
//         rt_device_write( 0, "AT\r\n", 4);

//         if (osSemaphoreWait(client->resp_notice, resp->timeout) != OC_EOK)
//             continue;
//         else
//             break;
//     }

//     at_delete_resp(resp);
//     client->resp = OC_NULL;

//     osMutexRelease(client->lock);

//     return result;
// }

// /**
//  * Send data to AT server, send data don't have end sign(eg: \r\n).
//  *
//  * @param client current AT client object
//  * @param buf   send data buffer
//  * @param size  send fixed data size
//  *
//  * @return >0: send data size
//  *         =0: send failed
//  */
// oc_size_t at_client_obj_send(at_client_t client, const char *buf, oc_size_t size)
// {
//     OC_ASSERT(buf);

//     if (client == OC_NULL)
//     {
//         LOG_E("input AT Client object is NULL, please create or get AT Client object!");
//         return 0;
//     }

// #ifdef AT_PRINT_RAW_CMD
//     at_print_raw_cmd("sendline", buf, size);
// #endif

//     return rt_device_write(0, buf, size);
// }

// static oc_err_t at_client_getchar(at_client_t client, char *ch, oc_int32_t timeout)
// {
//     oc_err_t result = OC_EOK;

//     while (rt_device_read(0, ch, 1) == 0)
//     {
//         //rt_sem_control(client->rx_notice, RT_IPC_CMD_RESET, OC_NULL);

//         result = osSemaphoreWait(client->rx_notice, rt_tick_from_millisecond(timeout));
//         if (result != OC_EOK)
//         {
//             return result;
//         }
//     }

//     return OC_EOK;
// }

// /**
//  * AT client receive fixed-length data.
//  *
//  * @param client current AT client object
//  * @param buf   receive data buffer
//  * @param size  receive fixed data size
//  * @param timeout  receive data timeout (ms)
//  *
//  * @note this function can only be used in execution function of URC data
//  *
//  * @return >0: receive data size
//  *         =0: receive failed
//  */
// oc_size_t at_client_obj_recv(at_client_t client, char *buf, oc_size_t size, oc_int32_t timeout)
// {
//     oc_size_t read_idx = 0;
//     oc_err_t result = OC_EOK;
//     char ch;

//     OC_ASSERT(buf);

//     if (client == OC_NULL)
//     {
//         LOG_E("input AT Client object is NULL, please create or get AT Client object!");
//         return 0;
//     }

//     while (1)
//     {
//         if (read_idx < size)
//         {
//             result = at_client_getchar(client, &ch, timeout);
//             if (result != OC_EOK)
//             {
//                 LOG_E("AT Client receive failed, uart device get data error(%d)", result);
//                 return 0;
//             }

//             buf[read_idx++] = ch;
//         }
//         else
//         {
//             break;
//         }
//     }

// #ifdef AT_PRINT_RAW_CMD
//     at_print_raw_cmd("urc_recv", buf, size);
// #endif

//     return read_idx;
// }

// /**
//  *  AT client set end sign.
//  *
//  * @param client current AT client object
//  * @param ch the end sign, can not be used when it is '\0'
//  */
// void at_obj_set_end_sign(at_client_t client, char ch)
// {
//     if (client == OC_NULL)
//     {
//         LOG_E("input AT Client object is NULL, please create or get AT Client object!");
//         return;
//     }

//     client->end_sign = ch;
// }

// /**
//  * set URC(Unsolicited Result Code) table
//  *
//  * @param client current AT client object
//  * @param table URC table
//  * @param size table size
//  */
// int at_obj_set_urc_table(at_client_t client, const struct at_urc *urc_table, oc_size_t table_sz)
// {
//     oc_size_t idx;

//     if (client == OC_NULL)
//     {
//         LOG_E("input AT Client object is NULL, please create or get AT Client object!");
//         return -OC_ERROR;
//     }

//     for (idx = 0; idx < table_sz; idx++)
//     {
//         OC_ASSERT(urc_table[idx].cmd_prefix);
//         OC_ASSERT(urc_table[idx].cmd_suffix);
//     }

//     if (client->urc_table_size == 0)
//     {
//         client->urc_table = (struct at_urc_table *) rt_calloc(1, sizeof(struct at_urc_table));
//         if (client->urc_table == OC_NULL)
//         {
//             return -OC_ENOMEM;
//         }

//         client->urc_table[0].urc = urc_table;
//         client->urc_table[0].urc_size = table_sz;
//         client->urc_table_size++;
//     }
//     else
//     {
//         struct at_urc_table *old_urc_table = OC_NULL;
//         size_t old_table_size = client->urc_table_size * sizeof(struct at_urc_table);

//         old_urc_table = (struct at_urc_table *) rt_malloc(old_table_size);
//         if (old_urc_table == OC_NULL)
//         {
//             return -OC_ENOMEM;
//         }
//         rt_memcpy(old_urc_table, client->urc_table, old_table_size);

//         /* realloc urc table space */
//         client->urc_table = (struct at_urc_table *) rt_realloc(client->urc_table,
//                 old_table_size + sizeof(struct at_urc_table));
//         if (client->urc_table == OC_NULL)
//         {
//             rt_free(old_urc_table);
//             return -OC_ENOMEM;
//         }
//         rt_memcpy(client->urc_table, old_urc_table, old_table_size);
//         client->urc_table[client->urc_table_size].urc = urc_table;
//         client->urc_table[client->urc_table_size].urc_size = table_sz;
//         client->urc_table_size++;
        
//         rt_free(old_urc_table);
//     }

//     return OC_EOK;
// }

// /**
//  * get AT client object by AT device name.
//  *
//  * @dev_name AT client device name
//  *
//  * @return AT client object
//  */
// at_client_t at_client_get(const char *dev_name)
// {
//     int idx = 0;

//     RT_ASSERT(dev_name);

//     for (idx = 0; idx < AT_CLIENT_NUM_MAX; idx++)
//     {
//         if (rt_strcmp(at_client_table[idx].device->parent.name, dev_name) == 0)
//         {
//             return &at_client_table[idx];
//         }
//     }

//     return OC_NULL;
// }

// /**
//  * get first AT client object in the table.
//  *
//  * @return AT client object
//  */
// at_client_t at_client_get_first(void)
// {
//     if (at_client_table[0].device == OC_NULL)
//     {
//         return OC_NULL;
//     }

//     return &at_client_table[0];
// }

// static const struct at_urc *get_urc_obj(at_client_t client)
// {
//     oc_size_t i, j, prefix_len, suffix_len;
//     oc_size_t bufsz;
//     char *buffer = OC_NULL;
//     const struct at_urc *urc = OC_NULL;
//     struct at_urc_table *urc_table = OC_NULL;

//     if (client->urc_table == OC_NULL)
//     {
//         return OC_NULL;
//     }

//     buffer = client->recv_line_buf;
//     bufsz = client->recv_line_len;

//     for (i = 0; i < client->urc_table_size; i++)
//     {
//         for (j = 0; j < client->urc_table[i].urc_size; j++)
//         {
//             urc_table = client->urc_table + i;
//             urc = urc_table->urc + j;

//             prefix_len = strlen(urc->cmd_prefix);
//             suffix_len = strlen(urc->cmd_suffix);
//             if (bufsz < prefix_len + suffix_len)
//             {
//                 continue;
//             }
//             if ((prefix_len ? !strncmp(buffer, urc->cmd_prefix, prefix_len) : 1)
//                     && (suffix_len ? !strncmp(buffer + bufsz - suffix_len, urc->cmd_suffix, suffix_len) : 1))
//             {
//                 return urc;
//             }
//         }
//     }

//     return OC_NULL;
// }

// static int at_recv_readline(at_client_t client)
// {
//     oc_size_t read_len = 0;
//     char ch = 0, last_ch = 0;
//     oc_bool_t is_full = OC_FALSE;

//     memset(client->recv_line_buf, 0x00, client->recv_bufsz);
//     client->recv_line_len = 0;

//     while (1)
//     {
//         at_client_getchar(client, &ch, osWaitForever);

//         if (read_len < client->recv_bufsz)
//         {
//             client->recv_line_buf[read_len++] = ch;
//             client->recv_line_len = read_len;
//         }
//         else
//         {
//             is_full = OC_TRUE;
//         }

//         /* is newline or URC data */
//         if ((ch == '\n' && last_ch == '\r') || (client->end_sign != 0 && ch == client->end_sign)
//                 || get_urc_obj(client))
//         {
//             if (is_full)
//             {
//                 LOG_E("read line failed. The line data length is out of buffer size(%d)!", client->recv_bufsz);
//                 rt_memset(client->recv_line_buf, 0x00, client->recv_bufsz);
//                 client->recv_line_len = 0;
//                 return -OC_EFULL;
//             }
//             break;
//         }
//         last_ch = ch;
//     }

// #ifdef AT_PRINT_RAW_CMD
//     at_print_raw_cmd("recvline", client->recv_line_buf, read_len);
// #endif

//     return read_len;
// }

// static void client_parser(at_client_t client)
// {
//     const struct at_urc *urc;

//     while(1)
//     {
//         if (at_recv_readline(client) > 0)
//         {
//             if ((urc = get_urc_obj(client)) != OC_NULL)
//             {
//                 /* current receive is request, try to execute related operations */
//                 if (urc->func != OC_NULL)
//                 {
//                     urc->func(client, client->recv_line_buf, client->recv_line_len);
//                 }
//             }
//             else if (client->resp != OC_NULL)
//             {
//                 at_response_t resp = client->resp;

//                 /* current receive is response */
//                 client->recv_line_buf[client->recv_line_len - 1] = '\0';
//                 if (resp->buf_len + client->recv_line_len < resp->buf_size)
//                 {
//                     /* copy response lines, separated by '\0' */
//                     rt_memcpy(resp->buf + resp->buf_len, client->recv_line_buf, client->recv_line_len);

//                     /* update the current response information */
//                     resp->buf_len += client->recv_line_len;
//                     resp->line_counts++;
//                 }
//                 else
//                 {
//                     client->resp_status = AT_RESP_BUFF_FULL;
//                     LOG_E("Read response buffer failed. The Response buffer size is out of buffer size(%d)!", resp->buf_size);
//                 }
//                 /* check response result */
//                 if (rt_memcmp(client->recv_line_buf, AT_RESP_END_OK, rt_strlen(AT_RESP_END_OK)) == 0
//                         && resp->line_num == 0)
//                 {
//                     /* get the end data by response result, return response state END_OK. */
//                     client->resp_status = AT_RESP_OK;
//                 }
//                 else if (rt_strstr(client->recv_line_buf, AT_RESP_END_ERROR)
//                         || (rt_memcmp(client->recv_line_buf, AT_RESP_END_FAIL, rt_strlen(AT_RESP_END_FAIL)) == 0))
//                 {
//                     client->resp_status = AT_RESP_ERROR;
//                 }
//                 else if (resp->line_counts == resp->line_num && resp->line_num)
//                 {
//                     /* get the end data by response line, return response state END_OK.*/
//                     client->resp_status = AT_RESP_OK;
//                 }
//                 else
//                 {
//                     continue;
//                 }

//                 client->resp = OC_NULL;
//                 osSemaphoreRelease(client->resp_notice);
//             }
//             else
//             {
// //                log_d("unrecognized line: %.*s", client->recv_line_len, client->recv_line_buf);
//             }
//         }
//     }
// }

// static oc_err_t at_client_rx_ind(oc_device_t dev, oc_size_t size)
// {
//     int idx = 0;

//     for (idx = 0; idx < AT_CLIENT_NUM_MAX; idx++)
//     {
//         if (at_client_table[idx].device == dev && size > 0)
//         {
//             osSemaphoreRelease(at_client_table[idx].rx_notice);
//         }
//     }

//     return OC_EOK;
// }

// /* initialize the client object parameters */
// static int at_client_para_init(at_client_t client)
// {
// #define AT_CLIENT_LOCK_NAME            "at_c"
// #define AT_CLIENT_SEM_NAME             "at_cs"
// #define AT_CLIENT_RESP_NAME            "at_cr"
// #define AT_CLIENT_THREAD_NAME          "at_clnt"

//     int result = OC_EOK;
//     static int at_client_num = 0;
//     char name[OC_NAME_MAX];

//     client->status = AT_STATUS_UNINITIALIZED;

//     client->recv_line_len = 0;
//     client->recv_line_buf = (char *) rt_calloc(1, client->recv_bufsz);
//     if (client->recv_line_buf == OC_NULL)
//     {
//         LOG_E("AT client initialize failed! No memory for receive buffer.");
//         result = -OC_ENOMEM;
//         goto __exit;
//     }


//     rt_snprintf(name, OC_NAME_MAX, "%s%d", AT_CLIENT_LOCK_NAME, at_client_num);
//     osMutexDef(name);
//     client->lock = osMutexCreate(osMutex(name));
//     if (client->lock == OC_NULL)
//     {
//         LOG_E("AT client initialize failed! at_client_recv_lock create failed!");
//         result = -OC_ENOMEM;
//         goto __exit;
//     }

//     rt_snprintf(name, OC_NAME_MAX, "%s%d", AT_CLIENT_SEM_NAME, at_client_num);
//     osSemaphoreDef(name);
//     client->rx_notice = osSemaphoreCreate(osseamphore(name), 0);
//     if (client->rx_notice == OC_NULL)
//     {
//         LOG_E("AT client initialize failed! at_client_notice semaphore create failed!");
//         result = -OC_ENOMEM;
//         goto __exit;
//     }

//     rt_snprintf(name, OC_NAME_MAX, "%s%d", AT_CLIENT_RESP_NAME, at_client_num);
//     osSemaphoreDef(name);
//     client->resp_notice = osSemaphoreCreate(osseamphore(name), 0);
//     if (client->resp_notice == OC_NULL)
//     {
//         LOG_E("AT client initialize failed! at_client_resp semaphore create failed!");
//         result = -OC_ENOMEM;
//         goto __exit;
//     }

//     client->urc_table = OC_NULL;
//     client->urc_table_size = 0;

//     rt_snprintf(name, OC_NAME_MAX, "%s%d", AT_CLIENT_THREAD_NAME, at_client_num);
//     client->parser = rt_thread_create(name,
//                                      (void (*)(void *parameter))client_parser,
//                                      client,
//                                      1024 + 512,
//                                      RT_THREAD_PRIORITY_MAX / 3 - 1,
//                                      5);
//     if (client->parser == OC_NULL)
//     {
//         result = -OC_ENOMEM;
//         goto __exit;
//     }

// __exit:
//     if (result != OC_EOK)
//     {
//         if (client->lock)
//         {
//             osMutexDelete(client->lock);
//         }

//         if (client->rx_notice)
//         {
//             osSemaphoreDelete(client->rx_notice);
//         }

//         if (client->resp_notice)
//         {
//             osSemaphoreDelete(client->resp_notice);
//         }

//         if (client->device)
//         {
//             rt_device_close(client->device);
//         }

//         if (client->recv_line_buf)
//         {
//             free(client->recv_line_buf);
//         }

//         memset(client, 0x00, sizeof(struct at_client));
//     }
//     else
//     {
//         at_client_num++;
//     }

//     return result;
// }

// /**
//  * AT client initialize.
//  *
//  * @param dev_name AT client device name
//  * @param recv_bufsz the maximum number of receive buffer length
//  *
//  * @return 0 : initialize success
//  *        -1 : initialize failed
//  *        -5 : no memory
//  */
// int at_client_init(const char *dev_name,  oc_size_t recv_bufsz)
// {
//     int idx = 0;
//     int result = OC_EOK;
//     oc_err_t open_result = OC_EOK;
//     at_client_t client = OC_NULL;

//     RT_ASSERT(dev_name);
//     RT_ASSERT(recv_bufsz > 0);

//     for (idx = 0; idx < AT_CLIENT_NUM_MAX && at_client_table[idx].device; idx++);

//     if (idx >= AT_CLIENT_NUM_MAX)
//     {
//         LOG_E("AT client initialize failed! Check the maximum number(%d) of AT client.", AT_CLIENT_NUM_MAX);
//         result = -OC_EFULL;
//         goto __exit;
//     }

//     client = &at_client_table[idx];
//     client->recv_bufsz = recv_bufsz;

//     result = at_client_para_init(client);
//     if (result != OC_EOK)
//     {
//         goto __exit;
//     }

//     /* find and open command device */
//     client->device = rt_device_find(dev_name);
//     if (client->device)
//     {
//         RT_ASSERT(client->device->type == RT_Device_Class_Char);

//         /* using DMA mode first */
//         open_result = rt_device_open(client->device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);
//         /* using interrupt mode when DMA mode not supported */
//         if (open_result == -RT_EIO)
//         {
//             open_result = rt_device_open(client->device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
//         }
//         RT_ASSERT(open_result == OC_EOK);
// A
//         rt_device_set_rx_indicate(client->device, at_client_rx_ind);
//     }
//     else
//     {
//         LOG_E("AT client initialize failed! Not find the device(%s).", dev_name);
//         result = -OC_ERROR;
//         goto __exit;
//     }

// __exit:
//     if (result == OC_EOK)
//     {
//         client->status = AT_STATUS_INITIALIZED;

//         rt_thread_startup(client->parser);

//         LOG_I("AT client(V%s) on device %s initialize success.", AT_SW_VERSION, dev_name);
//     }
//     else
//     {
//         LOG_E("AT client(V%s) on device %s initialize failed(%d).", AT_SW_VERSION, dev_name, result);
//     }

//     return result;


// #endif



