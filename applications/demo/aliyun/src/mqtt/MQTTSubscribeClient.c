/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include "MQTTPacket.h"

#include <string.h>

/**
  * Determines the length of the MQTT subscribe packet that would be produced using the supplied parameters
  * @param count the number of topic filter strings in topicFilters
  * @param topicFilters the array of topic filter strings to be used in the publish
  * @return the length of buffer needed to contain the serialized version of the packet
  */
static int MQTTSerialize_subscribeLength(int count, MQTTString topicFilters[])
{
    int i;
    int len = 2; /* packetid */

    for (i = 0; i < count; ++i) {
        len += 2 + MQTTstrlen(topicFilters[i]) + 1;    /* length + topic + req_qos */
    }
    return len;
}


/**
  * Serializes the supplied subscribe data into the supplied buffer, ready for sending
  * @param buf the buffer into which the packet will be serialized
  * @param buflen the length in bytes of the supplied bufferr
  * @param dup integer - the MQTT dup flag
  * @param packetid integer - the MQTT packet identifier
  * @param count - number of members in the topicFilters and reqQos arrays
  * @param topicFilters - array of topic filter names
  * @param requestedQoSs - array of requested QoS
  * @return the length of the serialized data.  <= 0 indicates error
  */
int MQTTSerialize_subscribe(unsigned char *buf, int buflen, unsigned char dup, unsigned short packetid, int count,
                            MQTTString topicFilters[], int requestedQoSs[])
{
    unsigned char *ptr = buf;
    MQTTHeader header = {0};
    int rem_len = 0;
    int rc = 0;
    int i = 0;

    if (MQTTPacket_len(rem_len = MQTTSerialize_subscribeLength(count, topicFilters)) > buflen) {
        rc = MQTTPACKET_BUFFER_TOO_SHORT;
        goto exit;
    }

    header.byte = 0;
    MQTT_HEADER_SET_TYPE(header.byte, SUBSCRIBE);
    MQTT_HEADER_SET_DUP(header.byte, dup);
    MQTT_HEADER_SET_QOS(header.byte, 1);
    writeChar(&ptr, header.byte); /* write header */

    ptr += MQTTPacket_encode(ptr, rem_len); /* write remaining length */;

    writeInt(&ptr, packetid);

    for (i = 0; i < count; ++i) {
        writeMQTTString(&ptr, topicFilters[i]);
        writeChar(&ptr, requestedQoSs[i]);
    }

    rc = ptr - buf;
exit:
    return rc;
}



/**
  * Deserializes the supplied (wire) buffer into suback data
  * @param packetid returned integer - the MQTT packet identifier
  * @param maxcount - the maximum number of members allowed in the grantedQoSs array
  * @param count returned integer - number of members in the grantedQoSs array
  * @param grantedQoSs returned array of integers - the granted qualities of service
  * @param buf the raw buffer data, of the correct length determined by the remaining length field
  * @param buflen the length in bytes of the data in the supplied buffer
  * @return error code.  1 is success, 0 is failure
  */
int MQTTDeserialize_suback(unsigned short *packetid, int maxcount, int *count, int grantedQoSs[], unsigned char *buf,
                           int buflen)
{
    MQTTHeader header = {0};
    unsigned char *curdata = buf;
    unsigned char *enddata = NULL;
    int rc = 0;
    int mylen;

    header.byte = readChar(&curdata);
    if (MQTT_HEADER_GET_TYPE(header.byte) != SUBACK) {
        goto exit;
    }

    curdata += (rc = MQTTPacket_decodeBuf(curdata, &mylen)); /* read remaining length */
    enddata = curdata + mylen;
    if (enddata - curdata < 2) {
        goto exit;
    }

    *packetid = readInt(&curdata);

    *count = 0;
    while (curdata < enddata) {
        if (*count >= maxcount) {
            rc = -1;
            goto exit;
        }
        grantedQoSs[(*count)++] = readChar(&curdata);
    }

    rc = 1;
exit:
    return rc;
}



