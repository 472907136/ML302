/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#include <oc_pin.h>
#include "oc_config.h"

#ifdef OC_USING_HW_PIN

#include "oc_dbg.h"

static struct oc_device_pin _hw_pin;

int oc_device_pin_register(const char *name, const struct oc_pin_ops *ops, void *user_data)
{
    _hw_pin.ops                 = ops;
    return 0;
}

// oc_err_t oc_pin_attach_irq(oc_int32_t pin, oc_uint32_t mode,void (*irq_fun)(void *args), void  *args)
// {
//     OC_ASSERT(_hw_pin.ops != OC_NULL);
//     if(_hw_pin.ops->pin_attach_irq)
//     {
//         return _hw_pin.ops->pin_attach_irq(pin, mode, irq_fun, args);
//     }
//     return OC_ENOSYS;
// }
// oc_err_t oc_pin_detach_irq(oc_int32_t pin)
// {
//     OC_ASSERT(_hw_pin.ops != OC_NULL);
//     if(_hw_pin.ops->pin_detach_irq)
//     {
//         return _hw_pin.ops->pin_detach_irq(pin);
//     }
//     return OC_ENOSYS;
// }

// oc_err_t oc_pin_irq_enable(oc_base_t pin, oc_uint32_t enabled)
// {
//     OC_ASSERT(_hw_pin.ops != OC_NULL);
//     if(_hw_pin.ops->pin_irq_enable)
//     {
//         return _hw_pin.ops->pin_irq_enable(pin, enabled);
//     }
//     return OC_ENOSYS;
// }

/* oc hw pin apis */
void oc_pin_mode(oc_base_t pin, oc_base_t mode)
{
    OC_ASSERT(_hw_pin.ops != OC_NULL);
    _hw_pin.ops->pin_mode(pin, mode);
}

void oc_pin_write(oc_base_t pin, oc_base_t value)
{
    OC_ASSERT(_hw_pin.ops != OC_NULL);
    _hw_pin.ops->pin_write(pin, value);
}

int  oc_pin_read(oc_base_t pin)
{
    OC_ASSERT(_hw_pin.ops != OC_NULL);
    return _hw_pin.ops->pin_read(pin);
}

#endif //OC_USING_HW_PIN
