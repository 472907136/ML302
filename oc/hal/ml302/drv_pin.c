
/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */
#include "drv_pin.h"
#include "oc.h"
#include "oc_device.h"

#ifdef OC_USING_HW_PIN


static void _drv_pin_write(oc_base_t pin, oc_base_t value)
{
    cm_gpio_id_t gpio_pin;

    if ((pin < CM_GPIO_0) && (pin >= CM_GPIO_END))
    {
        return;
    }
    gpio_pin = (cm_gpio_id_t)pin;

    cm_gpio_write(gpio_pin,value);
}

static int _drv_pin_read(oc_base_t pin)
{
    int value = PIN_LOW;
    cm_gpio_id_t gpio_pin;

    if ((pin < CM_GPIO_0) && (pin >= CM_GPIO_END))
    {
        return;
    }
    gpio_pin = (cm_gpio_id_t)pin;
    value = cm_gpio_read(gpio_pin);

    return value;
}

static void _drv_pin_mode(oc_base_t pin, oc_base_t mode)
{
    cm_gpio_id_t gpio_pin;
    cm_gpio_cfg_t cfg;

    if ((pin < CM_GPIO_0) && (pin >= CM_GPIO_END))
    {
        return;
    }

    gpio_pin = (cm_gpio_id_t)pin;
    cfg.id = gpio_pin;
    cfg.irq_enable = 0;

    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        cfg.dir = CM_GPIO_OUT;
        cm_gpio_init(&cfg);
        cm_gpio_pull_config(gpio_pin,CM_GPIO_PULL_UP);
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        cfg.dir = CM_GPIO_IN;
        cm_gpio_init(&cfg);
        cm_gpio_pull_config(gpio_pin,CM_GPIO_FLOAT);
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        cfg.dir = CM_GPIO_IN;
        cm_gpio_init(&cfg);
        cm_gpio_pull_config(gpio_pin,CM_GPIO_PULL_UP);
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        cfg.dir = CM_GPIO_IN;
        cm_gpio_init(&cfg);
        cm_gpio_pull_config(gpio_pin,CM_GPIO_PULL_DOWN);
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        cfg.dir = CM_GPIO_OUT;
        cm_gpio_init(&cfg);
        cm_gpio_pull_config(gpio_pin,CM_GPIO_PULL_DOWN);
    }

    
}

// static oc_err_t _drv_pin_attach_irq(cm_gpio_cfg_t pin,oc_uint32_t mode, void (*hdr)(void *args), void *args)
// {
//     cm_gpio_id_t gpio_pin;
//     oc_uint8_t status;

//    if ((pin < cm_GPIO_0) && (pin >= cm_GPIO_END))
//     {
//         return -OC_ERROR;
//     }

//     gpio_pin = (cm_gpio_id_t)pin;
//     cfg.id = gpio_pin;
//     cfg.dir = cm_GPIO_IN;
// 	cfg.irq_enable = 0;

//     /* 7: eint func */
//     if (hal_pinmux_set_function(gpio_pin, 7) != HAL_PINMUX_STATUS_OK)
//     {
//         return -OC_ERROR;
//     }

//     switch (mode)
//     {
//     case PIN_IRQ_MODE_RISING:
//         cfg.rising = 1;
//         break;
//     case PIN_IRQ_MODE_FALLING:
//         cfg.falling = 1;
//         break;
//     case PIN_IRQ_MODE_RISING_FALLING:
//         cfg.rising = 1;
// 	    cfg.falling = 1;
//         break;
//     case PIN_IRQ_MODE_HIGH_LEVEL:
//         cfg.level_or_edge = 1;
//         break;
//     case PIN_IRQ_MODE_LOW_LEVEL:
//         cfg.level_or_edge = 0;
//         break;
//     default:
//         return -OC_ERROR;
//     }

//     cfg.cb= hdr;
//     cfg.param = "GPIO";
//     status = cm_gpio_init(&cfg);
//     return status;
// }

// static oc_err_t _drv_pin_dettach_irq(oc_int32_t pin)
// {
//     hal_eint_status_t status;
//     hal_eint_number_t eint_number;
//     hal_gpio_pin_t gpio_pin;

//    if ((pin < cm_GPIO_0) && (pin >= cm_GPIO_END))
//     {
//         return -OC_ERROR;
//     }

//     gpio_pin = (hal_gpio_pin_t)pin;
//     eint_number = (hal_eint_number_t)pin;

//     /* disable eint interrupt */
//     status = hal_eint_mask(eint_number);
//     if (status != HAL_EINT_STATUS_OK)
//     {
//         return -OC_ERROR;
//     }

//     /* 0: gpio func */
//     if (hal_pinmux_set_function(gpio_pin, 0) != HAL_PINMUX_STATUS_OK)
//     {
//         return -OC_ERROR;
//     }

//     /* deinit eint */
//     status = hal_eint_deinit(eint_number);
//     if (status != HAL_EINT_STATUS_OK)
//     {
//         return -OC_ERROR;
//     }

//     return OC_EOK;
// }

// static oc_err_t _drv_pin_irq_enable(oc_base_t pin,oc_uint32_t enabled)
// {
//     hal_eint_status_t status;
//     hal_eint_number_t eint_number;

//    if ((pin < cm_GPIO_0) && (pin >= cm_GPIO_END))
//     {
//         return -OC_ERROR;
//     }

//     eint_number = (hal_eint_number_t)pin;

//     if (enabled == PIN_IRQ_ENABLE)
//     {
//         status = hal_eint_unmask(eint_number);
//     }
//     else
//     {
//         status = hal_eint_mask(eint_number);
//     }

//     return (status == HAL_EINT_STATUS_OK) ? OC_EOK : (-OC_ERROR);
// }

const static struct oc_pin_ops _drv_pin_ops =
{
    _drv_pin_mode,
    _drv_pin_write,
    _drv_pin_read,
    // _drv_pin_attach_irq,
    // _drv_pin_dettach_irq,
    // _drv_pin_irq_enable,
};

int oc_hw_pin_init(void)
{
    /* register 'pin' device */
    return oc_device_pin_register("pin", &_drv_pin_ops, OC_NULL);
}

#endif //OC_USING_HW_PIN

