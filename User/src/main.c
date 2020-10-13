#include "stm32f10x.h"
#include "lcd_ili9341.h"
#include "lcd_xpt2046.h"
#include "bsp_led.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "gui.h"
#include "delay.h"
#include "gpio_switch.h"
#ifdef DEBUG
#include "tty.h"
#endif

int main(void)
{
    LED_GPIO_Config();
    switch_config();
    delay_init();

    #ifdef DEBUG
    tty_init();
    #endif

    ILI9341_Init();
    XPT2046_Init();

    ADCx_Init();
    delay_us(1);

    BASIC_TIM_Init();

    GUI_Init();

    while (1)
    {
        //触摸检测函数，本函数至少10ms调用一次
        XPT2046_TouchEvenHandler();
    }
}


/* ------------------------------------------end of file---------------------------------------- */

