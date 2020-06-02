/*********************************************************
 *  @file    cm_ssl.h
 *  @brief   ML302 OpenCPU SSL header file
 *  Copyright (c) 2019 China Mobile IOT.
 *  All rights reserved.
 *  created by XieGangLiang 2019/04/28
 ********************************************************/
#ifndef __CM_SSL_H__
#define __CM_SSL_H__
#include "cm_sys.h"
#include "wolfssl/openssl/crypto.h"
#include "wolfssl/openssl/ssl.h"
#include "wolfssl/openssl/err.h"
#include "wolfssl/openssl/rand.h"
#include "wolfssl/internal.h"


typedef struct ssl_file {
	uint16_t file_len;
	uint8_t data[0];
} ssl_file_t;

 typedef struct ssl_ctx {
	WOLFSSL_CTX *wolf_ctx;

	uint16_t cipher_suit;
	uint8_t ssl_ver;
	uint8_t sec_lvl;

	uint8_t *ca_cert;
	uint8_t *client_cert;
	uint8_t *client_key;

	uint8_t ign_rtctime;

	uint8_t http_enable;
	int8_t http_index;

	uint8_t smtp_style;
	int8_t smtp_index;

	uint8_t used_cnt;
	uint8_t init_flg;
} ssl_ctx_t;

/**
 *  \brief SSL上下文结构体默认初始化
 *  
 *  \param [in] ssl_ctx SSL上下文结构体指针
 *  \return 空
 *  
 *  \details More details
 */
void set_ssl_ctx_default(ssl_ctx_t *ssl_ctx);
/**
 *  \brief 初始化WoffSSL
 *  
 *  \param [in] ssl_ctx SSL上下文结构体
 *  \return 0：成功 其他：失败
 *  
 *  \details More details
 */
int8_t wolf_ssl_init(ssl_ctx_t *ssl_ctx);

#endif