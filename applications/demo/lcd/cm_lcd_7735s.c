/*********************************************************
*  @file    cm_lcd_7735s_demo.c
*  @brief   ML302 OpenCPU LCD example 
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2020/03/25
********************************************************/

#include "main.h"
#include "cm_lcd.h"
#include "cm_lcd_7735s.h"



static void lcd_delay_ms(int ms_delay)
{
    cm_sys_delay_us(ms_delay * 1000);
}
static void _st7735SetDirection(lcdDirect_t direct_type)
{

    switch (direct_type)
    {
    case LCD_DIRECT_NORMAL:
        LCD_CtrlWrite_ST7735(0x36);
        LCD_DataWrite_ST7735(0x00);
        break;
    case LCD_DIRECT_ROT_90:
        LCD_CtrlWrite_ST7735(0x36);
        LCD_DataWrite_ST7735(0x60);
        break;
    }

    LCD_CtrlWrite_ST7735(0x2c);
}
static void _st7735SetDisplayWindow(
    uint16_t left,  // start Horizon address
    uint16_t top,   // start Vertical address
    uint16_t right, // end Horizon address
    uint16_t bottom // end Vertical address
)
{
    uint16_t newleft = left;
    uint16_t newtop = top;
    uint16_t newright = right;
    uint16_t newbottom = bottom;

    cm_lcd_write_cmd(0x2a);                   // set hori start , end (left, right)
    cm_lcd_write_data((newleft >> 8) & 0xff);  // left high 8 b
    cm_lcd_write_data(newleft & 0xff);         // left low 8 b
    cm_lcd_write_data((newright >> 8) & 0xff); // right high 8 b
    cm_lcd_write_data(newright & 0xff);        // right low 8 b

    cm_lcd_write_cmd(0x2b);                    // set vert start , end (top, bot)
    cm_lcd_write_data((newtop >> 8) & 0xff);    // top high 8 b
    cm_lcd_write_data(newtop & 0xff);           // top low 8 b
    cm_lcd_write_data((newbottom >> 8) & 0xff); // bot high 8 b
    cm_lcd_write_data(newbottom & 0xff);        // bot low 8 b
    cm_lcd_write_cmd(0x2c);                    // recover memory write mode
}

static void _st7735Init(void)
{
    cm_lcd_write_cmd(0x11); 
        lcd_delay_ms(120);        
        cm_lcd_write_cmd(0xB1);
        cm_lcd_write_data(0x05);
        cm_lcd_write_data(0x3C);
        cm_lcd_write_data(0x3C);
        cm_lcd_write_cmd(0xB2);
        cm_lcd_write_data(0x05);
        cm_lcd_write_data(0x3C);
        cm_lcd_write_data(0x3C);
        cm_lcd_write_cmd(0xB3);
        cm_lcd_write_data(0x05);
        cm_lcd_write_data(0x3C);
        cm_lcd_write_data(0x3C);
        cm_lcd_write_data(0x05);
        cm_lcd_write_data(0x3C);
        cm_lcd_write_data(0x3C);
        
        cm_lcd_write_cmd(0xB4); //Dot inversion
        cm_lcd_write_data(0x03);
        cm_lcd_write_cmd(0xC0);
        cm_lcd_write_data(0x28);
        cm_lcd_write_data(0x08);
        cm_lcd_write_data(0x04);
        cm_lcd_write_cmd(0xC1);
        cm_lcd_write_data(0XC0);
        cm_lcd_write_cmd(0xC2);
        cm_lcd_write_data(0x0D);
        cm_lcd_write_data(0x00);
        cm_lcd_write_cmd(0xC3);
        cm_lcd_write_data(0x8D);
        cm_lcd_write_data(0x2A);
        cm_lcd_write_cmd(0xC4);
        cm_lcd_write_data(0x8D);
        cm_lcd_write_data(0xEE);
       
        cm_lcd_write_cmd(0xC5); //VCOM
        cm_lcd_write_data(0x1A);
        cm_lcd_write_cmd(0x36); //MX, MY, RGB mode
        cm_lcd_write_data(0xC0);
      
        cm_lcd_write_cmd(0xE0);
        cm_lcd_write_data(0x04);
        cm_lcd_write_data(0x22);
        cm_lcd_write_data(0x07);
        cm_lcd_write_data(0x0A);
        cm_lcd_write_data(0x2E);
        cm_lcd_write_data(0x30);
        cm_lcd_write_data(0x25);
        cm_lcd_write_data(0x2A);
        cm_lcd_write_data(0x28);
        cm_lcd_write_data(0x26);
        cm_lcd_write_data(0x2E);
        cm_lcd_write_data(0x3A);
        cm_lcd_write_data(0x00);
        cm_lcd_write_data(0x01);
        cm_lcd_write_data(0x03);
        cm_lcd_write_data(0x13);
        cm_lcd_write_cmd(0xE1);
        cm_lcd_write_data(0x04);
        cm_lcd_write_data(0x16);
        cm_lcd_write_data(0x06);
        cm_lcd_write_data(0x0D);
        cm_lcd_write_data(0x2D);
        cm_lcd_write_data(0x26);
        cm_lcd_write_data(0x23);
        cm_lcd_write_data(0x27);
        cm_lcd_write_data(0x27);
        cm_lcd_write_data(0x25);
        cm_lcd_write_data(0x2D);
        cm_lcd_write_data(0x3B);
        cm_lcd_write_data(0x00);
        cm_lcd_write_data(0x01);
        cm_lcd_write_data(0x04);
        cm_lcd_write_data(0x13);

        cm_lcd_write_cmd(0x3A); //65k mode
        cm_lcd_write_data(0x05);
        cm_lcd_write_cmd(0x29); //Display on


    lcd_delay_ms(20);
    cm_lcd_write_cmd(0x2c);

}
static void _st7735SleepIn(bool is_sleep)
{

    if (is_sleep)
    {
        cm_lcd_write_cmd(0x28); //display off
        lcd_delay_ms(120);
        cm_lcd_write_cmd(0x10); // enter sleep mode
    }
    else
    {
        _st7735Init();
    }
}
static void _st7735Close(void)
{
    _st7735SleepIn(true);
}
static void _st7735Invalidate(void)
{
    _st7735SetDisplayWindow(0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
void _st7735InvalidateRect(
    uint16_t left,  //the left value of the rectangel
    uint16_t top,   //top of the rectangle
    uint16_t right, //right of the rectangle
    uint16_t bottom //bottom of the rectangle
)
{
    left = (left >= LCD_WIDTH) ? LCD_WIDTH - 1 : left;
    right = (right >= LCD_WIDTH) ? LCD_WIDTH - 1 : right;
    top = (top >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : top;
    bottom = (bottom >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : bottom;

    if ((right < left) || (bottom < top))
    {
		return;
    }

    _st7735SetDisplayWindow(left, top, right, bottom);
}
static void _st7735RotationInvalidateRect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, lcdAngle_t angle)
{
    switch (angle)
    {
    case LCD_ANGLE_0:
        _st7735SetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_90:
        _st7735SetDisplayWindow(left, top, bottom, right);
        break;
    case LCD_ANGLE_180:
        _st7735SetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_270:
        _st7735SetDisplayWindow(left, top, bottom, right);
        break;
    default:
        _st7735SetDisplayWindow(left, top, right, bottom);
        break;
    }
}
static void _st7735EsdCheck(void)
{

    
}
static uint32_t _st7735ReadId(void);

static const lcdOperations_t st7735sOperations =
    {
        _st7735Init,
        _st7735SleepIn,
        _st7735EsdCheck,
        _st7735SetDisplayWindow,
        _st7735InvalidateRect,
        _st7735Invalidate,
        _st7735Close,
        _st7735RotationInvalidateRect,
        NULL,
        _st7735ReadId,
};
const lcdSpec_t cm_lcd_st7735s =
    {
        LCD_ST7735_ID,
        LCD_WIDTH,
        LCD_HEIGHT,
        HAL_GOUDA_SPI_LINE_4,
        LCD_CTRL_SPI,
        (lcdOperations_t *)&st7735sOperations,
        false,
        0x2a000,
       10000000,// 500000, //10000000, 10M
};



static uint32_t _st7735ReadId(void)
{
    return cm_lcd_read_id(&cm_lcd_st7735s);
}

