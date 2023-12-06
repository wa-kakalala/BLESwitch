#ifndef _UART_H_
#define _UART_H_
#include <reg51.h>
// define type u8
typedef unsigned char  u8;

#define UART_RXBUF_LEN 6
#define UART_RX_LEN    6

extern u8 uart_rx_buf[UART_RXBUF_LEN];
extern u8 rx_enable;

void Uart_Init(void);
void Uart_SendByte( u8 tx_data );
void Uart_LoopTest( void );
#endif