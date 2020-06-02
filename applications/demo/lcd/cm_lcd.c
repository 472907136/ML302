/*********************************************************
*  @file    cm_lcd_7735s_demo.c
*  @brief   ML302 OpenCPU LCD example 
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2020/03/25
********************************************************/

#include "cm_main.h"
#include "cm_lcd.h"

extern lcdSpec_t cm_lcd_st7735s;
lcdSpec_t *cm_lcd_cfg_tab[] =
{
	(lcdSpec_t *)&cm_lcd_st7735s,
};
uint32_t cm_lcd_get_count(void)
{
    uint32_t count;
	count = sizeof(cm_lcd_cfg_tab) / sizeof(cm_lcd_cfg_tab[0]);
    return count;
}