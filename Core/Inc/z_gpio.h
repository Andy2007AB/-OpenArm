#ifndef __Z_GPIO_H__
#define __Z_GPIO_H__

#include "z_type.h"
#include "main.h"

// 引脚定义
#define NLED_PIN GPIO_PIN_13
#define NLED_PORT GPIOB

#define BEEP_PIN GPIO_PIN_5
#define BEEP_PORT GPIOB

// 函数声明
void tb_gpio_init(void);
void nled_init(void);
void nled_switch(void);
void beep_init(void);
void beep_times(u32 time, u32 count);
void gpioA_pin_set(unsigned char pin, unsigned char level);
void gpioB_pin_set(unsigned char pin, unsigned char level);
void gpioC_pin_set(unsigned char pin, unsigned char level);
void sensor_init(void);
void beep_on_times(int times, int delay);

// 宏定义
#define nled_on() HAL_GPIO_WritePin(NLED_PORT, NLED_PIN, GPIO_PIN_SET)
#define nled_off() HAL_GPIO_WritePin(NLED_PORT, NLED_PIN, GPIO_PIN_RESET)
#define beep_on() HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_SET)
#define beep_off() HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_RESET)

#endif