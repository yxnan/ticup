/**
  ******************************************************************************
  * @file    tty.h
  * @author  yxnan@pm.me
  * @date    2020-09-26
  * @brief   stm32f103 tty debug support header
  ******************************************************************************
  * @attention
  *
  * There is no stream buffering in the hardware or HAL driver
  * Handling the string manually through USART is recommended.
  *
  ******************************************************************************
  */

#ifndef __TTY_H
#define	__TTY_H

#include "stm32f10x.h"
#include <stdio.h>

#define READSTR_MAX_BUFSIZE     100
#define STRBUF                  ewf32r9823g8398dbhq38

#ifdef __TTY_C
char STRBUF[READSTR_MAX_BUFSIZE];
#else
extern char STRBUF[READSTR_MAX_BUFSIZE];
#endif

#define tty_print(...) \
    do {\
        sprintf(STRBUF, __VA_ARGS__); \
        putstr(STRBUF);\
    } while(0)

#define tty_scan(...) \
    do {\
        readstr(STRBUF, READSTR_MAX_BUFSIZE);\
        sscanf(STRBUF, __VA_ARGS__); \
    } while(0)

// USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
// #define  DEBUG_DMA_CLK                  RCC_AHBPeriph_DMA1
// #define  DEBUG_DMA_CH_TX                DMA1_Channel4
// #define  DEBUG_DMA_CH_RX                DMA1_Channel5
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

#define  DEBUG_USART_TX_GPIO_PORT       GPIOA
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

/* -------------------------------------------------------- */

// USART2
// #define  DEBUG_USARTx                   USART2
// #define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
// #define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
// #define  DEBUG_USART_BAUDRATE           115200

// USART GPIO
// #define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
// #define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

// #define  DEBUG_USART_TX_GPIO_PORT       GPIOA
// #define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
// #define  DEBUG_USART_RX_GPIO_PORT       GPIOA
// #define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

// #define  DEBUG_USART_IRQ                USART2_IRQn
// #define  DEBUG_USART_IRQHandler         USART2_IRQHandler

/* -------------------------------------------------------- */

// USART3
// #define  DEBUG_USARTx                   USART3
// #define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
// #define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
// #define  DEBUG_USART_BAUDRATE           115200

// USART GPIO
// #define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
// #define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

// #define  DEBUG_USART_TX_GPIO_PORT       GPIOB
// #define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
// #define  DEBUG_USART_RX_GPIO_PORT       GPIOB
// #define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

// #define  DEBUG_USART_IRQ                USART3_IRQn
// #define  DEBUG_USART_IRQHandler         USART3_IRQHandler

/* -------------------------------------------------------- */

// UART4
// #define  DEBUG_USARTx                   UART4
// #define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
// #define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
// #define  DEBUG_USART_BAUDRATE           115200

// USART GPIO
// #define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
// #define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

// #define  DEBUG_USART_TX_GPIO_PORT       GPIOC
// #define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
// #define  DEBUG_USART_RX_GPIO_PORT       GPIOC
// #define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

// #define  DEBUG_USART_IRQ                UART4_IRQn
// #define  DEBUG_USART_IRQHandler         UART4_IRQHandler

/* -------------------------------------------------------- */

// UART5
// #define  DEBUG_USARTx                   UART5
// #define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
// #define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
// #define  DEBUG_USART_BAUDRATE           115200

// USART GPIO
// #define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
// #define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

// #define  DEBUG_USART_TX_GPIO_PORT       GPIOC
// #define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_12
// #define  DEBUG_USART_RX_GPIO_PORT       GPIOD
// #define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

// #define  DEBUG_USART_IRQ                UART5_IRQn
// #define  DEBUG_USART_IRQHandler         UART5_IRQHandler

/* -------------------------------------------------------- */

void tty_init(void);
void putstr(char *ptr);
void readstr(char *ptr, size_t buflen);
void readu32(uint32_t *uint32);
// void tty_dma_tx_config(uint32_t buffer);
// void tty_dma_send(size_t buflen);

#endif /* __TTY_H */
