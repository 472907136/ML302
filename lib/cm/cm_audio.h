/*********************************************************
*  @file    cm_audio.h
*  @brief   ML302 OpenCPU audio APIs header
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by WangPeng 2020/05/06
********************************************************/
#ifndef __CM_AUDIO_H__
#define __CM_AUDIO_H__
  /**

 * @defgroup AUDIO_FUNCTIONS
 * 
   AUDIO API，请参照SDK中示例文件使用
 *@{
 */
/**
 *  @brief ADC 通道，一共两个通道
 */
typedef enum
{
    CM_PLAYER_EVENT_FINISHED = 1,     //播放结束
} cm_player_event_t;

typedef void (*cm_play_callback)( cm_player_event_t event); 
typedef void (*cm_record_callback)(uint8_t *buf, uint32_t len);

typedef enum{
    CM_AUDIO_OUTPUT_HEADSET = 1,    //耳机通道
    CM_AUDIO_OUTPUT_SPEAKER,         //扬声器通道
}cm_audio_output_t;

typedef enum{
    CM_AUDIO_FORMAT_PCM = 1,
    CM_AUDIO_FORMAT_WAVPCM,
    CM_AUDIO_FORMAT_MP3,
}cm_audio_stream_format_t;

typedef enum
{
    CM_AUSAMPLE_FORMAT_S16,              // int16_t, 小开端
} cm_audio_sample_format_t;

typedef enum
{
    CM_AIF_FREQ_8000HZ  =  8000,        //（默认）录音时默认且只能使用8000Hz
    CM_AIF_FREQ_11025HZ = 11025,
    CM_AIF_FREQ_22050HZ = 22050,
    CM_AIF_FREQ_24000HZ = 24000,
    CM_AIF_FREQ_32000HZ = 32000,
    CM_AIF_FREQ_44100HZ = 44100,
} cm_audio_freq_t;

typedef enum
{
    CM_AUDIO_SINGLE_CHANNEL  = 1,        //（默认）
    CM_AUDIO_DUAL_CHANNEL    = 2,
} cm_audio_channel_t;

typedef struct
{
    cm_audio_sample_format_t sample_format;
    cm_audio_freq_t sample_rate;
    cm_audio_channel_t channel_count;
} cm_audio_frame_t;

/**
 *  \brief 设置播放通道
 *  
 *  \param [in] audch 播放通道
 *  \return 0：成功 -1：失败
 *  
 *  \details More details
 */
int32_t cm_audio_set_output(cm_audio_output_t audch);

/**
 *  \brief 获取播放通道
 *  
 *  \return ONEMO_AUDIO_OUTPUT_HEADSET：耳机通道 ONEMO_AUDIO_OUTPUT_SPEAKER：扬声器通道
 *  
 *  \details More details
 */
cm_audio_output_t cm_audio_get_output();

/**
 *  \brief 设置音量（音频范围0~9
 *  
 *  \param [in] level 音量
 *  \return 0：成功 -1：失败
 *  
 *  \details More details
 */
int32_t cm_audio_set_volume(uint32_t level);

/**
 *  \brief 获取音量
 *  
 *  \return 当前音量值
 *  
 *  \details More details
 */
int32_t cm_audio_get_volume();

/**
 *  \brief 从内存播放音频
 *  
 *  \param [in] buf 播放音频数据
 *  \param [in] size PCM数据长度
 *  \param [in] format 音频格式
 *  \param [in] frame PCM数据参数（非PCM时设置为NULL）
 *  \param [in] cb 回调函数
 *  \return 0：成功 -1：失败
 *  
 *  \details More details
 */
int32_t cm_audio_player_startmem(const void *buf, unsigned size, cm_audio_stream_format_t format, cm_audio_frame_t *frame, cm_play_callback cb);

/**
 *  \brief 播放结束（每次播放结束或用户主动结束均需在调用该函数后，再进行其他的音频播放操作）
 *  
 *  \return 0：成功 -1：失败
 *  
 *  \details More details
 */
int32_t cm_audio_player_stop();

/**
 *  \brief 开始录音
 *  
 *  \param [in] cb 回调函数
 *  \return 0：成功 -1：失败
 *  
 *  \details More details
 */
int32_t cm_audio_record_start(cm_record_callback cb);

/**
 *  \brief 结束录音
 *  
 *  \return 无
 *  
 *  \details More details
 */
void cm_audio_record_stop();
/** @} */ 
#endif
