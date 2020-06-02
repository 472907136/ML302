/*********************************************************
*  @file    cm_dm.h
*  @brief   ML302 DM  header File
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangliang 2019/11/28
********************************************************/
#ifndef __CM_DM_H__
#define __CM_DM_H__



#define DM_CIS_URI_EQUAL(oriUri, desUri) (((oriUri)->flag == (desUri)->flag) && ((oriUri)->objectId == (desUri)->objectId) && ((oriUri)->instanceId == (desUri)->instanceId) && ((oriUri)->resourceId == (desUri)->resourceId))

typedef struct cm_dm_event
{
    uint32_t id;     ///< event identifier
    uint32_t param1; ///< 1st parameter
    uint32_t param2; ///< 2nd parameter
    uint32_t param3; ///< 3rd parameter
} cm_dm_event_t;

typedef void (*cm_dm_callback_t)(void *ctx);

typedef enum
{
    DM_EVETN_IND = 0,
    DM_SAMPLE_CALLBACK_READ,
    DM_SAMPLE_CALLBACK_WRITE,
    DM_SAMPLE_CALLBACK_EXECUTE,
    DM_SAMPLE_CALLBACK_OBSERVE,
    DM_SAMPLE_CALLBACK_SETPARAMS,
    DM_SAMPLE_CALLBACK_DISCOVER,
} cm_dm_callback_type_t;

struct cm_dm_st_callback_info
{
    struct cm_dm_st_callback_info *next;
    cis_listid_t mid;
    cm_dm_callback_type_t flag;
    cis_uri_t uri;

    union {
        struct
        {
            cis_data_t *value;
            cis_attrcount_t count;
        } asWrite;

        struct
        {
            uint8_t *buffer;
            uint32_t length;
        } asExec;

        struct
        {
            bool flag;
        } asObserve;

        struct
        {
            cis_observe_attr_t params;
        } asObserveParam;
    } param;
};

struct dm_st_observe_info
{
    struct cm_dm_callback_type_t *next;
    cis_listid_t mid;
    cis_uri_t uri;
    cis_observe_attr_t params;
};
#endif