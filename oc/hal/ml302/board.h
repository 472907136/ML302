/*
 * Copyright (c) 2019-2029, ocean Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-03-23     longmain       first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__
#include "cm_gpio.h"
typedef enum{
    HAL_UART_0,//引脚29:TXD 引脚30:RXD
    HAL_UART_1,//引脚97:TXD 引脚98:RXD
    HAL_UART_2,//引脚15:TXD 引脚14:RXD
}hal_uart_id_t;



/**
 * pin mapping for ML302
 * ML302 pin index   | 8910DM pin number    | ML302 pin func| 8910DM pin func   | Usage
*/
#define OC_PIN_3      ONEMO_GPIO_9          /* GPIO1        | GPIO9             | Normal */
#define OC_PIN_7      ONEMO_GPIO_15         /* I2C1_SDA     | I2C1_SDA/GPIO_15  | Normal */
#define OC_PIN_8      ONEMO_GPIO_14         /* I2C1_SCL     | I2C1_SCL/GPIO_14  | Normal */
#define OC_PIN_11     ONEMO_GPIO_19         /* GPIO_19      | GPIO_19           | Normal */
#define OC_PIN_12     ONEMO_GPIO_11         /* GPIO_2       | GPIO_11           | Normal */
//#define OC_PIN_14     UART2_RXD           /* UART2_RXD    | UART2_RXD         | Normal */
#define OC_PIN_15     ONEMO_GPIO_23         /* UART2_TXD    | UART2_TXD/GPIO_23 | Normal */
#define OC_PIN_21     ONEMO_GPIO_2          /* PCM_DIN      | GPIO_2            | Normal */
#define OC_PIN_22     ONEMO_GPIO_3          /* PCM_DOUT     | GPIO_3            | Normal */
#define OC_PIN_23     ONEMO_GPIO_1          /* PCM_SYNC     | GPIO_1            | Normal */
#define OC_PIN_24     ONEMO_GPIO_0          /* PCM_CLK      | GPIO_0            | Normal */
//#define OC_PIN_26     ADC2                /* ADC2         | ADC2              | Normal */
//#define OC_PIN_27     ADC1                /* ADC1         | ADC1              | Normal */
//#define OC_PIN_29     UART0_TXD           /* UART0_TXD    | UART0_TXD         | AT     */
//#define OC_PIN_30     UART0_RXD           /* UART0_RXD    | UART0_RXD         | AT     */
//#define OC_PIN_31     UART0_RTS           /* UART0_RTS    | UART0_RTS         | Normal */
//#define OC_PIN_32     UART0_CTS           /* UART0_CTS    | UART0_CTS         | Normal */
//#define OC_PIN_52     SPI_CS              /* SPI_CS       | SPI_CS            | SPI_CS */
//#define OC_PIN_53     SPI_CLK             /* SPI_CLK      | SPI_CLK           | SPI_CLK */
//#define OC_PIN_54     SPI_RX              /* SPI_RX       | SPI_RX            | SPI_RX */
//#define OC_PIN_55     SPI_TX              /* SPI_TX       | SPI_TX            | SPI_TX */
#define OC_PIN_69     ONEMO_GPIO_22         /* GPIO_3       | GPIO_22           | Normal */
#define OC_PIN_78     ONEMO_GPIO_17         /* GPIO_4       | GPIO_17           | Normal */
#define OC_PIN_82     ONEMO_GPIO_27         /* SD_DATA2     | GPIO_27           | Normal */
#define OC_PIN_83     ONEMO_GPIO_26         /* SD_DATA1     | GPIO_26           | Normal */
#define OC_PIN_84     ONEMO_GPIO_25         /* SD_DATA0     | GPIO_25           | Normal */
#define OC_PIN_85     ONEMO_GPIO_24         /* SD_CMD       | GPIO_24           | Normal */
#define OC_PIN_97     ONEMO_GPIO_21         /* UART1_TXD    | GPIO_21           | Normal */
#define OC_PIN_98     ONEMO_GPIO_20         /* UART1_RXD    | GPIO_20           | Normal */
#define OC_PIN_100    ONEMO_GPIO_28         /* SD_DATA3     | GPIO_28           | Normal */
#define OC_PIN_136    ONEMO_GPIO_12         /* WLAN_EN      | GPIO_12           | Normal */

#endif

