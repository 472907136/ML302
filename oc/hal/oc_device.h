/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#ifndef __OC_DEVICE_H__
#define __OC_DEVICE_H__
#include "oc.h"
#include "oc_vir_at.h"

#ifdef OC_USING_HW_PIN
#include "oc_pin.h"
#endif //OC_USING_HW_PIN




#ifdef OC_USING_ADC
#include "oc_adc.h"
#endif //OC_USING_ADC

#ifdef OC_USING_I2C
#include "oc_i2c.h"
#endif //OC_USING_I2C

#ifdef OC_USING_PWM
#include "oc_pwm.h"
#endif //OC_USING_PWM

#ifdef OC_USING_SPI
#include "oc_spi.h"
#endif //OC_USING_SPI

#ifdef OC_USING_RTC
#include "oc_rtc.h"
#endif //OC_USING_RTC

#ifdef OC_USING_HWTIMER
#include "oc_hwtimer.h"
#endif //OC_USING_HWTIMER

#ifdef OC_USING_CAN
#include "oc_can.h"
#endif //OC_USING_CAN


oc_err_t oc_hw_device_init(void);


#endif //__OC_DEVICE_H__



