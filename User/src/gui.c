/**
  ******************************************************************************
  * @file    gui.c
  * @author  yxnan@pm.me
  * @date    2020-08-02
  * @brief   stm32f103 simple GUI interface
  ******************************************************************************
  */

#include "gui.h"
#include "dsp.h"
#include "gpio_switch.h"
#include "lcd_xpt2046.h"
#include "lcd_ili9341.h"
#include "bsp_adc.h"
#include "bsp_led.h"
#include "bsp_tim.h"
#include <stdio.h>

#ifdef DEBUG
#include "tty.h"
static void serial_send(Button *btn);
#endif

uint8_t AutoMode = 1;
__IO uint32_t time_tick = 0;

Button button[BUTTON_NUM];

static char labelMode[ LABEL_BUFFER_SIZE ];
static char labelTHD[ LABEL_BUFFER_SIZE ];
static char labelVpp[ LABEL_BUFFER_SIZE ];
static char labelFreq[ LABEL_BUFFER_SIZE ];

extern __IO int16_t *ADC_Buffer;

int16_t  FFT_Buffer[FFT_SIZE * 2];
uint16_t FFT_MagBuffer[FFT_SIZE];

static void update_msg(void);
static void clear_graph(void);
static void def_button(void);
static void draw_button(Button *btn);
static void plot_graph(uint16_t buffer[], GraphTypeEnm graphType);
static void start_sampling(void);
static void calculate_params();
static void switch_mode(Button *btn);
static void switch_source(Button *btn);
static void switch_auto(Button *btn);


void GUI_Init(void)
{
    char textBuffer[2];

    /* 整屏清为白色 */
    LCD_SetBackColor(CL_WHITE);
    ILI9341_Clear(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);

    /* 初始化按钮 */
    def_button();

    /* 描绘按钮 */
    for (uint16_t i = 0; i < BUTTON_NUM; i++)
    {
        draw_button(&button[i]);
    }

    // 默认模式为F1
    button[0].btn_command(&button[0]);

    // 描绘坐标系
    LCD_SetTextColor(CL_BLACK);

    ILI9341_DispString_EN(  GRAPH_START_X - 5*WIDTH_CH_CHAR,
                            GRAPH_START_Y - HEIGHT_CH_CHAR/2,
                            "3.3V ");
    ILI9341_DrawLine(   GRAPH_START_X,
                        GRAPH_START_Y,
                        GRAPH_END_X,
                        GRAPH_START_Y);

    ILI9341_DispString_EN(  GRAPH_START_X - 5*WIDTH_CH_CHAR,
                            GRAPH_END_Y - HEIGHT_CH_CHAR/2,
                            "  0V ");
    ILI9341_DrawLine(   GRAPH_START_X,
                        GRAPH_END_Y,
                        GRAPH_END_X,
                        GRAPH_END_Y);

    // draw freq axis, aligned manually.
    LCD_SetTextColor(CH2_COLOR);
    uint16_t x = GRAPH_START_X+8;
    for (uint16_t i = 1; i <= 9; i++)
    {
        sprintf(textBuffer, "%d", i);
        ILI9341_DispString_EN(x, GRAPH_END_Y + (GRAPH_BOTTOM-HEIGHT_CH_CHAR)/2, textBuffer);
        x += 10;
        if (i > 3)
            x++;
    }

    ILI9341_DispString_EN(x, GRAPH_END_Y + (GRAPH_BOTTOM-HEIGHT_CH_CHAR)/2, " ... kHz");
}

static void update_msg(void)
{
    LCD_SetBackColor(CL_WHITE);
    ILI9341_Clear(LABEL1_START_X, LABEL1_START_Y, LCD_X_LENGTH, 4*HEIGHT_CH_CHAR);

    // 文字信息
    LCD_SetFont(&Font8x16);
    LCD_SetTextColor(CL_RED);

    ILI9341_DispString_EN(LABEL1_START_X, LABEL1_START_Y, labelMode);
    ILI9341_DispString_EN(LABEL2_START_X, LABEL2_START_Y, labelTHD);
    ILI9341_DispString_EN(LABEL3_START_X, LABEL3_START_Y, labelVpp);
    ILI9341_DispString_EN(LABEL4_START_X, LABEL4_START_Y, labelFreq);
}

#ifdef DEBUG
static void serial_send(Button *btn)
{
    for (uint16_t i = 0; i < FFT_SIZE/2; i++)
        tty_print("%d ", FFT_MagBuffer[i]);
}
#endif

static void clear_graph(void)
{
    ILI9341_Clear(  GRAPH_START_X,
                    GRAPH_START_Y + 1,
                    LCD_X_LENGTH,
                    GRAPH_END_Y - GRAPH_START_Y - 1);
}

static void plot_graph(uint16_t buffer[], GraphTypeEnm graphType)
{
    uint16_t *pt = buffer;
    uint16_t pt_x, pt_y;
    uint16_t prev_x, prev_y;
    uint16_t linecolor;
    uint16_t x_scalar;
    float y_scalar;

    if (graphType == TIME_DOMAIN)
    {
        x_scalar = 1;
        y_scalar = (float)ADC_RANGE;
        linecolor = CH1_COLOR;
    }
    else
    {
        x_scalar = FFT_SIZE/1024;
        y_scalar = 500.0f;  // 经验值
        linecolor = CH2_COLOR;
    }

    LCD_SetTextColor(linecolor);
    prev_x = pt_x = GRAPH_START_X;
    prev_y = LCD_Y_LENGTH - ((*pt/y_scalar)*GRAPH_HEIGHT + GRAPH_BOTTOM + 1);
    if (prev_y <= GRAPH_START_Y)
        prev_y = GRAPH_START_Y + 1;

    for (uint16_t i = 1; pt_x<=GRAPH_END_X && i<ADC_BUFFER_SIZE; i++)
    {
        // 保证画FFT图像时不会跳过峰值点
        if (graphType == FREQ_DOMAIN)
        {
            uint16_t j;
            for (j = 1; j < x_scalar; j++)
                if (*(pt+j) > MAG_LOWER_THRESH) break;

            pt += j;
        }
        else
        {
            pt += x_scalar;
        }

        pt_x++;
        pt_y = LCD_Y_LENGTH - ((*pt/y_scalar)*GRAPH_HEIGHT + GRAPH_BOTTOM + 1);
        if (pt_y <= GRAPH_START_Y)
            pt_y = GRAPH_START_Y + 1;

        // ILI9341_SetPointPixel(pt_x, pt_y);
        ILI9341_DrawLine(prev_x, prev_y, pt_x, pt_y);
        prev_x = pt_x;
        prev_y = pt_y;
    }
}

void def_button(void)
{
    button[0].param       = F1_NORMAL;
    button[0].x           = BUTTON_START_X;
    button[0].y           = BUTTON_START_Y;
    button[0].touch_flag  = 0;
    button[0].btn_command = switch_mode;

    button[1].param       = F2_TOP_DISTORTION;
    button[1].x           = BUTTON_START_X;
    button[1].y           = BUTTON_START_Y + 1*BUTTON_HEIGHT + 1*BUTTON_GAP;
    button[1].touch_flag  = 0;
    button[1].btn_command = switch_mode;

    button[2].param       = F3_BOTTOM_DISTORTION;
    button[2].x           = BUTTON_START_X;
    button[2].y           = BUTTON_START_Y + 2*BUTTON_HEIGHT + 2*BUTTON_GAP;
    button[2].touch_flag  = 0;
    button[2].btn_command = switch_mode;

    button[3].param       = F4_TWOWAY_DISTORTION;
    button[3].x           = BUTTON_START_X;
    button[3].y           = BUTTON_START_Y + 3*BUTTON_HEIGHT + 3*BUTTON_GAP;
    button[3].touch_flag  = 0;
    button[3].btn_command = switch_mode;

    button[4].param       = F5_CROSSOVER_DISTORTION;
    button[4].x           = BUTTON_START_X;
    button[4].y           = BUTTON_START_Y + 4*BUTTON_HEIGHT + 4*BUTTON_GAP;
    button[4].touch_flag  = 0;
    button[4].btn_command = switch_mode;

    button[5].param       = F6_INPUT_SOURCE;
    button[5].x           = BUTTON_START_X + BUTTON_WIDTH + 5;
    button[5].y           = button[3].y;
    button[5].touch_flag  = 0;
    button[5].btn_command = switch_source;

    button[6].param       = F7_AUTO_SERIAL_AP;
    button[6].x           = BUTTON_START_X + BUTTON_WIDTH + 5;
    button[6].y           = button[2].y;
    button[6].touch_flag  = 0;
    #ifdef DEBUG
    button[6].btn_command = serial_send;
    #else
    button[6].btn_command = switch_auto;
    #endif
}

static void draw_button(Button *btn)
{
    char textBuffer[5];

    /* 释放按键 */
    if (btn->touch_flag == 0)
        LCD_SetTextColor(CL_GREY);
    else
        LCD_SetTextColor(CL_WHITE);

    ILI9341_DrawRectangle(btn->x, btn->y,
        BUTTON_WIDTH, BUTTON_HEIGHT, 1);

    /*按钮边框*/
    LCD_SetTextColor(CL_BLUE4);
    ILI9341_DrawRectangle(btn->x, btn->y,
        BUTTON_WIDTH, BUTTON_HEIGHT, 0);

    // 按钮文字
    LCD_SetFont(&Font8x16);
    LCD_SetTextColor(CL_RED);

    if (btn->param == F6_INPUT_SOURCE)
    {
        ILI9341_DispString_EN(  btn->x + (BUTTON_WIDTH  - 3*WIDTH_CH_CHAR)/2,
                                btn->y + (BUTTON_HEIGHT - HEIGHT_CH_CHAR)/2,
                                "SRC");
    }
    else if (btn->param == F7_AUTO_SERIAL_AP)
    {
        #ifdef DEBUG
        ILI9341_DispString_EN(  btn->x + (BUTTON_WIDTH  - 3*WIDTH_CH_CHAR)/2,
                                btn->y + (BUTTON_HEIGHT - HEIGHT_CH_CHAR)/2,
                                "dbg");
        #else
        if (AutoMode)
            ILI9341_DispString_EN(  btn->x + (BUTTON_WIDTH  - 4*WIDTH_CH_CHAR)/2,
                                    btn->y + (BUTTON_HEIGHT - HEIGHT_CH_CHAR)/2,
                                    "Stop");
        else
            ILI9341_DispString_EN(  btn->x + (BUTTON_WIDTH  - 3*WIDTH_CH_CHAR)/2,
                                    btn->y + (BUTTON_HEIGHT - HEIGHT_CH_CHAR)/2,
                                    "Run");
        #endif
    }
    else
    {
        sprintf(textBuffer, "F%d", btn->param);
        ILI9341_DispString_EN(  btn->x + (BUTTON_WIDTH  - 2*WIDTH_CH_CHAR)/2,
                                btn->y + (BUTTON_HEIGHT - HEIGHT_CH_CHAR)/2,
                                textBuffer);
    }
}

static void start_sampling(void)
{
    ADCx_Mode_Config();
    ADC_SoftwareStartConvCmd(ADC_x, ENABLE);

    while (DMA_GetFlagStatus(ADC_DMA_FLAG_TC) != SET);
    DMA_ClearFlag(ADC_DMA_FLAG_TC);

    clear_graph();
    plot_graph(ADC_Buffer, TIME_DOMAIN);

    // FFT会改变ADC_Buffer，所以要先把时域图画好
    calculate_params();
    plot_graph(FFT_MagBuffer, FREQ_DOMAIN);
    update_msg();

}

static void calculate_params(void)
{
    // Vpp
    float vpp = vpp_q15(ADC_Buffer);
    sprintf(labelVpp, "Vpp = %.3fV", vpp);

    // THD
    float THD = 0.00f;
    for (uint16_t i = 0; i < FFT_REPEAT; i++)
    {
        fft_q15(ADC_Buffer + i*FFT_SIZE, FFT_Buffer);
        mag_q15(FFT_Buffer, FFT_MagBuffer);
        THD += THD_to_nth(FFT_MagBuffer, 5) / FFT_REPEAT;
    }

    sprintf(labelTHD, "THD = %.2f%%", THD*100);

    // freq
    float freq = freq_u16(FFT_MagBuffer);
    sprintf(labelFreq, "Freq = %.3fHz", freq);
}

static void switch_mode(Button *btn)
{
    set_switch_mode(btn->param);

    switch (btn->param)
    {
        case F1_NORMAL:
            sprintf(labelMode, "F1: Normal");
            break;
        case F2_TOP_DISTORTION:
            sprintf(labelMode, "F2: Top Distortion");
            break;
        case F3_BOTTOM_DISTORTION:
            sprintf(labelMode, "F3: Bottom Distortion");
            break;
        case F4_TWOWAY_DISTORTION:
            sprintf(labelMode, "F4: Two-way Distortion");
            break;
        case F5_CROSSOVER_DISTORTION:
            sprintf(labelMode, "F5: Crossover Distortion");
            break;
        default: break;
    }

    if (!AutoMode)
        start_sampling();
}

static void switch_source(Button *btn)
{
    src_toggle();
    LED_RED_OUT = get_src();
}

static void switch_auto(Button *btn)
{
    AutoMode = !AutoMode;
}

void BASIC_TIM_IRQHandler(void)
{
    if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);
        if (AutoMode)
        {
            time_tick++;
            if (time_tick > 1000)
            {
                start_sampling();
                time_tick = 0;
            }
        }

    }
}

/**
* @brief  Touch_Press 被按下时调用的函数，由触摸屏调用
* @param  x 触摸位置的x坐标
* @param  y 触摸位置的y坐标
* @retval 无
*/
void Touch_Press(uint16_t x,uint16_t y)
{
    for(uint8_t i = 0; i < BUTTON_NUM; i++)
    {
        /* 触摸到了按钮 */
        if( x>=button[i].x && x<=button[i].x+BUTTON_WIDTH &&
            y>=button[i].y && y<=button[i].y+BUTTON_HEIGHT)
        {
            if(button[i].touch_flag == 0)     /*原本的状态为没有按下，则更新状态*/
            {
                button[i].touch_flag = 1;         /* 记录按下标志 */

                draw_button(&button[i]);  /*重绘按钮*/
            }

        }
        else if(button[i].touch_flag == 1)    /* 触摸移出了按键的范围且之前有按下按钮 */
        {
            button[i].touch_flag = 0;         /* 清除按下标志，判断为误操作*/

            draw_button(&button[i]);   /*重绘按钮*/
        }

    }
}

/**
* @brief  Touch_Release 被释放时调用的函数，由触摸屏调用
* @param  x 触摸最后释放时的x坐标
* @param  y 触摸最后释放时的y坐标
* @retval 无
*/
void Touch_Release(uint16_t x, uint16_t y)
{
    /* 在图像区域释放 */
    if (x<GRAPH_END_X && x>GRAPH_START_X && y<GRAPH_END_Y && y>GRAPH_START_Y)
    {
        start_sampling();
        return;
    }

    /* 在按钮区域释放 */
    for (uint8_t i = 0; i < BUTTON_NUM; i++)
    {
        if( x>=button[i].x && x<=button[i].x+BUTTON_WIDTH &&
            y>=button[i].y && y<=button[i].y+BUTTON_HEIGHT)
        {
            button[i].touch_flag = 0;       /*释放触摸标志*/

            button[i].btn_command(&button[i]);  /*执行按键的功能命令*/

            draw_button(&button[i]); /*重绘按钮*/

            break;
        }
    }
}

/* ------------------------------------------end of file---------------------------------------- */



