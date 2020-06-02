#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stddef.h"

//#include "cmiot_include.h"
#include "wolfssl/openssl/crypto.h"
#include "wolfssl/openssl/ssl.h"
#include "wolfssl/openssl/err.h"
#include "wolfssl/openssl/rand.h"

#include "cmiot_ssl.h"

/*#include "cm_utils_base64.h"
#include "cm_utils_hmac.h"
#include "cm_utils_md5.h"
#include "cm_utils_sha1.h"
#include "cm_utils_sha256.h"
#include "cm_aes.h"*/

#include "wolfssl/internal.h"
#include "cm_sys.h"

static ssl_ctx_t ssl_ctx_array[SSL_CTX_NUM] = {0};

//SSL初始设置
static void set_ssl_ctx_default(ssl_ctx_t *ssl_ctx)
{
	ssl_ctx->init_flg = SSL_CTX_INITED;
	ssl_ctx->ssl_ver = 4;
	ssl_ctx->cipher_suit = 0xffff;
	ssl_ctx->sec_lvl = 0;
	ssl_ctx->ca_cert = NULL;
	ssl_ctx->client_cert = NULL;
	ssl_ctx->client_key = NULL;
	memset(ssl_ctx->tcp_enable,0,SSL_TCP_ENABLE_MAXLEN);
	ssl_ctx->http_enable = 0;
	memset(ssl_ctx->mqtt_enable,0,SSL_MQTT_ENABLE_MAXLEN);
	ssl_ctx->wolf_ctx = NULL;
	ssl_ctx->cert_type = 1;
	ssl_ctx->used_cnt = 0;
	ssl_ctx->ssl = NULL;
}
//通过ssl context获取ssl id
INT8 get_ssl_ctx_index(ssl_ctx_t *ssl_ctx)
{
	UINT8 i = 0;

	if (ssl_ctx == NULL)
		return -1;

	while (i < SSL_CTX_NUM)
	{
		if (ssl_ctx_array+ i == ssl_ctx)
		{
			return i;
		}
		i++;
	}

	return -1;
}
//通过ssl id获取ssl context
ssl_ctx_t *get_ssl_ctx_by_index(UINT8 id)
{
	ssl_ctx_t *ctx = NULL;

	if (id >= SSL_CTX_NUM)
		return NULL;

	ctx = ssl_ctx_array + id;
	if (SSL_CTX_INITED != ctx->init_flg)
		set_ssl_ctx_default(ctx);

	return ctx;
}

ssl_ctx_t *chack_ssl_enable(cm_ssl_connect_type connect_type,uint8_t connect_id)
{
	int i;
	ssl_ctx_t *ctx = NULL;
	if(connect_type == SSL_CONN_TCP)
	{
		if(connect_id > 8 || connect_id < 1)
		{
			cm_sys_log("%s:ssl tcp connect_id err",__func__);
			return NULL;
		}
	}
	else if(connect_type == SSL_CONN_HTTP)
	{
		cm_sys_log("%s:ssl http",__func__);
	}
	else if(connect_type == SSL_CONN_MQTT)
	{
		if(connect_id > 5)
		{
			cm_sys_log("%s:ssl mqtt connect_id err",__func__);
			return NULL;
		}
	}
	else
		return NULL;
	for(i=0;i<SSL_CTX_NUM;i++)
	{
		if(ssl_ctx_array[i].used_cnt>0)
		{
			if(connect_type == 0)
			{
				if(ssl_ctx_array[i].tcp_enable[connect_id] == 1)
				{
					ctx = ssl_ctx_array + i;
					return ctx;
				}
			}
			else if(connect_type == 1)
			{
				if(ssl_ctx_array[i].http_enable == 1)
				{
					ctx = ssl_ctx_array + i;
					return ctx;
				}
			}
			else if(connect_type == 2)
			{
				if(ssl_ctx_array[i].mqtt_enable[connect_id] == 1)
				{
					ctx = ssl_ctx_array + i;
					return ctx;
				}
			}
		}
	}
	return NULL;
}

void ssl_ctx_used(ssl_ctx_t *ctx)
{
	if (ctx == NULL)
		return ;

	ctx->used_cnt++;
}

void ssl_ctx_unused(ssl_ctx_t *ctx)
{
	if (ctx == NULL)
		return ;

	if (ctx->used_cnt > 0)
	{
		memset(ctx->tcp_enable,0,SSL_TCP_ENABLE_MAXLEN);
		ctx->http_enable = 0;
		memset(ctx->mqtt_enable,0,SSL_MQTT_ENABLE_MAXLEN);
		ctx->used_cnt--;
	}
	
}
//获取SSL版本
static wolfSSL_method_func ssl_get_ssl_version_method(ssl_ctx_t *ssl_ctx)
{
	if (ssl_ctx == NULL)
		return wolfTLSv1_1_client_method_ex;

	if (ssl_ctx->ssl_ver == 0)
		return wolfSSLv3_client_method;
	if (ssl_ctx->ssl_ver == 1)
		return wolfTLSv1_client_method;
	if (ssl_ctx->ssl_ver == 2)
		return wolfTLSv1_1_client_method;
	if (ssl_ctx->ssl_ver == 3)
		return wolfTLSv1_2_client_method;
	if (ssl_ctx->ssl_ver == 4)
		return wolfSSLv23_client_method; //use highest possible version
}
//获取加密套件
static int8_t ssl_get_full_cipher_list(UINT8 *buf)
{
	uint8_t *cipher_item = NULL;
	uint16_t len = 0;

	if (buf == NULL)
		return -1;

	cipher_item = GetCipherNameInternal(0, TLS_RSA_WITH_AES_256_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = GetCipherNameInternal(0, TLS_RSA_WITH_AES_128_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = GetCipherNameInternal(0, SSL_RSA_WITH_RC4_128_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = GetCipherNameInternal(0, SSL_RSA_WITH_RC4_128_MD5);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = GetCipherNameInternal(0, SSL_RSA_WITH_3DES_EDE_CBC_SHA);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s:", cipher_item);

	cipher_item = GetCipherNameInternal(0, TLS_RSA_WITH_AES_256_CBC_SHA256);
	if (cipher_item != NULL)
		len += sprintf(buf + len, "%s", cipher_item);

	if (strlen(buf) == 0)
		return -1;

	return 0;
}

static uint8_t wolf_ssl_init_flg = 0;
int8_t wolf_ssl_init(ssl_ctx_t *ssl_ctx)
{
	cm_sys_log("%s", __func__);
	wolfSSL_method_func method = NULL;
	int32_t ret = 0;

	if (ssl_ctx == NULL)
	{
		cm_sys_log("%s input error", __func__);
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
			cm_sys_log("%s ssl ctx new error", __func__);
			return -1;
		}
	}
	else if (ssl_ctx->used_cnt > 0)
	{
		cm_sys_log("%s ssl ctx busy, can't reconfig", __func__);
		return 0;
	}

	if (ssl_ctx->cipher_suit != 0xffff)
	{
		ret = wolfSSL_CTX_set_cipher_list(ssl_ctx->wolf_ctx, GetCipherNameInternal(0, ssl_ctx->cipher_suit));
	}
	else
	{
		uint8_t cipher_list[1024] = {0};
		ssl_get_full_cipher_list(cipher_list);
		ret = wolfSSL_CTX_set_cipher_list(ssl_ctx->wolf_ctx, cipher_list);
	}

	if (ret != WOLFSSL_SUCCESS)
	{
		cm_sys_log("%s cipher set error", __func__);
		return -1;
	}
	
	if (ssl_ctx->sec_lvl == 0)
		wolfSSL_CTX_set_verify(ssl_ctx->wolf_ctx, WOLFSSL_VERIFY_NONE, NULL);
	else
	{
		if(ssl_ctx->ca_cert_len <=0 || ssl_ctx->ca_cert == NULL )
		{
			cm_sys_log("%s ca file error", __func__);
			return -1;
		}
		wolfSSL_CTX_set_verify(ssl_ctx->wolf_ctx, WOLFSSL_VERIFY_PEER, NULL);
		cm_sys_log("%s ssl_ctx->ca_cert_len=%d,ssl_ctx->cert_type=%d,ssl_ctx->ca_cert=%s", __func__,ssl_ctx->ca_cert_len,ssl_ctx->cert_type,ssl_ctx->ca_cert);
		ret = wolfSSL_CTX_load_verify_buffer(ssl_ctx->wolf_ctx, ssl_ctx->ca_cert, ssl_ctx->ca_cert_len, ssl_ctx->cert_type);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_sys_log("%s ca file set error", __func__);
			return -1;
		}
	}

	if (ssl_ctx->sec_lvl == 2)
	{
		
		if(ssl_ctx->client_cert_len <=0 || ssl_ctx->client_cert == NULL )
		{
			cm_sys_log("%s ca file error", __func__);
			return -1;
		}
		ret = wolfSSL_CTX_use_certificate_chain_buffer_format(ssl_ctx->wolf_ctx, ssl_ctx->client_cert, ssl_ctx->client_cert_len, ssl_ctx->cert_type);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_sys_log("%s local ca file use error", __func__);
			return -1;
		}
		if(ssl_ctx->client_key_len <=0 || ssl_ctx->client_key == NULL )
		{
			cm_sys_log("%s ca file error", __func__);
			return -1;
		}

		ret = wolfSSL_CTX_use_PrivateKey_buffer(ssl_ctx->wolf_ctx, ssl_ctx->client_key, ssl_ctx->client_key_len, ssl_ctx->cert_type);
		if (ret != WOLFSSL_SUCCESS)
		{
			cm_sys_log("%s local key file use error", __func__);
			return -1;
		}
	}

	return 0;
}
/*************************************************
* Function: cm_ssl_init
* Description: ssl初始化
* Author: chen
* Parameter:
				ssl_cfg		参数设置
					ssl_id			ssl id,支持(0-7)8路ssl
					cipher_suit	 	设置支持的加密套件,可设置为：
									TLS_RSA_WITH_AES_256_CBC_SHA；TLS_RSA_WITH_AES_128_CBC_SHA
									TLS_RSA_WITH_RC4_128_SHA；TLS_RSA_WITH_RC4_128_MD5
									TLS_RSA_WITH_3DES_EDE_CBC_SHA；TLS_RSA_WITH_AES_256_CBC_SHA256
									0xFFFF:支持所有加密格式
					ssl_ver			设置SSL版本：0：SSL3.0; 1:TLS1.0; 2:TLS1.1; 3:TLS1.2; 4:TLS1.3
					sec_lvl			设置服务等级 0：无证书验证；1单项认证（CA证书认证）；2双向认证
					cert_type		设置证书类型 1:PEM;2:ASN1;3:RAW
					*ca_cert		CA证书
					ca_cert_len		CA证书长度
					*client_cert	客户端证书
					client_cert_len 客户端证书长度
					*client_key		客户端密钥
					client_key_len	客户端密钥长度

				（设置服务等级sec_lvl=0时，证书可设置为NULL,sec_lvl=1时,CA证书必须设置，
				sec_lvl=2时，证书相关三个参数必须设置）
* Returns: 0/-1
* History:
*************************************************/

int32_t cm_ssl_init(cm_ssl_cfg ssl_cfg)
{
	cm_sys_log("%s:id %d", __func__,ssl_cfg.ssl_id);
	if(ssl_cfg.ssl_id>=SSL_CTX_NUM)
	{
		cm_sys_log("%s ssl_id error %d",__func__,ssl_cfg.ssl_id);
		return -1;
	}
	if((ssl_cfg.cipher_suit != TLS_RSA_WITH_AES_256_CBC_SHA) &&
		(ssl_cfg.cipher_suit != TLS_RSA_WITH_AES_128_CBC_SHA) &&
		(ssl_cfg.cipher_suit != TLS_RSA_WITH_RC4_128_SHA) &&
		(ssl_cfg.cipher_suit != TLS_RSA_WITH_RC4_128_MD5) &&
		(ssl_cfg.cipher_suit != TLS_RSA_WITH_3DES_EDE_CBC_SHA) &&
		(ssl_cfg.cipher_suit != TLS_RSA_WITH_AES_256_CBC_SHA256) &&
		(ssl_cfg.cipher_suit != TLS_RSA_WITH_SUIT_ALL) )
	{
		cm_sys_log("%s cipher_suit error %d",__func__,ssl_cfg.cipher_suit);
		return -1;
	}
		
	if(ssl_cfg.ssl_ver > 4 )
	{
		cm_sys_log("%s ssl_ver error %d",__func__,ssl_cfg.ssl_ver);
		return -1;
	}
	if(ssl_cfg.sec_lvl > 2 )
	{
		cm_sys_log("%s sec_lvl error %d",__func__,ssl_cfg.sec_lvl);
		return -1;
	}

	if(ssl_cfg.cert_type > 3 || ssl_cfg.cert_type < 1)
	{
		cm_sys_log("%s cert_type error %d",__func__,ssl_cfg.cert_type);
		return -1;
	}
	if(ssl_cfg.sec_lvl>0)
	{
		if(ssl_cfg.ca_cert == NULL || ssl_cfg.ca_cert_len <= 0)
		{
			cm_sys_log("%s ca_cert error %d,%d",__func__,ssl_cfg.ca_cert,ssl_cfg.ca_cert_len);
			return -1;
		}
	}
	if(ssl_cfg.sec_lvl>1)
	{
		if(ssl_cfg.client_cert == NULL || ssl_cfg.client_cert_len <= 0)
		{
			cm_sys_log("%s client_cert error %d,%d",__func__,ssl_cfg.client_cert,ssl_cfg.client_cert_len);
			return -1;
		}
		if(ssl_cfg.client_key == NULL || ssl_cfg.client_key_len <= 0)
		{
			cm_sys_log("%s client_key error %d,%d",__func__,ssl_cfg.client_key,ssl_cfg.client_key_len);
			return -1;
		}
	}
	ssl_ctx_t *p_ssl_ctx = NULL;
	p_ssl_ctx = get_ssl_ctx_by_index(ssl_cfg.ssl_id);
	if (p_ssl_ctx == NULL)
	{
		cm_sys_log("%s failed to get ssl contex",__func__);
		return -1;
	}
	if(p_ssl_ctx->used_cnt > 0)
	{
		cm_sys_log("%s:ssl ctx busy, can't reconfig",__func__);
		return -1;
	}
	p_ssl_ctx->cipher_suit = ssl_cfg.cipher_suit;
	p_ssl_ctx->ssl_ver = ssl_cfg.ssl_ver;
	p_ssl_ctx->sec_lvl = ssl_cfg.sec_lvl;
	p_ssl_ctx->cert_type = ssl_cfg.cert_type;
	p_ssl_ctx->ca_cert = ssl_cfg.ca_cert;
	p_ssl_ctx->ca_cert_len = ssl_cfg.ca_cert_len;
	p_ssl_ctx->client_cert = ssl_cfg.client_cert;
	p_ssl_ctx->client_cert_len = ssl_cfg.client_cert_len;
	p_ssl_ctx->client_key = ssl_cfg.client_key;
	p_ssl_ctx->client_key_len = ssl_cfg.client_key_len;
	memset(p_ssl_ctx->tcp_enable,0,SSL_TCP_ENABLE_MAXLEN);
	p_ssl_ctx->http_enable = 0;
	memset(p_ssl_ctx->mqtt_enable,0,SSL_MQTT_ENABLE_MAXLEN);
	if(wolf_ssl_init(p_ssl_ctx)!=0)
	{
		cm_sys_log("%s wolf_ssl_init fail",__func__);
		return -1;
	}
	return 0;
}
/*************************************************
* Function: cm_ssl_start
* Description: 打开ssl功能
* Author: chen
* Parameter:
					ssl_id			ssl id,支持(0-7)8路ssl
					connect_type	指定该路ssl使能的协议
									SSL_CONN_TCP
									SSL_CONN_HTTP
									SSL_CONN_MQTT
					connect_id		指定需要使用该路ssl的协议链路
									TCP连接时：使用socked id（1-8）
									HTTP连接时：http只可存在1路，可设任意值
									MQTT连接时：使用mqtt id（0-5）
* Returns: 0/-1
* History:
*************************************************/

int32_t cm_ssl_start(uint8_t ssl_id,cm_ssl_connect_type connect_type,uint8_t connect_id)
{
	cm_sys_log("%s:id %d,connect_type %d,connect_id %d", __func__,ssl_id,connect_type,connect_id);
	if(ssl_id>=SSL_CTX_NUM )
	{
		cm_sys_log("%s input err",__func__);
		return -1;
	}
	ssl_ctx_t *p_ssl_ctx = NULL;
	p_ssl_ctx = get_ssl_ctx_by_index(ssl_id);
	if (p_ssl_ctx == NULL)
	{
		cm_sys_log("%s failed to get ssl contex",__func__);
		return -1;
	}
	if(p_ssl_ctx->used_cnt > 0)
	{
		cm_sys_log("%s:ssl ctx busy, can't reconfig",__func__);
		return -1;
	}
	if(connect_type == SSL_CONN_TCP)
	{
		if(connect_id > 8 || connect_id < 1)
		{
			cm_sys_log("%s:ssl tcp connect_id err",__func__);
			return -1;
		}
		p_ssl_ctx->tcp_enable[connect_id] = 1;
	}
	else if(connect_type == SSL_CONN_HTTP)
	{
		p_ssl_ctx->http_enable = 1;
	}
	else if(connect_type == SSL_CONN_MQTT)
	{
		if(connect_id > 5)
		{
			cm_sys_log("%s:ssl mqtt connect_id err",__func__);
			return -1;
		}
		p_ssl_ctx->mqtt_enable[connect_id] = 1;
	}
	else
	{
		return -1;
	}
	ssl_ctx_used(p_ssl_ctx);
	return 0;
}
/*************************************************
* Function: cm_ssl_stop
* Description: 关闭ssl功能
* Author: chen
* Parameter:
					ssl_id			ssl id,支持(0-7)8路ssl
					（该路ssl未存在链路使用时才可关闭）
* Returns: 0/-1
* History:
*************************************************/

int32_t cm_ssl_stop(uint8_t ssl_id)
{
	cm_sys_log("%s:id %d", __func__,ssl_id);
	if(ssl_id>=SSL_CTX_NUM)
	{
		cm_sys_log("%s input err",__func__);
		return -1;
	}
	
	ssl_ctx_t *p_ssl_ctx = NULL;
	p_ssl_ctx = get_ssl_ctx_by_index(ssl_id);
	if (p_ssl_ctx == NULL)
	{
		cm_sys_log("%s failed to get ssl contex",__func__);
		return -1;
	}
	
	if(p_ssl_ctx->ssl)
	{
		cm_sys_log("%s ssl_id(%d) is working",__func__);
		return -1;
	}
	ssl_ctx_unused(p_ssl_ctx);
	return 0;
}
