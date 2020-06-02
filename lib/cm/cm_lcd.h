/*********************************************************
*  @file    lcd.h
*  @brief   ML302 OpenCPU LCD Base header 
*  Copyright (c) 2020 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2020/03/25
********************************************************/
typedef enum
{
    HAL_GOUDA_SPI_LINE_4 = 0,
    HAL_GOUDA_SPI_LINE_3 = 1,
    HAL_GOUDA_SPI_LINE_4_START_BYTE = 2,
    HAL_GOUDA_SPI_LINE_3_2_LANE = 3,
} HAL_GOUDA_SPI_LINE_T;
#define LCD_RESOLUTION_RGB565 2
#define LCD_RESOLUTION_YUV422_UYVY 5
#define LCD_RESOLUTION_YUV420 6
#define LCD_RESOLUTION_YUV422_YUYV 9
typedef enum
{
    LCD_ANGLE_0,
    LCD_ANGLE_90,
    LCD_ANGLE_180,
    LCD_ANGLE_270
} lcdAngle_t;
typedef enum
{
    LCD_DIRECT_NORMAL = 0x00,
    LCD_DIRECT_ROT_90, // Rotation 90
} lcdDirect_t;

// LCD Contrller type
typedef enum
{
    LCD_CTRL_SPI = 0, //
    LCD_CTRL_CSI
} LCD_CTRL_MODULE_E;

typedef struct
{
    void (*init)(void);
    void (*enterSleep)(bool is_sleep);
    void (*checkEsd)(void);
    void (*setDisplayWindow)(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
    void (*invlaidateRect)(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
    void (*invalidate)(void);
    void (*close)(void);
    void (*rotationInvalidateRect)(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, lcdAngle_t angle);
    void (*setBrushDirection)(lcdDirect_t direct_type);
    uint32_t (*readId)(void);
} lcdOperations_t;

typedef struct
{
    uint32_t dev_id;
    uint32_t width;
    uint32_t height;
    HAL_GOUDA_SPI_LINE_T bus_mode;
    LCD_CTRL_MODULE_E controller;
    lcdOperations_t *operation;
    bool is_use_fmark;
    uint32_t fmark_delay;
    uint32_t freq;
} lcdSpec_t;

typedef struct
{
    uint16_t *buffer;
    uint16_t region_x;
    uint16_t region_y;
    ///the width of after convert. if don't need rotation and zoom ,set to  roginal width.
    uint16_t width;
    ///the height of after convert. if don't need rotation and zoom ,set to  roginal height.
    uint16_t height;
    ///image buffer width.
    uint16_t widthOriginal;
    uint16_t colorFormat;
    ///0 no rotation. 1 rotate 90 degree.
    uint16_t rotation;
    /// enable the color mask function.
    bool keyMaskEnable;
    /// mask color value
    uint16_t maskColor;
} lcdFrameBuffer_t;
typedef struct
{
    /// x coordinate of the top left corner of the lcd.
    uint16_t x;
    /// y coordinate of the top left corner of the lcd.
    uint16_t y;
    uint16_t width;
    uint16_t height;

} lcdDisplay_t;
/**
 *  \brief lcd初始化
 *  
 *  \return 空
 *  
 *  \details More details
 */
void cm_lcd_init(void);
/**
 *  \brief LCD去初始化
 *  
 *  \return 空
 *  
 *  \details More details
 */
void cm_lcd_deinit(void);
/**
 *  \brief LCD写命令
 *  
 *  \param [in] cmd 写入到LCD IC的命令
 *  \return 空
 *  
 *  \details More details
 */
void cm_lcd_write_cmd(unsigned char cmd);
/**
 *  \brief LCD写数据
 *  
 *  \param [in] data 写入到LCD的数据
 *  \return 空
 *  
 *  \details More details
 */
void cm_lcd_write_data(unsigned char data);
/**
 *  \brief LCD传送显示数据
 *  
 *  \param [in] dataBufferWin 显示数据的结构体
 *  \param [in] lcdRec 指定现实数据位置的结构体
 *  \return 空
 *  
 *  \details More details
 */
void cm_lcd_block_transfer(const lcdFrameBuffer_t *dataBufferWin,const lcdDisplay_t *lcdRec);
/**
 *  \brief 读取LCD的ID，此函数不在应用程序中使用，仅用在移植LCD驱动时
 *  
 *  \param [in] lcdspec 传入参数，lcd信息结构体的指针
 *  \return LCD ID
 *  
 *  \details More details
 */
uint32_t cm_lcd_read_id(lcdSpec_t * lcdspec);
/**
 *  \brief 获取LCD的信息
 *  
 *  \param [out] lcdspec 传出参数，LCD信息结构体的指针
 *  \return 空
 *  
 *  \details More details
 */
void cm_lcd_get_info(lcdSpec_t * lcdspec);