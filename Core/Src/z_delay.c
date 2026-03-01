#include "z_delay.h"
#include "main.h"

void tb_delay_ms(u32 ms) {
    HAL_Delay(ms);
}

void tb_delay_us(u32 us) {
    // 简单的微秒延时实现
    u32 i;
    for(i = 0; i < us * 8; i++) {
        __NOP();
    }
}

void mdelay(u32 ms) {
    HAL_Delay(ms);
}