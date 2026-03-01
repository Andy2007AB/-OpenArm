#include "z_gpio.h"

void tb_gpio_init(void) {
    // 启用 AFIO 时钟
    __HAL_RCC_AFIO_CLK_ENABLE();
    
    // 禁用 JTAG，保留 SWD
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
    
    return;
}

void nled_init() {
    // LED 引脚已在 CubeMX 中初始化
    return;
}

void nled_switch(void) {
    static u8 flag = 0;
    if(flag) {
        nled_on();
    } else {
        nled_off();
    }
    flag = ~flag;
}

void beep_init(void) {
    // 蜂鸣器引脚已在 CubeMX 中初始化
    return;
}

void beep_times(u32 time, u32 count) {
    unsigned int i;
    for(i=0;i<count;i++) {
        beep_on();
nled_on();
        HAL_Delay(time);
        beep_off();
nled_off();
        HAL_Delay(time);
    }
}

void gpioA_pin_set(unsigned char pin, unsigned char level) {
    if(level) {
        HAL_GPIO_WritePin(GPIOA, 1 << pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOA, 1 << pin, GPIO_PIN_RESET);
    }
}

void gpioB_pin_set(unsigned char pin, unsigned char level) {
    if(level) {
        HAL_GPIO_WritePin(GPIOB, 1 << pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOB, 1 << pin, GPIO_PIN_RESET);
    }
}

void gpioC_pin_set(unsigned char pin, unsigned char level) {
    if(level) {
        HAL_GPIO_WritePin(GPIOC, 1 << pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOC, 1 << pin, GPIO_PIN_RESET);
    }
}

void sensor_init(void) {
    // 传感器引脚已在 CubeMX 中初始化
    return;
}

void beep_on_times(int times, int delay) {
    int i;
    for(i=0;i<times;i++) {
        beep_on();
        HAL_Delay(delay);
        beep_off();
        HAL_Delay(delay);
    }
}