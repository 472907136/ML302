/*********************************************************
*  @file    cm_gpio_demo.c
*  @brief   ML302 OpenCPU gpio example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/15
********************************************************/
#include "main.h"


void cm_test_gpio_read(unsigned char **cmd,int len)
{
	int test_gpio = char_to_int(cmd[2]);
    cm_gpio_cfg_t cfg;
    cfg.id = test_gpio;
    cfg.dir = CM_GPIO_IN;
    cfg.irq_enable = 0;
    cm_gpio_init(&cfg);
    cm_printf("[GPIO_READ]GPIO:%d = %d\n", test_gpio, cm_gpio_read(test_gpio));
	cm_gpio_deinit(test_gpio);
}

void cm_test_gpio_write(unsigned char **cmd,int len)
{
	int test_gpio = char_to_int(cmd[2]);
    cm_gpio_cfg_t cfg;
    cfg.id = test_gpio;
    cfg.dir = CM_GPIO_OUT;
    cfg.irq_enable = 0;
    cm_gpio_init(&cfg);
	cm_gpio_pull_config(test_gpio, CM_GPIO_PULL_UP);
    cm_printf("[GPIO_WRITE]GPIO:%d LOW\n", test_gpio);
    cm_gpio_write(test_gpio,CM_GPIO_LOW);
    osDelay(3000);  
    cm_printf("[GPIO_WRITE]GPIO:%d HIGH\n", test_gpio);
    cm_gpio_write(test_gpio,CM_GPIO_HIGH);
    osDelay(3000);
    cm_printf("[GPIO_WRITE]GPIO:%d LOW\n", test_gpio);
    cm_gpio_write(test_gpio,CM_GPIO_LOW);
    osDelay(3000);  
	cm_gpio_pull_disable(test_gpio);
	cm_gpio_deinit(test_gpio);
}

int g_test_gpio = 0;
#define TEST_IRQ_ENABLE		"INTERRUPT_ON"
#define TEST_IRQ_DISABLE	"INTERRUPT_OFF"

static void gpio_irq_cb(void *param)
{
    cm_printf("[GPIO_IRQ]%s:%d INTERRUPT\n",param, g_test_gpio);
}
void cm_test_gpio_irq(unsigned char **cmd,int len)
{
    cm_gpio_cfg_t cfg ;
	int test_gpio = char_to_int(cmd[2]);
	g_test_gpio = test_gpio;
    cfg.id = test_gpio;
    cfg.dir = CM_GPIO_IN;
	if(strcmp(cmd[3], TEST_IRQ_ENABLE) == 0)
	{
		cfg.irq_enable = 1;
	}
	if(strcmp(cmd[3], TEST_IRQ_DISABLE) == 0)
	{
		cfg.irq_enable = 0;
	}
	cfg.level_or_edge = 0;
	cfg.rising = 1;
	cfg.falling = 1;
    cfg.cb= gpio_irq_cb;
    cfg.param = "GPIO";
    cm_printf("[GPIO_IRQ]GPIO:%d init:%d\n", test_gpio, cm_gpio_init(&cfg));
}
