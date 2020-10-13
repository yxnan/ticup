#include "bsp_adc.h"

__IO int16_t ADC_PrimBuffer[ADC_BUFFER_SIZE+DUMMY_OFFSET];
__IO int16_t *ADC_Buffer = &ADC_PrimBuffer[DUMMY_OFFSET];

/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 打开 ADC IO端口时钟
    ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );

    // 配置 ADC IO 引脚模式
    GPIO_InitStructure.GPIO_Pin = ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

    // 初始化 ADC IO
    GPIO_Init(ADC_PORT, &GPIO_InitStructure);
}

/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
void ADCx_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
    ADC_APBxClock_FUN ( ADC_CLK, ENABLE );

    DMA_DeInit(ADC_DMA_CHANNEL);

    DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC_x->DR ) );
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_PrimBuffer;

    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE+DUMMY_OFFSET;

    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
    DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    ADC_Init(ADC_x, &ADC_InitStructure);

    // 配置ADC时钟为CLK2的8分频，即9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // 配置 ADC 通道的转换顺序和采样时间
    ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, ADC_SampleTime_71Cycles5);
    // ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_1Cycles5);
    // ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_1Cycles5);
    // ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_1Cycles5);

    // 使能ADC DMA 请求
    ADC_DMACmd(ADC_x, ENABLE);

    // 开启ADC电源
    ADC_Cmd(ADC_x, ENABLE);

    // 由于没有采用外部触发，所以使用软件触发ADC转换
    // ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}


/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
    ADCx_GPIO_Config();
    ADCx_Mode_Config();

    // 初始化ADC 校准寄存器
    ADC_ResetCalibration(ADC_x);
    // 等待校准寄存器初始化完成
    while(ADC_GetResetCalibrationStatus(ADC_x));

    // ADC开始校准
    ADC_StartCalibration(ADC_x);
    // 等待校准完成
    while(ADC_GetCalibrationStatus(ADC_x));
}
/*********************************************END OF FILE**********************/
