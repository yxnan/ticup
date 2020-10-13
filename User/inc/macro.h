/**
  ******************************************************************************
  * @file    macro.h
  * @author  yxnan@pm.me
  * @date    2020-10-13
  * @brief   auxiliary macros
  ******************************************************************************
  * @attention
  *
  * Inspired by:
  *     https://stackoverflow.com/questions/28231743/self-unrolling-macro-loop-in-c-c/
  *
  ******************************************************************************
  */

#ifndef __MACRO_H
#define __MACRO_H

/* --------------------------------------------------------------------
    #define S var
    #define F func

    M_SEQ_ASSIGN(3, S, F) -> var3 = func(2); var2 = func(1); var1 = func(0)
*/
#define M_SEQ_ASSIGN_1(S, F) S##1 = F(0)
#define M_SEQ_ASSIGN_2(S, F) S##2 = F(1); M_SEQ_ASSIGN_1(S, F)
#define M_SEQ_ASSIGN_3(S, F) S##3 = F(2); M_SEQ_ASSIGN_2(S, F)
#define M_SEQ_ASSIGN_4(S, F) S##4 = F(3); M_SEQ_ASSIGN_3(S, F)
#define M_SEQ_ASSIGN_5(S, F) S##5 = F(4); M_SEQ_ASSIGN_4(S, F)

#define M_EXPAND(...) __VA_ARGS__

#define M_SEQ_ASSIGN__(N, S, F) M_EXPAND(M_SEQ_ASSIGN_ ## N)(S, F)
#define M_SEQ_ASSIGN_(N, S, F) M_SEQ_ASSIGN__(N, S, F)
#define M_SEQ_ASSIGN(N, S, F) M_SEQ_ASSIGN_(M_EXPAND(N), S, F)

/* --------------------------------------------------------------------
    #define WINDOW_TYPE     hanning
    #define FFT_SIZE        4096

    M_CONC3(WINDOW_TYPE, _ ,FFT_SIZE) -> hanning_4096
*/

#define M_CONC3_(A, B, C)  A##B##C
#define M_CONC3(A, B, C)  M_CONC3_(A, B, C)


#endif /* __MACRO_H */


