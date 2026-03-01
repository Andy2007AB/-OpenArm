#include "z_action.h"
#include "z_usart.h"
#include "main.h"
#include "z_w25q64.h"
#include <stdio.h>
#include <string.h>

// 函数声明
int str_contain_str(u8 *str1, u8 *str2);

// 外部变量声明
extern u8 uart1_receive_buf[UART_BUF_SIZE];

// 定义常量
#define ACTION_SIZE 512
#define W25Q64_SECTOR_SIZE 4096

// 全局变量
duoji_state duoji_doing[DJ_NUM];
u8 group_do_ok = 1;
u8 cmd_return[64];

// 动作组执行相关变量
int do_start_index; // 动作组执行 起始序号
int do_times; // 动作组执行 执行次数
int group_num_start; // 动作组执行 起始序号
int group_num_end; // 动作组执行 终止序号
int group_num_times; // 动作组执行 起始变量
int action_time = 0; // 动作组执行时间间隔

// 存储信息结构体
typedef struct {
    int dj_bias_pwm[DJ_NUM]; // 舵机偏差值
    u8 pre_cmd[512]; // 预设命令
} eeprom_info_t;

eeprom_info_t eeprom_info;
#define FLAG_VERIFY 0x55
#define PRE_CMD_SIZE 511

void set_servo(int index, int pwm, int time) {
    // 更新舵机状态
    duoji_doing[index].aim = pwm;
    duoji_doing[index].time = time;
    duoji_doing[index].inc = (duoji_doing[index].aim - duoji_doing[index].cur) / (duoji_doing[index].time/20.000);
    
    // 发送总线命令（移除PWM相关代码）
    sprintf((char *)cmd_return, "#%03dP%04dT%04d!\r\n", index, pwm, time);
    zx_uart_send_str(cmd_return);
}

void parse_action(u8 *uart_receive_buf) {
    u16 index, time, i = 0, step = 0;
    float pwm;
    
    // 解析单个舵机指令 #XXXPYYYYTZZZZ!
    while(uart_receive_buf[i]) {
        if(uart_receive_buf[i] == '#' && step == 1) {
            // 解析舵机编号
            index = 0;
            i++;
            while(uart_receive_buf[i] && uart_receive_buf[i] != 'P') {
                index = index*10 + uart_receive_buf[i]-'0';
                i++;
            }
            
            // 解析目标位置
            if(uart_receive_buf[i] == 'P') {
                i++;
                pwm = 0;
                while(uart_receive_buf[i] && uart_receive_buf[i] != 'T') {
                    pwm = pwm*10 + uart_receive_buf[i]-'0';
                    i++;
                }
                
                // 解析执行时间
                if(uart_receive_buf[i] == 'T') {
                    i++;
                    time = 0;
                    while(uart_receive_buf[i] && uart_receive_buf[i] != '!') {
                        time = time*10 + uart_receive_buf[i]-'0';
                        i++;
                    }
                    
                    // 控制舵机
                    if(index < DJ_NUM) {
                        set_servo(index, (int)pwm, time);
                    }
                }
            }
        } else if(uart_receive_buf[i] == '{' || uart_receive_buf[i] == '<') {
            step = 1;
            i++;
        } else if(uart_receive_buf[i] == '}' || uart_receive_buf[i] == '>') {
            step = 0;
            i++;
        } else {
            i++;
        }
    }
}

void do_group_once_from_buffer(u8 *group_buf, u8 len) {
    // 执行动作组
    group_do_ok = 0;
    
    // 解析并执行动作组命令
    parse_action(group_buf);
    
    // 模拟动作执行完成
    HAL_Delay(1000);
    group_do_ok = 1;
    
    // 避免未使用参数警告
    (void)len;
}

void loop_servo_monitor(void) {
    // 空函数，预留用于舵机状态监控
}

/***********************************************
函数名称：rewrite_eeprom() 
功能介绍：把eeprom_info写入到W25Q64_INFO_ADDR_SAVE_STR位置
函数参数：无
返回值：无
***********************************************/
void rewrite_eeprom(void) {
    spiFlahsOn(1);
    w25x_erase_sector(W25Q64_INFO_ADDR_SAVE_STR / W25Q64_SECTOR_SIZE);
    w25x_writeS((u8 *)(&eeprom_info), W25Q64_INFO_ADDR_SAVE_STR, sizeof(eeprom_info));
    spiFlahsOn(0);
}

/***********************************************
函数名称：get_action_index() 
功能介绍：获取动作组序号
函数参数：str - 动作组字符串
返回值：动作组序号
***********************************************/
int get_action_index(u8 *str) {
    int index = 0;
    while(*str) {
        if(*str == 'G') {
            str++;
            while((*str != '#') && (*str != '$')) {
                index = index*10 + *str-'0';
                str++;
            }
            return index;
        } else {
            str++;
        }
    }
    return -1;
}

/***********************************************
函数名称：replace_char() 
功能介绍：字符串中的字符替代函数
函数参数：str - 字符串，ch1 - 要替换的字符，ch2 - 替换后的字符
返回值：无
***********************************************/
void replace_char(u8* str, u8 ch1, u8 ch2) {
    while(*str) {
        if(*str == ch1) {
            *str = ch2;
        }
        str++;
    }
    return;
}

/***********************************************
函数名称：getMaxTime() 
功能介绍：获取最大时间
函数参数：str - 动作组字符串
返回值：最大时间
***********************************************/
int getMaxTime(u8 *str) {
    int i = 0, max_time = 0, tmp_time = 0;
    while(str[i]) {
        if(str[i] == 'T') {
            tmp_time = (str[i+1]-'0')*1000 + (str[i+2]-'0')*100 + (str[i+3]-'0')*10 + (str[i+4]-'0');
            if(tmp_time > max_time) max_time = tmp_time;
            i = i+4;
            continue;
        }
        i++;
    }
    return max_time;
}

/***********************************************
函数名称：do_group_once() 
功能介绍：从存储芯片中读取第group_num个动作组，执行动作组
函数参数：group_num - 动作组序号
返回值：无
***********************************************/
void do_group_once(int group_num) {
    spiFlahsOn(1);
    // 将uart1_receive_buf清零
    memset(uart1_receive_buf, 0, sizeof(uart1_receive_buf));
    // 从存储芯片中读取第group_num个动作组
    w25x_read(uart1_receive_buf, group_num*ACTION_SIZE, ACTION_SIZE-1);
    // 获取最大的组时间
    action_time = getMaxTime(uart1_receive_buf);
    
    // 把读取出来的动作组传递到parse_action执行
    parse_action(uart1_receive_buf);
    spiFlahsOn(0);
}

/***********************************************
函数名称：parse_group_cmd() 
功能介绍：解析 $ 开头 ! 结尾的指令，处理动作组相关指令
函数参数：cmd - 命令字符串
返回值：无
***********************************************/
void parse_group_cmd(u8 *cmd) {
    int pos, i, index;
    
    if(pos = str_contain_str(cmd, (u8 *)"$DST!"), pos) {
        group_do_ok = 1;
        for(i=0; i<DJ_NUM; i++) {
            duoji_doing[i].inc = 0;
            duoji_doing[i].aim = duoji_doing[i].cur;
        }
        zx_uart_send_str((u8 *)"#255PDST!"); // 总线停止
    } else if(pos = str_contain_str(cmd, (u8 *)"$DST:"), pos) {
        if(sscanf((char *)cmd, "$DST:%d!", &index)) {
            duoji_doing[index].inc = 0;
            duoji_doing[index].aim = duoji_doing[index].cur;
            sprintf((char *)cmd_return, "#%03dPDST!\r\n", (int)index);
            zx_uart_send_str(cmd_return);
            memset(cmd_return, 0, sizeof(cmd_return));
        }
    } else if(pos = str_contain_str(cmd, (u8 *)"$DGT:"), pos) {
        if(sscanf((char *)cmd, "$DGT:%d-%d,%d!", &group_num_start, &group_num_end, &group_num_times)) {
            if(group_num_start != group_num_end) {
                do_start_index = group_num_start;
                do_times = group_num_times;
                group_do_ok = 0;
            } else {
                do_group_once(group_num_start);
            }
        }
    }
}

/***********************************************
函数名称：save_action() 
功能介绍：动作组保存函数，设置开机动作组
函数参数：str - 动作组字符串
返回值：无
***********************************************/
void save_action(u8 *str) {
    int action_index = 0;
    // 预存命令处理
    spiFlahsOn(1);
    if(str[1] == '$' && str[2] == '!') {
        eeprom_info.pre_cmd[PRE_CMD_SIZE] = 0;
        rewrite_eeprom(); // 去掉开机动作组
        uart1_send_str((u8 *)"@CLEAR PRE_CMD OK!");
        return;
    } else if(str[1] == '$') {
        // 设置开机动作组
        if(sscanf((char *)str, "<$DGT:%d-%d,%d!>", &group_num_start, &group_num_end, &group_num_times)) {
            if(group_num_start == group_num_end) {
                w25x_read(eeprom_info.pre_cmd, group_num_start*ACTION_SIZE, ACTION_SIZE);
            } else {
                memset(eeprom_info.pre_cmd, 0, sizeof(eeprom_info.pre_cmd));
                strcpy((char *)eeprom_info.pre_cmd, (char *)str+1);
                eeprom_info.pre_cmd[strlen((char *)str) - 2] = '\0';
            }
            eeprom_info.pre_cmd[PRE_CMD_SIZE] = FLAG_VERIFY;
            rewrite_eeprom();
            uart1_send_str((u8 *)"@SET PRE_CMD OK!");
        }
        return;
    }
    
    action_index = get_action_index(str);
    // <G0001#001...>
    if((action_index == -1) || str[6] != '#') {
        uart1_send_str((u8 *)"E");
        return;
    }
    // 把尖括号替换成大括号直接存储到存储芯片里面去，则在执行动作组的时候直接拿出来解析就可以了
    replace_char(str, '<', '{');
    replace_char(str, '>', '}');
    
    if(action_index*ACTION_SIZE % W25Q64_SECTOR_SIZE == 0) {
        w25x_erase_sector(action_index*ACTION_SIZE/W25Q64_SECTOR_SIZE);
    }
    
    w25x_write(str, action_index*ACTION_SIZE, strlen((char *)str) + 1);
    uart1_send_str((u8 *)"A");
    spiFlahsOn(0);
    return;
}

/***********************************************
函数名称：loop_action() 
功能介绍：执行动作组指令
函数参数：无
返回值：无
***********************************************/
void loop_action(void) {
    static u32 systick_ms_bak = 0;
    if(group_do_ok == 0) {
        if(HAL_GetTick() - systick_ms_bak > (unsigned int)action_time) {
            systick_ms_bak = HAL_GetTick();
            if(group_num_times != 0 && do_times == 0) {
                group_do_ok = 1;
                uart1_send_str((u8 *)"@GroupDone!");
                return;
            }
            // 调用do_start_index个动作
            do_group_once(do_start_index);
            
            if(group_num_start < group_num_end) {
                if(do_start_index == group_num_end) {
                    do_start_index = group_num_start;
                    if(group_num_times != 0) {
                        do_times--;
                    }
                    return;
                }
                do_start_index++;
            } else {
                if(do_start_index == group_num_end) {
                    do_start_index = group_num_start;
                    if(group_num_times != 0) {
                        do_times--;
                    }
                    return;
                }
                do_start_index--;
            }
        }
    } else {
        action_time = 10;
    }
}