/**
  ******************************************************************************
  * @file    gui.h
  * @author  yxnan@pm.me
  * @date    2020-08-02
  * @brief   stm32f103 simple GUI interface header
  ******************************************************************************
  */

#ifndef __GUI_H
#define __GUI_H

#include "stm32f10x.h"
#include "lcd_ili9341.h"

#define LABEL_BUFFER_SIZE      25

#define BUTTON_WIDTH           40
#define BUTTON_HEIGHT          28
#define BUTTON_PER_COLUMN      5
#define BUTTON_NUM             (BUTTON_PER_COLUMN+2)

#define BUTTON_GAP          ((LCD_Y_LENGTH - BUTTON_PER_COLUMN*BUTTON_HEIGHT) / (BUTTON_PER_COLUMN+1))
#define BUTTON_START_X      ( BUTTON_WIDTH / 2)
#define BUTTON_START_Y        BUTTON_GAP

#define LABEL1_START_X      ( BUTTON_WIDTH * 2 )
#define LABEL1_START_Y        HEIGHT_CH_CHAR
#define LABEL2_START_X      ( BUTTON_WIDTH * 2 )
#define LABEL2_START_Y      ( HEIGHT_CH_CHAR * 2 )
#define LABEL3_START_X      ( BUTTON_WIDTH * 2 )
#define LABEL3_START_Y      ( HEIGHT_CH_CHAR * 3 )
#define LABEL4_START_X      ( BUTTON_WIDTH * 2 )
#define LABEL4_START_Y      ( HEIGHT_CH_CHAR * 4 )

#define GRAPH_BOTTOM        (HEIGHT_CH_CHAR + 8)
#define GRAPH_START_X     ( LABEL1_START_X + 5*WIDTH_CH_CHAR )
#define GRAPH_START_Y     ( LABEL4_START_Y + HEIGHT_CH_CHAR + HEIGHT_CH_CHAR/2)
#define GRAPH_END_X       ( LCD_X_LENGTH - GRAPH_BOTTOM )
#define GRAPH_END_Y       ( LCD_Y_LENGTH - GRAPH_BOTTOM )
#define GRAPH_WIDTH       ( GRAPH_END_X - GRAPH_START_X )
#define GRAPH_HEIGHT      ( GRAPH_END_Y - GRAPH_START_Y )

#define CH1_COLOR       CL_BLUE4
#define CH2_COLOR       CL_MAGENTA

/*
    LCD 颜色代码
    16Bit由高位至低位， RRRR RGGG GGGB BBBB

    下面的RGB 宏将24位的RGB值转换为16位格式。
*/
#if LCD_RGB_888
/*RGB888颜色转换*/
#define RGB(R,G,B)  ( (R<< 16) | (G << 8) | (B))    /* 将8位R,G,B转化为 24位RGB888格式 */

#else
/*RGB565 颜色转换*/
#define RGB(R,G,B)  (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3)) /* 将8位R,G,B转化为 16位RGB565格式 */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)

#endif

enum
{
    CL_WHITE    = RGB(255,255,255), /* 白色 */
    CL_BLACK    = RGB(  0,  0,  0), /* 黑色 */
    CL_RED      = RGB(255,  0,  0), /* 红色 */
    CL_GREEN    = RGB(  0,255,  0), /* 绿色 */
    CL_BLUE     = RGB(  0,  0,255), /* 蓝色 */
    CL_YELLOW   = RGB(255,255,  0), /* 黄色 */

    CL_GREY         = RGB( 98, 98, 98),  /* 深灰色 */
    CL_GREY1        = RGB( 150, 150, 150),  /* 浅灰色 */
    CL_GREY2        = RGB( 180, 180, 180),  /* 浅灰色 */
    CL_GREY3        = RGB( 200, 200, 200),  /* 最浅灰色 */
    CL_GREY4        = RGB( 230, 230, 230),  /* 最浅灰色 */

    CL_BUTTON_GREY  = RGB( 195, 195, 195), /* 按钮表面灰色 */

    CL_MAGENTA      = RGB(255, 0, 255), /* 红紫色，洋红色 */
    CL_CYAN         = RGB( 0, 255, 255),    /* 蓝绿色，青色 */

    CL_BLUE1        = RGB(  0,  0, 240),        /* 深蓝色 */
    CL_BLUE2        = RGB(  0,  0, 128),        /* 深蓝色 */
    CL_BLUE3        = RGB(  68, 68, 255),       /* 浅蓝色1 */
    CL_BLUE4        = RGB(  0, 64, 128),        /* 浅蓝色1 */

    /* UI 界面控件色 */
    CL_BTN_FACE     = RGB(236, 233, 216),   /* 按钮表面颜色(灰) */
    CL_BOX_BORDER1  = RGB(172, 168,153),    /* 分组框主线颜色 */
    CL_BOX_BORDER2  = RGB(255, 255,255),    /* 分组框阴影线颜色 */

    CL_MASK             = 0x7FFF    /* RGB565颜色掩码，用于文字背景透明 */
};

typedef enum
{
  TIME_DOMAIN,
  FREQ_DOMAIN
} GraphTypeEnm;

typedef enum
{
  F1_NORMAL = 1,
  F2_TOP_DISTORTION,
  F3_BOTTOM_DISTORTION,
  F4_TWOWAY_DISTORTION,
  F5_CROSSOVER_DISTORTION,
  F6_INPUT_SOURCE,
  F7_AUTO_SERIAL_AP,
} ButtonTypeEnm;

typedef struct Button
{
  uint16_t x;   //按键的x起始坐标
  uint16_t y;   //按键的y起始坐标
  ButtonTypeEnm  param;     //附加描述
  uint8_t  touch_flag; //按键按下的标志

  void (*btn_command)(struct Button * btn);  //按键功能函数

} Button;


void GUI_Init(void);
void Touch_Press(uint16_t x,uint16_t y);
void Touch_Release(uint16_t x,uint16_t y);


#endif /* __GUI_H */

