/**
  ******************************************************************************
  * @file    dsp.h
  * @author  yxnan@pm.me
  * @date    2020-10-11
  * @brief   digital signal processing functions header
  ******************************************************************************
  * @attention
  *
  * 1. fft_q15    - Signed int16 RFFT
  * 2. mag_q15    - Signed int16 complex magnitude
  * 3. vpp_q15    - Vpp
  * 4. freq_u16   - Detect fundamental frequency of signal in freq domain
  * 5. THD_to_nth - Calculate signal's THD to n-th harmonic
  *
  ******************************************************************************
  */

#ifndef __DSP_H
#define __DSP_H

#include "stm32f10x.h"
#include "macro.h"

#define FFT_SIZE        4096
#define FFT_REPEAT      2

#if (FFT_SIZE != 1024) && (FFT_SIZE != 2048) && (FFT_SIZE != 4096)
    #error "Invalid FFT_SIZE"
#endif

#define WINDOW_TYPE       hanning
#define FFT_TYPE          arm_rfft_sR_q15_len

#define WINDOW            M_CONC3(WINDOW_TYPE, _ ,FFT_SIZE)
#define FFT_INSTANCE      M_CONC3(FFT_TYPE, ,FFT_SIZE)

#define FREQ_MODIFIER           27.0f // *修正量*
#define SAMPLE_FREQ_MODIFIER    0.0f  // *修正量 x2*
#define SAMPLE_FREQ             (SAMPLE_FREQ_MODIFIER + 107.142f) // kHz
/*              (72MHz / RCC_PCLK2_Div8)
 *  ==  ------------------------------------------
 *      (ADC_SampleTime_71Cycles5 + (Tconv:=12.5))
 */


#define VDD                 3.3f
#define ADC_RANGE           (1<<12)
#define TO_VOLTAGE(QUANT)   ((float)QUANT/ADC_RANGE*VDD)
#define DC_OFFSET           TO_VOLTAGE(1.65f)
#define VPP_SCALAR          0.993f  // *修正量 x3*
#define MAG_UPPER_THRESH    40
#define MAG_LOWER_THRESH    15

void fft_q15(int16_t sampleBufferQ[], int16_t fftBufferQ[]);
void mag_q15(int16_t fftBufferQ[], uint16_t magBufferU[]);
float vpp_q15(int16_t sampleBufferQ[]);
float freq_u16(uint16_t magBufferU[]);
float THD_to_nth(uint16_t magBufferU[], uint16_t fft_rank);

#endif /* __DSP_H */