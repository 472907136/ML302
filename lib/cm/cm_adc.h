/*********************************************************
*  @file    cm_uart.h
*  @brief   ML302 OpenCPU ADC APIs header
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/21
********************************************************/
#ifndef __CM_ADC_H__
#define __CM_ADC_H__
  /**

 * @defgroup ADC_FUNCTIONS
 * 
   ADC API，请参照SDK中示例文件使用
 *@{
 */
/**
 *  @brief ADC 通道，一共两个通道
 */
typedef enum{
    CM_ADC_CHANNEL_1,
    CM_ADC_CHANNEL_2,
}cm_adc_channel_t;
/**
 *  \brief 获取ADC测得的电压值
 *  
 *  \param [in] chan 通道号
 *  \return 电压值，单位mv
 *  
 *  \details More details
 */
int cm_adc_get_vol(cm_adc_channel_t chan);

/**
 *  \brief 获取vbat测得的电压值
 *  
 *  \return 电压值，单位mv
 *  
 *  \details More details
 */
int cm_get_vbat();
/** @} */ 
#endif