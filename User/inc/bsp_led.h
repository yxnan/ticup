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

#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "sys.h"

/* MODIFY THIS if LED on different port */
#define LED_PORT            GPIOB
#define LED_CLK             RCC_APB2Periph_GPIOB

// R
#define LED_RED_PORT        LED_PORT    // reserved
#define LED_RED_CLK         LED_CLK
#define LED_RED_PIN         GPIO_Pin_5
#define LED_RED_OUT         PBout(5)

// G
#define LED_GREEN_PORT      LED_PORT
#define LED_GREEN_CLK       LED_CLK
#define LED_GREEN_PIN       GPIO_Pin_0
#define LED_GREEN_OUT       PBout(0)

// B
#define LED_BLUE_PORT       LED_PORT
#define LED_BLUE_CLK        LED_CLK
#define LED_BLUE_PIN        GPIO_Pin_1
#define LED_BLUE_OUT        PBout(1)


/** the macro definition to trigger the led on or off
  * 1 - off
  * 0 - on
  */
#define ON  0
#define OFF 1


/* basic colors */

// red
#define LED_RED() \
    do {\
        LED_RED_OUT   = ON; \
        LED_GREEN_OUT = OFF; \
        LED_BLUE_OUT  = OFF; \
    } while(0)


// green
#define LED_GREEN() \
    do {\
        LED_RED_OUT   = OFF; \
        LED_GREEN_OUT = ON; \
        LED_BLUE_OUT  = OFF; \
    } while(0)


// blue
#define LED_BLUE() \
    do {\
        LED_RED_OUT   = OFF; \
        LED_GREEN_OUT = OFF; \
        LED_BLUE_OUT  = ON; \
    } while(0)


// yellow = red + green
#define LED_YELLOW() \
    do {\
        LED_RED_OUT   = ON; \
        LED_GREEN_OUT = ON; \
        LED_BLUE_OUT  = OFF; \
    } while(0)


// purple = red + blue
#define LED_PURPLE() \
    do {\
        LED_RED_OUT   = ON; \
        LED_GREEN_OUT = OFF; \
        LED_BLUE_OUT  = ON; \
    } while(0)


// cyan = green + blue
#define LED_CYAN() \
    do {\
        LED_RED_OUT   = OFF; \
        LED_GREEN_OUT = ON; \
        LED_BLUE_OUT  = ON; \
    } while(0)


// white = red + green + blue
#define LED_WHITE() \
    do {\
        LED_RED_OUT   = ON; \
        LED_GREEN_OUT = ON; \
        LED_BLUE_OUT  = ON; \
    } while(0)


// off all
#define LED_OFFALL() \
    do {\
        LED_RED_OUT   = OFF; \
        LED_GREEN_OUT = OFF; \
        LED_BLUE_OUT  = OFF; \
    } while(0)


void LED_GPIO_Config(void);

#endif /* __LED_H */
