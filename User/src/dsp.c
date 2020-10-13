/**
  ******************************************************************************
  * @file    dsp.c
  * @author  yxnan@pm.me
  * @date    2020-10-11
  * @brief   digital signal processing functions
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

#include "dsp.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "window.h"
#include <math.h>
#include <stdio.h>

extern const arm_rfft_instance_q15 arm_rfft_sR_q15_len1024;
extern const arm_rfft_instance_q15 arm_rfft_sR_q15_len2048;
extern const arm_rfft_instance_q15 arm_rfft_sR_q15_len4096;


static uint16_t freq_fund_index_u16(uint16_t magBufferU[]);
static int16_t vpp_in(int16_t bufferQ[], uint16_t start, uint16_t bufsize);
static void max_u16_around(uint16_t bufferU[], uint16_t center, uint16_t width,
    uint16_t *result, uint16_t *pos);


float freq_u16(uint16_t magBufferU[])
{
    return (float)freq_fund_index_u16(magBufferU)/FFT_SIZE
            * SAMPLE_FREQ*1000 + FREQ_MODIFIER;
}

float vpp_q15(int16_t sampleBufferQ[])
{
    const uint16_t group_size = 256;
    const uint16_t group_count = FFT_SIZE/group_size;
    uint16_t result = 0;
    uint16_t vpp_by_group;

    for (uint16_t i = 0; i < FFT_SIZE/group_size; i++)
    {
        vpp_by_group = vpp_in(sampleBufferQ, i*group_size, group_size);
        result += vpp_by_group / group_count;
    }
    return TO_VOLTAGE(result) * VPP_SCALAR;
}

void fft_q15(int16_t sampleBufferQ[], int16_t fftBufferQ[])
{
    for (uint16_t i = 0; i < FFT_SIZE; i++)
        sampleBufferQ[i] = sampleBufferQ[i] * WINDOW[i];

    arm_rfft_q15(&FFT_INSTANCE, sampleBufferQ, fftBufferQ);
}

void mag_q15(int16_t fftBufferQ[], uint16_t magBufferU[])
{
    // arm_cmplx_mag_q15(fftBufferQ, magBufferU, FFT_SIZE/2);
    int16_t real, imag;
    int16_t *pSrc = fftBufferQ;
    uint16_t *pDst = magBufferU;
    int32_t acc0, acc1;
    uint16_t numSamples = FFT_SIZE/2;

    while (numSamples > 0)
    {
        /* out = sqrt(real * real + imag * imag) */
        real = *pSrc++;
        imag = *pSrc++;

        acc0 = (real * real);
        acc1 = (imag * imag);

        // arm_sqrt_q15((int16_t) (((int64_t) acc0 + acc1) >> 17), pDst++);
        *pDst = sqrt((float)(acc0 + acc1));
        pDst++;

        numSamples--;
    }
}



float THD_to_nth(uint16_t magBufferU[], uint16_t fft_rank)
{
    uint16_t mag_harmonic, _;
    uint32_t sqaure_sum = 0;
    uint16_t freq_fund = freq_fund_index_u16(magBufferU);
    for (uint16_t i = 2; i <= fft_rank; i++)
    {
        max_u16_around(magBufferU, i*freq_fund, 16, &mag_harmonic, &_);
        sqaure_sum += mag_harmonic*mag_harmonic;
    }

    max_u16_around(magBufferU, freq_fund, 16, &mag_harmonic, &_);

    return sqrt(sqaure_sum) / mag_harmonic;

}

static uint16_t freq_fund_index_u16(uint16_t magBufferU[])
{
    uint16_t _, freq_fund_index;
    for (uint16_t i = 5; i < FFT_SIZE; i++)
    {
        if (magBufferU[i] > MAG_UPPER_THRESH)
        {
            max_u16_around(magBufferU, i, 8, &_, &freq_fund_index);
            break;
        }
    }

    return freq_fund_index;
}

static int16_t vpp_in(int16_t bufferQ[], uint16_t start, uint16_t bufsize)
{
    int16_t max = bufferQ[start];
    int16_t min = bufferQ[start];
    for (uint16_t i = 1; i < bufsize; i++)
    {
        if (max < bufferQ[i])
            max = bufferQ[i];
        else if (min > bufferQ[i])
            min = bufferQ[i];
    }
    return max - min;
}

static void
max_u16_around(uint16_t bufferU[], uint16_t center, uint16_t width, uint16_t *result, uint16_t *pos)
{
    uint16_t *pt = &bufferU[center - width/2];
    *result = *pt++;
    *pos = 0;

    for (uint16_t i = 0; i < width; pt++, i++)
    {
        if (*result < *pt)
        {
            *result = *pt;
            *pos = i;
        }
    }

    *pos += center - width/2;
}