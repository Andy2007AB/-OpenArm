#ifndef __Z_KINEMATICS_H__
#define __Z_KINEMATICS_H__

#include "z_type.h"

// 逆运动学结构体
typedef struct {
    float L0;        // 基座高度
    float L1;        // 大臂长度
    float L2;        // 小臂长度
    float L3;        // 末端长度
    float servo_angle[4]; // 关节角度
    int servo_pos[4];     // 舵机位置值
} kinematics_t;

// 函数声明
void setup_kinematics(float L0, float L1, float L2, float L3, kinematics_t *kinematics);
int kinematics_analysis(float x, float y, float z, float Alpha, kinematics_t *kinematics);

#endif