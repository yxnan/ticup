#include "gpio_switch.h"
#include "macro.h"

static SwitchModeEnm CurrentMode = S1_SINE;
static uint8_t CurrentSource = 1;

void switch_config()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(SWITCH_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(SRC_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SWITCH_PIN_ALL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SWITCH_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SRC_PIN;
    GPIO_Init(SRC_PORT, &GPIO_InitStructure);

    GPIO_SetBits(SRC_PORT, SRC_PIN);
}

void set_switch_mode(SwitchModeEnm mode)
{
    CurrentMode = mode;
    switch (CurrentMode)
    {
        case S1_SINE:      M_SEQ_ASSIGN(5, S, CONF_SINE); break;
        case S2_TOP:       M_SEQ_ASSIGN(5, S, CONF_TOP); break;
        case S3_BOTTOM:    M_SEQ_ASSIGN(5, S, CONF_BOTTOM); break;
        case S4_TWOWAY:    M_SEQ_ASSIGN(5, S, CONF_TWOWAY); break;
        case S5_CROSSOVER: M_SEQ_ASSIGN(5, S, CONF_CROSSOVER); break;
        default: break;
    }
}

SwitchModeEnm get_switch_mode(void)
{
    return CurrentMode;
}

void src_toggle(void)
{
    CurrentSource = !CurrentSource;
    SRC = CurrentSource;
}

uint8_t get_src(void)
{
    return CurrentSource;
}