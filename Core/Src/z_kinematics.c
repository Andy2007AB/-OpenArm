#include "z_kinematics.h"
#include <math.h>

// 数学常量
#define pi 3.14159265358979323846

void setup_kinematics(float L0, float L1, float L2, float L3, kinematics_t *kinematics) {
    kinematics->L0 = L0;
    kinematics->L1 = L1;
    kinematics->L2 = L2;
    kinematics->L3 = L3;
}

int kinematics_analysis(float x, float y, float z, float Alpha, kinematics_t *kinematics) {
    float theta3, theta4, theta5, theta6;
    float l0, l1, l2, l3;
    float aaa, bbb, ccc, zf_flag;
    
    x = x*10; y = y*10; z = z*10; // 单位放大10倍
    l0 = kinematics->L0; l1 = kinematics->L1; l2 = kinematics->L2; l3 = kinematics->L3;
    
    theta6 = (x == 0) ? 0.0 : atan(x/y)*180.0/pi; // 基座旋转角
    y = sqrt(x*x + y*y) - l3 * cos(Alpha*pi/180.0); // 坐标变换
    z = z - l0 - l3*sin(Alpha*pi/180.0);
    
    ccc = acos(y / sqrt(y*y + z*z)); // 肩关节角度计算
    bbb = (y*y + z*z + l1*l1 - l2*l2)/(2*l1*sqrt(y*y + z*z));
    zf_flag = (z < 0) ? -1 : 1;
    theta5 = ccc * zf_flag + acos(bbb);
    theta5 *= 180.0 / pi;
    
    aaa = -(y*y + z*z - l1*l1 - l2*l2)/(2*l1*l2); // 肘关节角度计算
    theta4 = 180.0 - acos(aaa)*180.0/pi;
    
    theta3 = Alpha - theta5 + theta4; // 腕关节角度计算
    
    kinematics->servo_angle[0] = theta6; // 存储关节角度
    kinematics->servo_angle[1] = theta5 - 90;
    kinematics->servo_angle[2] = theta4;
    kinematics->servo_angle[3] = theta3;
    
    // 角度转位置值
    kinematics->servo_pos[0] = (int)(1500 - 2000.0 * theta6 / 270.0);
    kinematics->servo_pos[1] = (int)(1500 + 2000.0 * (theta5 - 90) / 270.0);
    kinematics->servo_pos[2] = (int)(1500 + 2000.0 * theta4 / 270.0);
    kinematics->servo_pos[3] = (int)(1500 + 2000.0 * theta3 / 270.0);
    return 0;
}