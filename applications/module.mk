#*********************************************************
#  @file    module.mk
#  @brief   ML302 OPENCPU demo feature config
#  Copyright (c) 2020-2030, ocean Team
#  All rights reserved.
#  created by longmain 2020/04/30
#*********************************************************


###################################################
# 开启demo示例功能
SRC_DIRS += applications/demo
INC      += -Iapplications
INC      += -Iapplications/.
INC      += -Iapplications/demo/.

###################################################
# 开启http支持
SRC_DIRS += applications/demo/http/src
INC      += -Iapplications/demo/http/inc

SRC_DIRS += applications\demo\wolfssl-3.15.3\src
SRC_DIRS += applications\demo\wolfssl-3.15.3\src\wolfcrypt\src
INC      += -Iapplications\demo\wolfssl-3.15.3\include

###################################################
# 开启onenet edp支持
SRC_DIRS += applications/demo/edp/src
INC      += -Iapplications/demo/edp/inc

###################################################
# 开启DM支持
SRC_DIRS += applications/demo/dm/ciscore
SRC_DIRS += applications/demo/dm/ciscore/dm_utils
SRC_DIRS += applications/demo/dm/ciscore/dtls
SRC_DIRS += applications/demo/dm/ciscore/er-coap-13
SRC_DIRS += applications/demo/dm/ciscore/std_object
SRC_DIRS += applications/demo/dm/adapter/win

INC      += -Iapplications/demo/dm/ciscore
INC      += -Iapplications/demo/dm/ciscore/dm_utils
INC      += -Iapplications/demo/dm/adapter/win
INC      += -Iapplications/demo/dm/inc

###################################################
# 开启mqtt支持

SRC_DIRS += applications/demo/libmqttsn/src
SRC_DIRS += applications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/client
SRC_DIRS += applications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/genccont/src
SRC_DIRS += applications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/platform/one-mo
SRC_DIRS += applications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/MQTTPacket/src

INC      += -Iapplications/demo/libmqttsn/include
INC      += -Iapplications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/platform
INC      += -Iapplications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/client
INC      += -Iapplications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/MQTTPacket/src
INC      += -Iapplications/demo/libmqttsn/src/paho.mqtt-sn.embedded-c/genccont/src

###################################################
# 开启阿里云支持

SRC_DIRS += applications/demo/aliyun/src
SRC_DIRS += applications/demo/aliyun/src/certs
SRC_DIRS += applications/demo/aliyun/src/dev_sign
SRC_DIRS += applications/demo/aliyun/src/dynamic_register
SRC_DIRS += applications/demo/aliyun/src/infra
SRC_DIRS += applications/demo/aliyun/src/mqtt
SRC_DIRS += applications/demo/aliyun/src/ota
SRC_DIRS += applications/demo/aliyun/src/wrappers
SRC_DIRS += applications/demo/aliyun/src/wrappers/tls
SRC_DIRS += applications/demo/aliyun/demo

INC      += -Iapplications/demo/aliyun/src
INC      += -Iapplications/demo/aliyun/src/dev_sign
INC      += -Iapplications/demo/aliyun/src/dynamic_register
INC      += -Iapplications/demo/aliyun/src/infra
INC      += -Iapplications/demo/aliyun/src/mqtt
INC      += -Iapplications/demo/aliyun/src/ota
INC      += -Iapplications/demo/aliyun/src/wrappers
