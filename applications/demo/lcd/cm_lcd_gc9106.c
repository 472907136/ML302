/*********************************************************
*  @file    cm_lcd_9106_demo.c
*  @brief   ML302 OpenCPU LCD example 
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2020/03/25
********************************************************/

#include "main.h"
#include "cm_lcd.h"
#include "cm_lcd_gc9106.h"


static void lcd_delay_ms(int ms_delay)
{
    cm_sys_delay_us(ms_delay * 1000);
}

static void _GC9106SetDisplayWindow(
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

void _GC9106Init(void)
{
	atCmdRespDefUrcText("GC91036 Init In");
	lcd_delay_ms(120);
    cm_lcd_write_cmd(0xfe);
	cm_lcd_write_cmd(0xfe);
	cm_lcd_write_cmd(0xef);
	
	cm_lcd_write_cmd(0xb3);
	cm_lcd_write_data(0x03);
	
	cm_lcd_write_cmd(0xb6);
	cm_lcd_write_data(0x01);
	
	cm_lcd_write_cmd(0xA3);
	cm_lcd_write_data(0x11);
	
	
	cm_lcd_write_cmd(0x21);
	cm_lcd_write_cmd(0x36);
	cm_lcd_write_data(0x00);//68
	
	cm_lcd_write_cmd(0x3a);
	cm_lcd_write_data(0x05);
	
	cm_lcd_write_cmd(0x35);
	cm_lcd_write_data(0x0);
	
	cm_lcd_write_cmd(0xb4);
	cm_lcd_write_data(0x21);
	
	cm_lcd_write_cmd(0xF0);
	cm_lcd_write_data(0x31);
	cm_lcd_write_data(0x4C);
	cm_lcd_write_data(0x24);
	cm_lcd_write_data(0x58);
	cm_lcd_write_data(0xa8);
	
	cm_lcd_write_data(0x26);
	cm_lcd_write_data(0x28); 
	cm_lcd_write_data(0x00);
	cm_lcd_write_data(0x2C);
	cm_lcd_write_data(0x0C);
	
	cm_lcd_write_data(0x0C);
	cm_lcd_write_data(0x15);
	cm_lcd_write_data(0x15);
	cm_lcd_write_data(0x0f);
	 
	cm_lcd_write_cmd(0xF1);
	cm_lcd_write_data(0x0E);
	cm_lcd_write_data(0x2D);
	cm_lcd_write_data(0x24);
	cm_lcd_write_data(0x3E);
	cm_lcd_write_data(0x99);
	
	cm_lcd_write_data(0x12);
	cm_lcd_write_data(0x13);
	cm_lcd_write_data(0x00);
	cm_lcd_write_data(0x0A);
	cm_lcd_write_data(0x0D);
	
	cm_lcd_write_data(0x0D);
	cm_lcd_write_data(0x14);
	cm_lcd_write_data(0x13);
	cm_lcd_write_data(0x0f);
	
	cm_lcd_write_cmd(0xfe);
	cm_lcd_write_cmd(0xff);
	cm_lcd_write_cmd(0x11);
	lcd_delay_ms(120);
	cm_lcd_write_cmd(0x29);

}
static void _GC9106SleepIn(bool is_sleep)
{

    if (is_sleep)
    {
		cm_lcd_write_cmd(0xfe);
		cm_lcd_write_cmd(0xef);
		cm_lcd_write_cmd(0x28);
		lcd_delay_ms(120);
		cm_lcd_write_cmd(0x10);
		lcd_delay_ms(10);
       
    }
    else
    {
        _GC9106Init();
		
    }
}
static void _GC9106Close(void)
{
    _GC9106SleepIn(true);
}
void _GC9106Invalidate(void)
{
    _GC9106SetDisplayWindow(0x0, 0x0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
}
void _GC9106InvalidateRect(
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

    _GC9106SetDisplayWindow(left, top, right, bottom);
}
static void _GC9106RotationInvalidateRect(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, lcdAngle_t angle)
{
    switch (angle)
    {
    case LCD_ANGLE_0:
        _GC9106SetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_90:
        _GC9106SetDisplayWindow(left, top, bottom, right);
        break;
    case LCD_ANGLE_180:
        _GC9106SetDisplayWindow(left, top, right, bottom);
        break;
    case LCD_ANGLE_270:
        _GC9106SetDisplayWindow(left, top, bottom, right);
        break;
    default:
        _GC9106SetDisplayWindow(left, top, right, bottom);
        break;
    }
}
static void _GC9106EsdCheck(void)
{

    
}
static uint32_t _GC9106ReadId(void);

static const lcdOperations_t GC9106sOperations =
    {
        _GC9106Init,
        _GC9106SleepIn,
        _GC9106EsdCheck,
        _GC9106SetDisplayWindow,
        _GC9106InvalidateRect,
        _GC9106Invalidate,
        _GC9106Close,
        _GC9106RotationInvalidateRect,
        NULL,
        _GC9106ReadId,
};
const lcdSpec_t cm_lcd_GC9106s =
    {
        LCD_GC9106_ID,
        LCD_WIDTH,
        LCD_HEIGHT,
        HAL_GOUDA_SPI_LINE_4,
        LCD_CTRL_SPI,
        (lcdOperations_t *)&GC9106sOperations,
        false,
        0x2a000,
       500000,// 500000, //10000000, 10M
};



static uint32_t _GC9106ReadId(void)
{
    return cm_lcd_read_id(&cm_lcd_GC9106s);;
}

