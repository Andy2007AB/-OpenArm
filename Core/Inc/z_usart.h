#ifndef __Z_USART_H__
#define __Z_USART_H__

#include "z_type.h"
#include "main.h"
#include "usart.h"

// 串口缓冲区大小
#define UART_BUF_SIZE 256

// 全局变量
extern u8 uart1_receive_buf[UART_BUF_SIZE];
extern u8 uart1_mode;
extern u8 uart1_get_ok;
extern u8 uart2_receive_buf[UART_BUF_SIZE];
extern u8 uart2_mode;
extern u8 uart2_get_ok;
extern u8 uart3_receive_buf[UART_BUF_SIZE];
extern u8 uart3_mode;
extern u8 uart3_get_ok;

// 函数声明
void tb_usart_init(void);
void uart1_send_str(u8 *str);
void uart1_send_nbyte(u8 *Data, u16 size);
void uart1_send_byte(u8 data);
void uart2_send_str(u8 *str);
void uart2_send_nbyte(u8 *Data, u16 size);
void uart2_send_byte(u8 data);
void zx_uart_send_str(u8 *str);
void uart3_send_str(u8 *str);
void uart3_send_nbyte(u8 *Data, u16 size);
void uart3_send_byte(u8 data);
void uart1_open(void);
void uart2_open(void);
void uart3_open(void);
void tb_interrupt_open(void);

#endif