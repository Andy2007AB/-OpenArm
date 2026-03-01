#ifndef __Z_SENSOR_H__
#define __Z_SENSOR_H__

#include "z_type.h"
#include "main.h"

// 传感器引脚定义
#define SENSOR_PIN GPIO_PIN_0
#define SENSOR_PORT GPIOB

// 宏定义
#define AI_Read() HAL_GPIO_ReadPin(SENSOR_PORT, SENSOR_PIN)

// 函数声明
void setup_sensor(void);
void AI_jiaqu(void);

#endif