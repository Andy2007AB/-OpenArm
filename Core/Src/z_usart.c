#include "z_usart.h"

// 全局变量
u8 uart1_receive_buf[UART_BUF_SIZE];
u8 uart1_mode = 0;
u8 uart1_get_ok = 0;
u8 uart2_receive_buf[UART_BUF_SIZE];
u8 uart2_mode = 0;
u8 uart2_get_ok = 0;
u8 uart3_receive_buf[UART_BUF_SIZE];
u8 uart3_mode = 0;
u8 uart3_get_ok = 0;

void tb_usart_init(void) {
    // 串口已经在 CubeMX 中初始化
    uart1_open();
    uart2_open();
    uart3_open();
    tb_interrupt_open();
    return;
}

// 从串口1发送一个字节
void tb_usart1_send_byte(u8 Data) {
    HAL_UART_Transmit(&huart1, &Data, 1, 100);
    return;
}

void tb_usart1_send_nbyte(u8 *Data, u16 size) {
    HAL_UART_Transmit(&huart1, Data, size, 100);
    return;
}

void tb_usart1_send_str(u8 *Data) {
    while(*Data) {
        HAL_UART_Transmit(&huart1, Data++, 1, 100);
    }
    return;
}

// 从串口2发送一个字节
void tb_usart2_send_byte(u8 Data) {
    HAL_UART_Transmit(&huart2, &Data, 1, 100);
    return;
}

void tb_usart2_send_nbyte(u8 *Data, u16 size) {
    HAL_UART_Transmit(&huart2, Data, size, 100);
    return;
}

void tb_usart2_send_str(u8 *Data) {
    while(*Data) {
        HAL_UART_Transmit(&huart2, Data++, 1, 100);
    }
    return;
}

// 从串口3发送一个字节
void tb_usart3_send_byte(u8 Data) {
    HAL_UART_Transmit(&huart3, &Data, 1, 100);
    return;
}

void tb_usart3_send_nbyte(u8 *Data, u16 size) {
    HAL_UART_Transmit(&huart3, Data, size, 100);
    return;
}

void tb_usart3_send_str(u8 *Data) {
    while(*Data) {
        HAL_UART_Transmit(&huart3, Data++, 1, 100);
    }
    return;
}

void uart1_send_str(u8 *str) {
    tb_usart1_send_str(str);
}

void uart1_send_nbyte(u8 *Data, u16 size) {
    tb_usart1_send_nbyte(Data, size);
}

void uart1_send_byte(u8 data) {
    tb_usart1_send_byte(data);
}

void uart2_send_str(u8 *str) {
    tb_usart2_send_str(str);
}

void uart2_send_nbyte(u8 *Data, u16 size) {
    tb_usart2_send_nbyte(Data, size);
}

void uart2_send_byte(u8 data) {
    tb_usart2_send_byte(data);
}

// 串口功能分配：
// USART1 - 与上位机通讯
// USART2 - 预留
// USART3 - 发送指令到舵机总线

void zx_uart_send_str(u8 *str) {
    // 只向USART3发送指令，避免总线混乱
    tb_usart3_send_str(str);
}

void uart3_send_str(u8 *str) {
    tb_usart3_send_str(str);
}

void uart3_send_nbyte(u8 *Data, u16 size) {
    tb_usart3_send_nbyte(Data, size);
}

void uart3_send_byte(u8 data) {
    tb_usart3_send_byte(data);
}

void uart1_open(void) {
    // 串口1已经在 CubeMX 中初始化
}

void uart2_open(void) {
    // 串口2已经在 CubeMX 中初始化
}

void uart3_open(void) {
    // 串口3已经在 CubeMX 中初始化
}

void tb_interrupt_open(void) {
    // 中断已经在 CubeMX 中配置
}