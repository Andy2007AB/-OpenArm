#include "z_ps2.h"
#include "z_delay.h"

void psx_io_config(void) {
    // 引脚已在 CubeMX 中初始化
    // 禁用 JTAG，保留 SWD
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
}

void PSX_init(void) {
    psx_io_config();
    PS2_ATT(1);
    PS2_CMD(1);
    PS2_CLK(1);
}

unsigned char psx_transfer(unsigned char dat) {
    unsigned char rd_data ,wt_data, i;
    wt_data = dat;
    rd_data = 0;
    for(i = 0;i < 8;i++){
        PS2_CMD((wt_data & (0x01 << i)));
        tb_delay_us(1);
        PS2_CLK(1);
        tb_delay_us(10);
        PS2_CLK(0);
        tb_delay_us(10);
        PS2_CLK(1);
        if(PS2_DAT) {
            rd_data |= 0x01<<i;
        }
    }
    return rd_data;
}

void psx_write_read(unsigned char *get_buf) {
    PS2_ATT(0);
    get_buf[0] = psx_transfer(START_CMD);
    get_buf[1] = psx_transfer(ASK_DAT_CMD);
    get_buf[2] = psx_transfer(get_buf[0]);
    get_buf[3] = psx_transfer(get_buf[0]);
    get_buf[4] = psx_transfer(get_buf[0]);
    get_buf[5] = psx_transfer(get_buf[0]);
    get_buf[6] = psx_transfer(get_buf[0]);
    get_buf[7] = psx_transfer(get_buf[0]);
    get_buf[8] = psx_transfer(get_buf[0]);
    PS2_ATT(1);
    
    return;
}