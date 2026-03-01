#include "z_sensor.h"
#include "z_delay.h"
#include "z_gpio.h"
#include "z_usart.h"

// 外部变量声明
extern u8 group_do_ok;

void setup_sensor(void) {
    // 传感器引脚已在 CubeMX 中初始化
    return;
}

void AI_jiaqu(void) {
    if(HAL_GetTick() < 3000) return; // 系统启动3秒后检测
    if(group_do_ok == 0) return; // 动作执行中不触发
    if(AI_Read() == 0) { // 检测PB0低电平
        HAL_Delay(20); // 防抖
        if(AI_Read() == 0) {
            // 执行动作组1-9
            zx_uart_send_str((u8 *)"$DGT:1-9,1!");
            beep_on_times(1, 100); // 蜂鸣提示
        }
    }
}