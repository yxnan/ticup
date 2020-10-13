#ifndef __FONT_H
#define __FONT_H

#include "stm32f10x.h"

typedef struct _tFont
{
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;

} sFONT;

extern sFONT Font24x32;
extern sFONT Font16x24;
extern sFONT Font8x16;

#define      WIDTH_CH_CHAR       (((sFONT *)LCD_GetFont())->Width)
#define      HEIGHT_CH_CHAR      (((sFONT *)LCD_GetFont())->Height)


#define LINE(x)  ((x) * (WIDTH_CH_CHAR))
#define LINEY(x) ((x) * (HEIGHT_CH_CHAR))


#endif /*end of __FONT_H    */
