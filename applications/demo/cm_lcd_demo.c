/*********************************************************
*  @file    cm_lcd_demo.c
*  @brief   ML302 OpenCPU ADC APIs header
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/10/21
********************************************************/
#include "main.h"
#include "cm_lcd.h"
static void fill_vertical_colorbar_Rgb565(uint16_t *buffer, unsigned width, unsigned height)
{
    for (unsigned h = 0; h < height; ++h)
    {
        for (unsigned w = 0; w < width; ++w)
        {
            if (w < width / 3)
                buffer[h * width + w] = 0xf800;
            else if (w < width / 3 * 2)
                buffer[h * width + w] = 0x07e0;
            else
                buffer[h * width + w] = 0x001f;
        }
    }
}
static void fill_horizontal_color_bar_Rgb565(uint16_t *buffer, unsigned width, unsigned height)
{
    for (unsigned h = 0; h < height; ++h)
    {
        for (unsigned w = 0; w < width; ++w)
        {
            if (h < height / 3)
                buffer[h * width + w] = 0xf800;
            else if (h < height / 3 * 2)
                buffer[h * width + w] = 0x07e0;
            else
                buffer[h * width + w] = 0x001f;
        }
    }
}
static inline void fill_Buffer_white_screen(void *buffer, unsigned width, unsigned height)
{
    memset(buffer, 0xff, width * height * sizeof(uint16_t));
}
lcdSpec_t lcd;
int lcd_init = 0;
void cm_test_lcd(unsigned char **cmd,int len)
{
	int func = char_to_int(cmd[2]);
	if(lcd_init == 0)
	{
		cm_lcd_init();
		cm_lcd_get_info(&lcd);
		lcd_init = 1;
	}
	cm_printf("[LCD]ID:%02x%02x%02x%02x\n",lcd.dev_id,lcd.dev_id>>8,lcd.dev_id>>16,lcd.dev_id>>24);
	unsigned short *buffer = malloc(lcd.width*lcd.height*sizeof(unsigned short));
	if(func == 0)
	{
		cm_printf("[LCD]:VERTICAL\n");
		fill_vertical_colorbar_Rgb565(buffer, lcd.width, lcd.height);
	}else if(func == 1)
	{
		cm_printf("[LCD]:HORIZONTAL\n");
		fill_horizontal_color_bar_Rgb565(buffer, lcd.width, lcd.height);
	}else
	{
		cm_printf("[LCD]BLANK\n");
		fill_Buffer_white_screen(buffer, lcd.width, lcd.height);
	}

    lcdDisplay_t rec = {};
    rec.x = 0;
    rec.y = 0;
    rec.width = lcd.width;
    rec.height = lcd.height;

    lcdFrameBuffer_t fb = {};
    fb.buffer = buffer;
    fb.colorFormat = LCD_RESOLUTION_RGB565;
    fb.height = lcd.height;
    fb.width = lcd.width;
    fb.widthOriginal = lcd.width;
    cm_lcd_block_transfer(&fb, &rec);
	cm_printf("[LCD]:func%d\n",func);

}
