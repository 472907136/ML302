/**
@2018
 */

#ifndef __CMMQTT_CONSTANT_C_
#define __CMMQTT_CONSTANT_C_



/* Used to avoid warnings in case of unused parameters in function pointers */
#define IOT_UNUSED(x) (void)(x)


#define CMMQTT_AT_RESPONSE_DATA_LEN       (128)
#define CMMQTT_AT_CMD_STRING_LEN          (1024)
#define CMMQTT_AT_CMD_PARAM_NUM           (20)
#define CMMQTT_AT_CMD_FILED_NUM           (10)

#define CMMQTT_TASK_MONITOR_NAME               "CMMQTT_TASK_MONITOR"
#define CMMQTT_TASK_PROCESS_NAME               "CMMQTT_TASK_PROCESS"
#define CMMQTT_TASK_MONITOR_STACK_SIZE         (1024 * 2)
#define CMMQTT_TASK_PROCESS_STACK_SIZE         (1024 * 4)

#define CMMQTT_TASK_PROCESS_PRIORITY           24
#define CMMQTT_TASK_MONITOR_PRIORITY           32
#define	CMMQTT_TASK_NULL					   0

#define CMMQTT_MAX_NETERROR			8
#define CMMQTT_MAX_PINGLOST			3
#define CMMQTT_MAX_CONNECTTIMES		3
#define CMMQTT_BASE_RECONN_TIME_S	(4)


#define CMMQTT_DISC_BUFSIZE			8
#define CMMQTT_PING_BUFSIZE			8
#define CMMQTT_ACK_BUFSIZE			8
#define CMMQTT_CON_BUFSIZE			256
#define CMMQTT_SUB_TOPIC_MAXSIZE	512
#define CMMQTT_SUB_BUFSIZE			(CMMQTT_SUB_TOPIC_MAXSIZE+8)
#define CMMQTT_UNSUB_BUFSIZE		256
#define CMMQTT_PUB_BUFSIZE			2048
#define CMMQTT_MINRECV_BUFSIZE		1024

#define CMMQTT_SEND_TIME			512
#define CMMQTT_RECV_TIME			1024


#define CMMQTT_PUBCB_NUM			8

#define CMMQTT_NV_MAX			2048

#define CMMQTT_CLIENTID_SIZE		50
#define CMMQTT_USER_SIZE			50
#define CMMQTT_PWD_SIZE				50
#define CMMQTT_SEREVR_SIZE			50
#define CMMQTT_TOPICNAME_SIZE		50
#define CMMQTT_WILLMSG_SIZE			256
#define CMMQTT_PUBMSG_SIZE			2048

#define CMMQTT_SENDPACK_ERROR	101

#define CMMQTT_CONMQTTSERVER_OK			11
#define CMMQTT_CONMQTTSERVER_FAIL		12
#define CMMQTT_CONMQTTSERVER_TIMEOUT	13

#define CMMQTT_SUB_OK			21
#define CMMQTT_SUBACK_FAIL		22
#define CMMQTT_TOPIC_SUB_DONE	23

#define CMMQTT_PUB_OK			31
#define CMMQTT_PUBACK_FAIL		32
#define CMMQTT_PUBRECACK_FAIL	33
#define CMMQTT_PUBREL_FAIL		34
#define CMMQTT_PUBCOMPACK_FAIL	35

//UNSUB
#define CMMQTT_UNSUB_OK			41
#define CMMQTT_UNSUBACK_FAIL	42

//CON
#define CMMQTT_CONPASSWD_OK		51
#define CMMQTT_CONPASSWD_FAIL	52

//CMD RESPONSE
#define CMMQTT_CMD_RESPONSE_OK		61
#define CMMQTT_CMD_RESPONSE_FAIL	62

typedef enum
{
	UNINITIALED			= 0,
	INITIALED,
	DISCONNECTED,
	CONNECTING,
	RECONNECTING,
	CONNECTED,
	NET_CONNECTING,
	NET_CONNECTED,	
}CMMQTTClientstate;

typedef enum
{

	
	CMMQTT_AT_UNKNOWN_ERROR = 0	,
	CMMQTT_AT_NOTCONN,
	CMMQTT_AT_SYSTEM_ERROR,
	CMMQTT_AT_NETWORK_ERROR,
	CMMQTT_AT_REGISTRATION_FAILURE,
	CMMQTT_AT_OK	=	100,
	CMMQTT_AT_MEMORY_ERROR ,
	CMMQTT_AT_PARAMETER_ERROR,
	CMMQTT_AT_NOT_SUPPORT,
	CMMQTT_AT_SDK_ERROR,
	CMMQTT_AT_NOT_FOUND, 
	CMMQTT_AT_TOPIC_SUB_DONE,
} Cmmqtt_At_Error;

typedef enum 
{
			SUCCESS = 0,
				
			FAILURE = -1,
	/** A required parameter was passed as null */
			NULL_VALUE_ERROR = -2,
	/** The TCP socket could not be established */
			TCP_CONNECTION_ERROR = -3,
	/** The TLS handshake failed */
			SSL_CONNECTION_ERROR = -4,
	/** Error associated with setting up the parameters of a Socket */
			TCP_SETUP_ERROR = -5,
	/** A timeout occurred while waiting for the TLS handshake to complete. */
			NETWORK_SSL_CONNECT_TIMEOUT_ERROR = -6,
	/** A Generic write error based on the platform used */
			NETWORK_SSL_WRITE_ERROR = -7,
	/** SSL initialization error at the TLS layer */
			NETWORK_SSL_INIT_ERROR = -8,
	/** An error occurred when loading the certificates.  The certificates could not be located or are incorrectly formatted. */
			NETWORK_SSL_CERT_ERROR = -9,
	/** SSL Write times out */
			NETWORK_SSL_WRITE_TIMEOUT_ERROR = -10,
	/** SSL Read times out */
			NETWORK_SSL_READ_TIMEOUT_ERROR = -11,
	/** A Generic error based on the platform used */
			NETWORK_SSL_READ_ERROR = -12,
	/** Returned when the Network is disconnected and reconnect is either disabled or physical layer is disconnected */
			NETWORK_DISCONNECTED_ERROR = -13,
	/** Returned when the Network is disconnected and the reconnect attempt has timed out */
			NETWORK_RECONNECT_TIMED_OUT_ERROR = -14,
	/** Returned when the Network is already connected and a connection attempt is made */
			NETWORK_ALREADY_CONNECTED_ERROR = -15,
	/** Network layer Error Codes */
	/** Network layer Random number generator seeding failed */
			NETWORK_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED = -16,
	/** A generic error code for Network layer errors */
			NETWORK_SSL_UNKNOWN_ERROR = -17,
	/** Returned when the physical layer is disconnected */
			NETWORK_PHYSICAL_LAYER_DISCONNECTED = -18,
	/** Returned when the root certificate is invalid */
			NETWORK_X509_ROOT_CRT_PARSE_ERROR = -19,
	/** Returned when the device certificate is invalid */
			NETWORK_X509_DEVICE_CRT_PARSE_ERROR = -20,
	/** Returned when the private key failed to parse */
			NETWORK_PK_PRIVATE_KEY_PARSE_ERROR = -21,
	/** Returned when the network layer failed to open a socket */
			NETWORK_ERR_NET_SOCKET_FAILED = -22,
	/** Returned when the server is unknown */
			NETWORK_ERR_NET_UNKNOWN_HOST = -23,
	/** Returned when connect request failed */
			NETWORK_ERR_NET_CONNECT_FAILED = -24,
	/** Returned when there is nothing to read in the TLS read buffer */
			NETWORK_SSL_NOTHING_TO_READ = -25,
	/** A connection could not be established. */
			MQTT_CONNECTION_ERROR = -26,
	/** A timeout occurred while waiting for the TLS handshake to complete */
			MQTT_CONNECT_TIMEOUT_ERROR = -27,
	/** A timeout occurred while waiting for the TLS request complete */
			MQTT_REQUEST_TIMEOUT_ERROR = -28,
	/** The current client state does not match the expected value */
			MQTT_UNEXPECTED_CLIENT_STATE_ERROR = -29,
	/** The client state is not idle when request is being made */
			MQTT_CLIENT_NOT_IDLE_ERROR = -30,
	/** The MQTT RX buffer received corrupt or unexpected message  */
			MQTT_RX_MESSAGE_PACKET_TYPE_INVALID_ERROR = -31,
	/** The MQTT RX buffer received a bigger message. The message will be dropped  */
			MQTT_RX_BUFFER_TOO_SHORT_ERROR = -32,
	/** The MQTT TX buffer is too short for the outgoing message. Request will fail  */
			MQTT_TX_BUFFER_TOO_SHORT_ERROR = -33,
	/** The client is subscribed to the maximum possible number of subscriptions  */
			MQTT_MAX_SUBSCRIPTIONS_REACHED_ERROR = -34,
	/** Failed to decode the remaining packet length on incoming packet */
			MQTT_DECODE_REMAINING_LENGTH_ERROR = -35,
	/** Connect request failed with the server returning an unknown error */
			MQTT_CONNACK_UNKNOWN_ERROR = -36,
	/** Connect request failed with the server returning an unacceptable protocol version error */
			MQTT_CONNACK_UNACCEPTABLE_PROTOCOL_VERSION_ERROR = -37,
	/** Connect request failed with the server returning an identifier rejected error */
			MQTT_CONNACK_IDENTIFIER_REJECTED_ERROR = -38,
	/** Connect request failed with the server returning an unavailable error */
			MQTT_CONNACK_SERVER_UNAVAILABLE_ERROR = -39,
	/** Connect request failed with the server returning a bad userdata error */
			MQTT_CONNACK_BAD_USERDATA_ERROR = -40,
	/** Connect request failed with the server failing to authenticate the request */
			MQTT_CONNACK_NOT_AUTHORIZED_ERROR = -41,
	/** An error occurred while parsing the JSON string.  Usually malformed JSON. */
			JSON_PARSE_ERROR = -42,
	/** Shadow: The response Ack table is currently full waiting for previously published updates */
			SHADOW_WAIT_FOR_PUBLISH = -43,
	/** Any time an snprintf writes more than size value, this error will be returned */
			SHADOW_JSON_BUFFER_TRUNCATED = -44,
	/** Any time an snprintf encounters an encoding error or not enough space in the given buffer */
			SHADOW_JSON_ERROR = -45,
	/** Mutex initialization failed */
			MUTEX_INIT_ERROR = -46,
	/** Mutex lock request failed */
			MUTEX_TAKE_ERROR = -47,
	/** Mutex unlock request failed */
			MUTEX_GIVE_ERROR = -48,
	/** Mutex destroy failed */
			MUTEX_DESTROY_ERROR = -49,
} Cmmqtt_Error_t;


#endif 
