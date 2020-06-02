#ifndef __CMIOT_SSL_H__
#define __CMIOT_SSL_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stddef.h"
#include "wolfssl/openssl/crypto.h"
#include "wolfssl/openssl/ssl.h"
#include "wolfssl/openssl/err.h"
#include "wolfssl/openssl/rand.h"
#include "wolfssl/internal.h"

#define INT8 char
#define UINT8 char

#define TLS_RSA_WITH_AES_256_CBC_SHA    0x0035
#define TLS_RSA_WITH_AES_128_CBC_SHA    0x002f
#define TLS_RSA_WITH_RC4_128_SHA        0x0005
#define TLS_RSA_WITH_RC4_128_MD5        0x0004
#define TLS_RSA_WITH_3DES_EDE_CBC_SHA   0x000a
#define TLS_RSA_WITH_AES_256_CBC_SHA256 0x003d
#define TLS_RSA_WITH_SUIT_ALL			0xFFFF

#define SSL_CTX_NUM 8
#define SSL_CTX_INITED 0xA5 

#define SSL_FILE_MAX_SIZE 10240		//限制证书长度10K

#define SSL_TCP_ENABLE_MAXLEN	 9	//tcp可以使用SSL的最大路数
#define SSL_MQTT_ENABLE_MAXLEN	 6	//mqtt可以使用SSL的最大路数
typedef enum
{
	SSL_3_0=0,
	TLS_1_0,
	TLS_1_1,
	TLS_1_2,
	TLS_1_3,
}cm_ssl_ver;
	
typedef enum
{
	SSL_AUTH_NONE=0,
	SSL_AUTH_SERVER,
	SSL_AUTH_BOTH,
}cm_sec_lvl;
typedef enum
{
	CERT_TYPE_PEM=1,
	CERT_TYPE_ASN1,
	CERT_TYPE_RAW,
}cm_cert_type;
typedef struct ssl_ctx {
	WOLFSSL_CTX *wolf_ctx;	//底层设置的wolf_ssl信息
	WOLFSSL *ssl;

	uint16_t cipher_suit;  //支持的加密套件
							//TLS_RSA_WITH_AES_256_CBC_SHA；TLS_RSA_WITH_AES_128_CBC_SHA
							//TLS_RSA_WITH_RC4_128_SHA；TLS_RSA_WITH_RC4_128_MD5
							//TLS_RSA_WITH_3DES_EDE_CBC_SHA；TLS_RSA_WITH_AES_256_CBC_SHA256
							//TLS_RSA_WITH_SUIT_ALL:支持所有加密格式
	uint8_t ssl_ver;		//SSL版本：0：SSL3.0; 1:TLS1.0; 2:TLS1.1; 3:TLS1.2; 4:TLS1.3
	uint8_t sec_lvl;		//服务等级 0：无证书验证；1单项认证（CA证书认证）；2双向认证

	uint8_t cert_type;	//1:PEM;2:ASN1;3:RAW
	uint8_t *ca_cert;		//CA证书
	uint16_t ca_cert_len;
	uint8_t *client_cert;	//客户端证书
	uint16_t client_cert_len;
	uint8_t *client_key;	//客户端密钥（双向认证使用）
	uint16_t client_key_len;

	uint8_t tcp_enable[SSL_TCP_ENABLE_MAXLEN];   //tcp ssl 使能,指定socket（1-8）与之对应

	uint8_t http_enable;  //http ssl 使能（HTTP只能使用1路）

	uint8_t mqtt_enable[SSL_MQTT_ENABLE_MAXLEN];	//mqtt ssl 使能，指定MQTT ID（0-5）与之对应

	uint8_t used_cnt;		//该ssl ctx是否已使用
	uint8_t init_flg;		//初始化标准
} ssl_ctx_t;
typedef struct cm_ssl_cfg
{
	uint8_t ssl_id;		//ssl id(0-7)
	uint16_t cipher_suit;  //支持的加密套件
							//TLS_RSA_WITH_AES_256_CBC_SHA；TLS_RSA_WITH_AES_128_CBC_SHA
							//TLS_RSA_WITH_RC4_128_SHA；TLS_RSA_WITH_RC4_128_MD5
							//TLS_RSA_WITH_3DES_EDE_CBC_SHA；TLS_RSA_WITH_AES_256_CBC_SHA256
							//TLS_RSA_WITH_SUIT_ALL:支持所有加密格式
	cm_ssl_ver ssl_ver;		//SSL版本：0：SSL3.0; 1:TLS1.0; 2:TLS1.1; 3:TLS1.2; 4:TLS1.3
	cm_sec_lvl sec_lvl;		//服务等级 0：无证书验证；1单项认证（CA证书认证）；2双向认证

	cm_cert_type cert_type;	//证书类型 1:PEM;2:ASN1;3:RAW
	uint8_t *ca_cert;		//CA证书
	uint16_t ca_cert_len;
	uint8_t *client_cert;	//客户端证书
	uint16_t client_cert_len;
	uint8_t *client_key;	//客户端密钥（双向认证使用）
	uint16_t client_key_len;

}cm_ssl_cfg;

typedef enum {
	SSL_CONN_TCP = 0,
	SSL_CONN_HTTP,
	SSL_CONN_MQTT,
} cm_ssl_connect_type;

int32_t cm_utils_base64encode(const uint8_t *data, uint32_t inputLength, uint32_t outputLenMax,
                              uint8_t *encodedData, uint32_t *outputLength);
int32_t cm_utils_base64decode(const uint8_t *data, uint32_t inputLength, uint32_t outputLenMax,
                              uint8_t *decodedData, uint32_t *outputLength);
void cm_utils_hmac_md5(const char *msg, int msg_len, char *digest, const char *key, int key_len);
void cm_utils_hmac_sha1(const char *msg, int msg_len, char *digest, const char *key, int key_len);

void cm_utils_md5(const unsigned char *input, unsigned int ilen, unsigned char output[16]);

void cm_utils_sha1(const unsigned char *input, unsigned int ilen, unsigned char output[20]);
void cm_utils_sha256(const unsigned char *input, unsigned int ilen, unsigned char output[32]);

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
									TLS_RSA_WITH_SUIT_ALL:支持所有加密格式
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

int32_t cm_ssl_init(cm_ssl_cfg ssl_cfg);

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
					(注意：HTTP使用时，只适用于异步接口)
* Returns: 0/-1
* History:
*************************************************/

int32_t cm_ssl_start(uint8_t ssl_id,cm_ssl_connect_type connect_type,uint8_t connect_id);

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

int32_t cm_ssl_stop(uint8_t ssl_id);

#endif
