#ifndef __Z_ACTION_H__
#define __Z_ACTION_H__

#include "z_type.h"

// 舵机数量
#define DJ_NUM 6

// 舵机状态结构体
typedef struct {
    int cur;     // 当前位置
    int aim;     // 目标位置
    int time;    // 执行时间
    float inc;   // 增量
} duoji_state;

// 全局变量
extern duoji_state duoji_doing[DJ_NUM];
extern u8 group_do_ok;
extern u8 cmd_return[64];

// 函数声明
void set_servo(int index, int pwm, int time);
void parse_action(u8 *uart_receive_buf);
void loop_servo_monitor(void);
void parse_group_cmd(u8 *cmd);
void save_action(u8 *str);
void loop_action(void);

#endif