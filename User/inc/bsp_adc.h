#ifndef __ADC_H
#define __ADC_H


#include "stm32f10x.h"
#include "dsp.h"

// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_x                         ADC1
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA
#define    ADC_PORT                      GPIOA

// 转换通道个数
#define    ADC_BUFFER_SIZE               (FFT_SIZE * FFT_REPEAT)
#define    DUMMY_OFFSET                  (ADC_BUFFER_SIZE/16)

#define    ADC_PIN                       GPIO_Pin_4
#define    ADC_CHANNEL                   ADC_Channel_4


// ADC1 对应 DMA1通道1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1
#define    ADC_DMA_FLAG_TC               DMA1_FLAG_TC1


// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

/**************************函数声明********************************/
void                ADCx_Init                               (void);
void                ADCx_Mode_Config                        (void);


#endif /* __ADC_H */

