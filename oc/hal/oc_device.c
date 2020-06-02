/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#include "oc.h"
#include "oc_device.h"

oc_err_t oc_hw_device_init(void)
{

#ifdef OC_USING_HW_PIN
#include "drv_pin.h"
    oc_hw_pin_init();
#endif //OC_USING_HW_PIN

#ifdef OC_USING_USART
#include "drv_usart.h"
    oc_hw_usart_init();
#endif //OC_USING_HW_PIN

#include "drv_vir_at.h"
    oc_hw_vir_at_init();
}











