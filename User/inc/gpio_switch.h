#ifndef __SWITCH_H
#define __SWITCH_H

#include "stm32f10x.h"
#include "sys.h"

#define S1                  PAout(2)
#define S2                  PAout(3)
#define S3                  PAout(8)
#define S4                  PAout(11)
#define S5                  PAout(12)

#define SWITCH_GPIO_CLK     RCC_APB2Periph_GPIOA
#define SWITCH_PORT         GPIOA
#define SWITCH_PIN1         GPIO_Pin_2
#define SWITCH_PIN2         GPIO_Pin_3
#define SWITCH_PIN3         GPIO_Pin_8
#define SWITCH_PIN4         GPIO_Pin_11
#define SWITCH_PIN5         GPIO_Pin_12
#define SWITCH_PIN_ALL      (SWITCH_PIN1 | SWITCH_PIN2 | SWITCH_PIN3 | SWITCH_PIN4 | SWITCH_PIN5)

#define SRC_GPIO_CLK        RCC_APB2Periph_GPIOB
#define SRC_PORT            GPIOB
#define SRC_PIN             GPIO_Pin_5
#define SRC                 PBout(5)

/*
| 输出/配置  | （S1）削顶 | （S2）削底 | （S3）选道1 | （S4）选道2  | （S5）选道3 |
|:-------: | :-------: | :-------: | :--------: | :--------: | :--------: |
|   正弦    |     0     |     1     |     0      |     1      |     1      |
|   削顶    |     1     |     1     |     1      |     0      |     1      |
|   削底    |     0     |     0     |     1      |     0      |     1      |
|   双向    |     1     |     0     |     1      |     0      |     1      |
|   交越    |     0     |     1     |     1      |     1      |     0      |
*/
                                                //         8421
#define CONF_SINE(n)            (0x1A>>n & 1U)  // 0b'0001 1010
#define CONF_TOP(n)             (0x17>>n & 1U)  // 0b'0001 0111
#define CONF_BOTTOM(n)          (0x14>>n & 1U)  // 0b'0001 0100
#define CONF_TWOWAY(n)          (0x15>>n & 1U)  // 0b'0001 0101
#define CONF_CROSSOVER(n)       (0x0E>>n & 1U)  // 0b'0000 1110

typedef enum
{
  S1_SINE = 1,
  S2_TOP,
  S3_BOTTOM,
  S4_TWOWAY,
  S5_CROSSOVER,
} SwitchModeEnm;

void switch_config();
void set_switch_mode(SwitchModeEnm mode);
SwitchModeEnm get_switch_mode(void);
void src_toggle(void);
uint8_t get_src(void);


#endif /* __SWITCH_H */


