/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  yxnan@pm.me
  * @date    2020-10-04
  * @brief   stm32f103 led control support header
  ******************************************************************************
  * @attention
  *
  * ASSUME all LED on the same GPIO port
  * TODO: PWM controled full-color output.
  *
  ******************************************************************************
  */
#define __LED_C

#include "bsp_led.h"

void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED_RED_PIN | LED_GREEN_PIN | LED_BLUE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    LED_OFFALL();
}