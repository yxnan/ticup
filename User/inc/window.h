/**
  ******************************************************************************
  * @file    window.h
  * @author  yxnan@pm.me
  * @date    2020-10-12
  * @brief   window function coefficients header
  ******************************************************************************
  * @attention
  *
  * C89 doesn't allow the trailing comma.
  * C99 does allow it.
  *
  ******************************************************************************
  */

#ifndef __WINDOW_H
#define __WINDOW_H

extern const float hanning_4096[];
extern const float hanning_2048[];
extern const float hanning_1024[];
extern const float flattop_4096[];
extern const float flattop_2048[];
extern const float flattop_1024[];
extern const float blackman_4096[];
extern const float blackman_2048[];
extern const float blackman_1024[];

#endif /* __WINDOW_H */