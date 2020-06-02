/*********************************************************
 *  @file    cm_ssl.c
 *  @brief   ML302 OpenCPU  SSL functions
 *  Copyright (c) 2019 China Mobile IOT.
 *  All rights reserved.
 *  created by XieGangLiang 2019/12/2
 ********************************************************/
#include "cm_ssl.h"


static uint8_t global_cipher_list[1024] = {0};





void set_ssl_ctx_default(ssl_ctx_t *ssl_ctx)
{
	ssl_ctx->init_flg = 0;
	ssl_ctx->ssl_ver = 4;
	ssl_ctx->cipher_suit = 0xffff;
	ssl_ctx->sec_lvl = 0;
	ssl_ctx->ca_cert = NULL;
	ssl_ctx->client_cert = NULL;
	ssl_ctx->client_key = NULL;
	ssl_ctx->ign_rtctime = 0;
	ssl_ctx->http_enable = 0;
	ssl_ctx->http_index = -1;
	ssl_ctx->smtp_style = 0;
	ssl_ctx->smtp_index = -1;
	ssl_ctx->wolf_ctx = NULL;

	ssl_ctx->used_cnt = 0;
}
static wolfSSL_method_func ssl_get_ssl_version_method(ssl_ctx_t *ssl_ctx)
{
	if (ssl_ctx == NULL)
		return (wolfSSL_method_func)wolfTLSv1_1_client_method_ex;

	if (ssl_ctx->ssl_ver == 0)
		return (wolfSSL_method_func)wolfSSLv3_client_method;
	if (ssl_ctx->ssl_ver == 1)
		return (wolfSSL_method_func)wolfTLSv1_client_method;
	if (ssl_ctx->ssl_ver == 2)
		return (wolfSSL_method_func)wolfTLSv1_1_client_method;
	if (ssl_ctx->ssl_ver == 3)
		return (wolfSSL_method_func)wolfTLSv1_2_client_method;
	if (ssl_ctx->ssl_ver == 4)
		return (wolfSSL_method_func)wolfSSLv23_client_method; //use highest possible version
		
	return (wolfSSL_method_func)NULL;
}
static int8_t ssl_get_full_cipher_list(uint8_t *buf)
{
	uint8_t *cipher_item = NULL;
	uint16_t len = 0;

	if (buf == NULL)
		return -1;

	cipher_item = (uint8_t *)GetCipherNameInternal(0, TLS_RSA_WITH_AES_256_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, TLS_RSA_WITH_AES_128_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, SSL_RSA_WITH_RC4_128_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, SSL_RSA_WITH_RC4_128_MD5);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, SSL_RSA_WITH_3DES_EDE_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = (uint8_t *)GetCipherNameInternal(0, TLS_RSA_WITH_AES_256_CBC_SHA256);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s", cipher_item);

	if (strlen(buf) == 0)
		return -1;

	return 0;
}

static uint8_t wolf_ssl_init_flg = 0;

int8_t wolf_ssl_init(ssl_ctx_t *ssl_ctx)
{
	wolfSSL_method_func method = NULL;
	int32_t ret = 0;

	if (ssl_ctx == NULL)
	{
		cm_sys_log("wolf_ssl_init input error");
		return -1;
	}

	if (wolf_ssl_init_flg == 0)
	{
		wolf_ssl_init_flg = 1;
		wolfSSL_Debugging_ON();
		wolfSSL_Init();
	}

	method = ssl_get_ssl_version_method(ssl_ctx);
	if (ssl_ctx->wolf_ctx == NULL)
	{
		ssl_ctx->wolf_ctx = SSL_CTX_new(method(NULL)); 
		if (ssl_ctx->wolf_ctx == NULL)
		{
			cm_sys_log("wolf_ssl_init ssl ctx new error");
			return -1;
		}
	}
	else if (ssl_ctx->used_cnt > 0)
	{
		cm_sys_log("wolf_ssl_init ssl ctx busy, can't reconfig");
		return 0;
	}

	if (ssl_ctx->cipher_suit != 0xffff)
	{
		ret = wolfSSL_CTX_set_cipher_list(ssl_ctx->wolf_ctx, GetCipherNameInternal(0, ssl_ctx->cipher_suit));
	}
	else
	{
		
		ssl_get_full_cipher_list(global_cipher_list);
		ret = wolfSSL_CTX_set_cipher_list(ssl_ctx->wolf_ctx, (const char *)global_cipher_list);
	}

	if (ret != WOLFSSL_SUCCESS)
	{
		cm_sys_log("wolf_ssl_init cipher set error");
		return -1;
	}

	if (ssl_ctx->sec_lvl == 0)
		wolfSSL_CTX_set_verify(ssl_ctx->wolf_ctx, WOLFSSL_VERIFY_NONE, NULL);
	else
	{
		int32_t format = SSL_FILETYPE_PEM;
		int ret = 0;
        //加载密钥证书
		cm_printf("[HTTP]sec level 1\n");
		wolfSSL_CTX_set_verify(ssl_ctx->wolf_ctx, WOLFSSL_VERIFY_PEER, NULL);
		ret = wolfSSL_CTX_load_verify_buffer(ssl_ctx->wolf_ctx, ssl_ctx->ca_cert, strlen(ssl_ctx->ca_cert), format);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_printf("[SSL]ca load error\n");
			return -1;
		}
	}

	if (ssl_ctx->sec_lvl == 2) 
	{
		cm_printf("[SSL]sec level 2\n");
		int ret = 0;
		int32_t format = SSL_FILETYPE_PEM;
		ret = wolfSSL_CTX_use_certificate_chain_buffer_format(ssl_ctx->wolf_ctx, ssl_ctx->client_cert, strlen(ssl_ctx->client_cert), format);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_printf("%s local ca file use error", __func__);
			return -1;
		}

		ret = wolfSSL_CTX_use_PrivateKey_buffer(ssl_ctx->wolf_ctx, ssl_ctx->client_key, strlen(ssl_ctx->client_key), format);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_printf("%s local key file use error", __func__);
			return -1;
		}
	}

	return 0;
}


