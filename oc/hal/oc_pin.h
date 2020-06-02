/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#ifndef __OC_PIN_H__
#define __OC_PIN_H__

#include <oc.h>
#include <oc_device.h>

#ifdef __cplusplus
extern "C" {
#endif

/* pin device and operations for OC-OS */
struct oc_device_pin
{
    const struct oc_pin_ops *ops;
};

#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PIN_MODE_OUTPUT         0x00
#define PIN_MODE_INPUT          0x01
#define PIN_MODE_INPUT_PULLUP   0x02
#define PIN_MODE_INPUT_PULLDOWN 0x03
#define PIN_MODE_OUTPUT_OD      0x04

#define PIN_IRQ_MODE_RISING             0x00
#define PIN_IRQ_MODE_FALLING            0x01
#define PIN_IRQ_MODE_RISING_FALLING     0x02
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03
#define PIN_IRQ_MODE_LOW_LEVEL          0x04

#define PIN_IRQ_DISABLE                 0x00
#define PIN_IRQ_ENABLE                  0x01

#define PIN_IRQ_PIN_NONE                -1

struct oc_device_pin_mode
{
    oc_uint16_t pin;
    oc_uint16_t mode;
};
struct oc_device_pin_status
{
    oc_uint16_t pin;
    oc_uint16_t status;
};
struct oc_pin_irq_hdr
{
    oc_int16_t        pin;
    oc_uint16_t       mode;
    void (*hdr)(void *args);
    void             *args;
};
struct oc_pin_ops
{
    void (*pin_mode)(oc_base_t pin, oc_base_t mode);
    void (*pin_write)(oc_base_t pin, oc_base_t value);
    int (*pin_read)(oc_base_t pin);

    // /* TODO: add GPIO interrupt */
    // oc_err_t (*pin_attach_irq)(oc_int32_t pin,oc_uint32_t mode, void (*hdr)(void *args), void *args);
    // oc_err_t (*pin_detach_irq)(oc_int32_t pin);
    // oc_err_t (*pin_irq_enable)(oc_base_t pin, oc_uint32_t enabled);
};

int oc_device_pin_register(const char *name, const struct oc_pin_ops *ops, void *user_data);

void oc_pin_mode(oc_base_t pin, oc_base_t mode);
void oc_pin_write(oc_base_t pin, oc_base_t value);
int  oc_pin_read(oc_base_t pin);
oc_err_t oc_pin_attach_irq(oc_int32_t pin, oc_uint32_t mode,
                             void (*hdr)(void *args), void  *args);
oc_err_t oc_pin_detach_irq(oc_int32_t pin);
oc_err_t oc_pin_irq_enable(oc_base_t pin, oc_uint32_t enabled);

#ifdef __cplusplus
}
#endif
#endif //__OC_PIN_H__
