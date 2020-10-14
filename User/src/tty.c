/**
  ******************************************************************************
  * @file    tty.c
  * @author  yxnan@pm.me
  * @date    2020-09-26
  * @brief   stm32f103 tty debug support
  ******************************************************************************
  * @attention
  *
  * There is no stream buffering in the hardware or HAL driver
  * Handling the string manually through USART is recommended.
  *
  ******************************************************************************
  */
#define __TTY_C

#include "tty.h"

void tty_init(void)
{
    GPIO_InitTypeDef GPIO_Initstructure;
    USART_InitTypeDef USART_Initstructure;

    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
    DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

    GPIO_Initstructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_Initstructure);

    GPIO_Initstructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_Initstructure);

    USART_Initstructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
    USART_Initstructure.USART_WordLength = USART_WordLength_8b;
    USART_Initstructure.USART_StopBits = USART_StopBits_1;
    USART_Initstructure.USART_Parity = USART_Parity_No ;
    USART_Initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEBUG_USARTx, &USART_Initstructure);

    USART_Cmd(DEBUG_USARTx, ENABLE);
}



#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
    #define GETCHAR_PROTOTYPE int __io_getchar(void)
    #define PUTCHAR               __io_putchar
    #define GETCHAR               __io_getchar
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
    #define GETCHAR_PROTOTYPE int fgetc(FILE * f)
    #define PUTCHAR               fputc
    #define GETCHAR               fgetc
#endif  /* __GNUC__ */

static PUTCHAR_PROTOTYPE
{
    USART_SendData(DEBUG_USARTx, (uint8_t) ch);
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
    return ch;
}

static GETCHAR_PROTOTYPE
{
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
    return USART_ReceiveData(DEBUG_USARTx);
}

/* redirect printf and scanf
    NOTE:
    There is NO stream buffering in the hardware or HAL driver, except you implement it.
    When scanf is implemented straightforward using _read, you must always call scanf
    *before* there is any input on the RX line or you will get overrun errors.

    And don't forget to remove the stdio internal buffering. A plain call

        scanf("%d",i);

    ends up a in call to _read with a 1024 (BUFSIZ) char buffer!
    i.e. _read won't return before the UART supplied 1024 chars of input.
    This is not what you typically want for an interactive comm.
    You may remove that buffering by using

        setvbuf( stdin, NULL, _IONBF, 0 );
        scanf("%d",i);

    which results in calls to _read single char by single char.

    Anyway, I would prefer collecting a string manually using USART
    functions and then using sscanf on it.
*/

/* -------------------------------------
int _write(int file, char *ptr, int len)
{
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}

int _read(int file, char *ptr, int len)
{
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        *ptr++ = __io_getchar();
    }

    return len;
}
------------------------------------- */

/*
 * putstr
 *  Output a '\0'-terminated string
 */
void putstr(char *ptr)
{
    while (*ptr != '\0')
    {
        PUTCHAR(*ptr);
        ptr++;
    }
}

/*
 * readstr
 *  Read a '\0'-terminated string with limited buffer size
 *  CRLF not included
 *  Input echo on
 *  Backspace ignored
 */
void readstr(char *ptr, size_t buflen)
{
    size_t i = 0;
    while ( i != buflen && (*ptr=GETCHAR()) != '\r' )
    {
        if (*ptr != '\b')
        {
            PUTCHAR(*ptr);
            ptr++;
            i++;
        }
    }
    PUTCHAR('\r');
    PUTCHAR('\n');
    *ptr = '\0';
}

/*
 * readu32
 *  Read a uint32 type integer, following a newline
 *  Skips non-numeric inputs
 *  Input echo on
 *  Backspace supported
 */
void readu32(uint32_t *uint32)
{
    char ch;
    uint32_t res = 0;
    size_t valid_cnt = 0;

    while((ch=GETCHAR()) != '\r')
    {
        if (ch >= '0' && ch <= '9')
        {
            res = res*10 + (ch - '0');
            PUTCHAR(ch);
            valid_cnt++;
        }
        else if (ch == '\b' && valid_cnt > 0)
        {
            res /= 10;
            PUTCHAR('\b');
            PUTCHAR(' ');
            PUTCHAR('\b');
            valid_cnt--;
        }
    }
    *uint32 = res;
}


/* ------------- DMA ------------- */

// void tty_dma_tx_config(uint32_t buffer)
// {
//     DMA_InitTypeDef DMA_InitStructure;

//     RCC_AHBPeriphClockCmd(DEBUG_DMA_CLK, ENABLE);
//     DMA_DeInit(DEBUG_DMA_CH_TX);

//     DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(DEBUG_USARTx->DR));
//     DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;
//     DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

//     DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//     DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

//     DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//     DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

//     DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//     DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

//     DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

//     DMA_Init(DEBUG_DMA_CH_TX, &DMA_InitStructure);
// }

// void tty_dma_send(size_t buflen)
// {
//     DMA_Cmd(DEBUG_DMA_CH_TX, DISABLE);
//     DEBUG_DMA_CH_TX->CNDTR = buflen;
//     DMA_Cmd(DEBUG_DMA_CH_TX, ENABLE);
// }
