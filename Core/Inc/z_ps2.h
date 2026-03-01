#ifndef __Z_PS2_H__
#define __Z_PS2_H__

#include "z_type.h"
#include "main.h"

// 手柄引脚定义
#define PS2_CLK_PIN GPIO_PIN_12
#define PS2_CLK_PORT GPIOA

#define PS2_ATT_PIN GPIO_PIN_13
#define PS2_ATT_PORT GPIOA

#define PS2_CMD_PIN GPIO_PIN_14
#define PS2_CMD_PORT GPIOA

#define PS2_DAT_PIN GPIO_PIN_15
#define PS2_DAT_PORT GPIOA

// 命令定义
#define START_CMD 0x01
#define ASK_DAT_CMD 0x42

// 宏定义
#define PS2_CLK(x) HAL_GPIO_WritePin(PS2_CLK_PORT, PS2_CLK_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define PS2_ATT(x) HAL_GPIO_WritePin(PS2_ATT_PORT, PS2_ATT_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define PS2_CMD(x) HAL_GPIO_WritePin(PS2_CMD_PORT, PS2_CMD_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define PS2_DAT HAL_GPIO_ReadPin(PS2_DAT_PORT, PS2_DAT_PIN)

// 函数声明
void psx_io_config(void);
void PSX_init(void);
unsigned char psx_transfer(unsigned char dat);
void psx_write_read(unsigned char *get_buf);

#endif