/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "z_gpio.h"
#include "z_usart.h"
#include "z_ps2.h"
#include "z_sensor.h"
#include "z_action.h"
#include "z_kinematics.h"
#include "z_w25q64.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------

/* USER CODE BEGIN PV */
// 手柄数据缓冲区
unsigned char ps2_buf[9];
// 逆运动学结构体
kinematics_t kinematics;
// 手柄模式
unsigned char ps_mode = 0;
// 按键备份
static unsigned char psx_button_bak[2] = {0};
// 按键数量定义
#define PSX_BUTTON_NUM 16
// 按键命令配置
const char *pre_cmd_set_red[PSX_BUTTON_NUM] = {
    "<PS2_RED01:#005P0600T2000!^#005PDST!>",  // L2
    "<PS2_RED02:#005P2400T2000!^#005PDST!>",  // R2
    "<PS2_RED03:#004P0600T2000!^#004PDST!>",  // L1
    "<PS2_RED04:#004P2400T2000!^#004PDST!>",  // R1
    "<PS2_RED05:#002P2400T2000!^#002PDST!>",  // RU
    "<PS2_RED06:#003P2400T2000!^#003PDST!>",  // RR
    "<PS2_RED07:#002P0600T2000!^#002PDST!>",  // RD
    "<PS2_RED08:#003P0600T2000!^#003PDST!>",  // RL
    "<PS2_RED09:$MODE!>",                      // SE
    "<PS2_RED10:>",                            // AL
    "<PS2_RED11:>",                            // AR
    "<PS2_RED12:$DJR!>",                      // ST
    "<PS2_RED13:#001P0600T2000!^#001PDST!>",  // LU
    "<PS2_RED14:#000P0600T2000!^#000PDST!>",  // LR
    "<PS2_RED15:#001P2400T2000!^#001PDST!>",  // LD
    "<PS2_RED16:#000P2400T2000!^#000PDST!>"   // LL
};
// AI模式按键命令配置
const char *ai_pre_cmd_set_red[PSX_BUTTON_NUM] = {
    "<PS2_RED01:#005P0600T2000!^#005PDST!>",  // L2
    "<PS2_RED02:#005P2400T2000!^#005PDST!>",  // R2
    "<PS2_RED03:#004P0600T2000!^#004PDST!>",  // L1
    "<PS2_RED04:#004P2400T2000!^#004PDST!>",  // R1
    "<PS2_RED05:{#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}>" ,  // RU 前放
    "<PS2_RED06:{#000P1800T1000!#001P1800T1000!#002P1800T1000!#003P1800T1000!#004P1800T1000!#005P1800T1000!}>" ,  // RR 右放
    "<PS2_RED07:{#000P1200T1000!#001P1200T1000!#002P1200T1000!#003P1200T1000!#004P1200T1000!#005P1200T1000!}>" ,  // RD 蜷缩
    "<PS2_RED08:{#000P1200T1000!#001P1200T1000!#002P1200T1000!#003P1200T1000!#004P1200T1000!#005P1200T1000!}>" ,  // RL 左放
    "<PS2_RED09:$MODE!>",                      // SE
    "<PS2_RED10:>",                            // AL
    "<PS2_RED11:>",                            // AR
    "<PS2_RED12:$DJR!>",                      // ST
    "<PS2_RED13:{#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}>" ,  // LU 前抓
    "<PS2_RED14:{#000P1800T1000!#001P1800T1000!#002P1800T1000!#003P1800T1000!#004P1800T1000!#005P1800T1000!}>" ,  // LR 右抓
    "<PS2_RED15:{#000P1200T1000!#001P1200T1000!#002P1200T1000!#003P1200T1000!#004P1200T1000!#005P1200T1000!}>" ,  // LD 蜷缩
    "<PS2_RED16:{#000P1200T1000!#001P1200T1000!#002P1200T1000!#003P1200T1000!#004P1200T1000!#005P1200T1000!}>"   // LL 左抓
};
// 本地缓冲区，用于处理手柄命令
u8 local_cmd_buf[UART_BUF_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void loop_ps2_button(void);
void parse_cmd(u8 *cmd);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  // 初始化GPIO
  tb_gpio_init();
  nled_init();
  beep_init();
  
  // 初始化串口
  tb_usart_init();
  
  // 初始化手柄
  PSX_init();
  
  // 初始化传感器
  setup_sensor();
  
  // 初始化逆运动学
  setup_kinematics(10.0, 15.0, 15.0, 5.0, &kinematics);
  
  // 初始化W25Q64
  SpiFlashInit();
  
  // 初始化舵机状态
  for(int i=0; i<DJ_NUM; i++) {
    duoji_doing[i].cur = 1500;
    duoji_doing[i].aim = 1500;
    duoji_doing[i].time = 1000;
    duoji_doing[i].inc = 0;
  }
  
  // 蜂鸣器提示
  beep_on_times(2, 100);
  nled_on();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 读取手柄数据
    psx_write_read(ps2_buf);
    
    // 处理手柄按键
    loop_ps2_button();
    
    // 处理串口命令
    if(uart1_get_ok) {
      if(uart1_mode == 1) {
        parse_cmd(uart1_receive_buf);
        parse_group_cmd(uart1_receive_buf);
      } else if(uart1_mode == 2) {
        parse_action(uart1_receive_buf);
      }
      uart1_get_ok = 0;
      uart1_mode = 0;
    }
    
    if(uart2_get_ok) {
      if(uart2_mode == 1) {
        parse_cmd(uart2_receive_buf);
        parse_group_cmd(uart2_receive_buf);
      } else if(uart2_mode == 2) {
        parse_action(uart2_receive_buf);
      }
      uart2_get_ok = 0;
      uart2_mode = 0;
    }
    
    if(uart3_get_ok) {
      if(uart3_mode == 1) {
        parse_cmd(uart3_receive_buf);
        parse_group_cmd(uart3_receive_buf);
      } else if(uart3_mode == 2) {
        parse_action(uart3_receive_buf);
      }
      uart3_get_ok = 0;
      uart3_mode = 0;
    }
    
    // 检测传感器信号
    AI_jiaqu();
    
    // 执行动作组指令
    loop_action();
    
    // 监控舵机状态（预留）
    
    // 延时
    HAL_Delay(50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// 字符串包含函数
int str_contain_str(u8 *str1, u8 *str2) {
    int i, j;
    for(i=0; str1[i] != 0; i++) {
        for(j=0; str2[j] != 0; j++) {
            if(str1[i+j] != str2[j]) break;
        }
        if(str2[j] == 0) return i+1;
    }
    return 0;
}

// 处理手柄按键
void parse_psx_buf(unsigned char *buf, unsigned char mode) {
    u8 i, pos = 0;
    static u16 bak=0xffff, temp, temp2;
    temp = (buf[0]<<8) + buf[1];
    
    if(bak != temp) {
        temp2 = temp;
        temp &= bak;
        for(i=0;i<16;i++) {
            if((1<<i) & temp) {
            } else {
                if((1<<i) & bak) { // 按键按下
                    memset(local_cmd_buf, 0, sizeof(local_cmd_buf));
                    if(mode == 0x41) { // PS2_LED_RED
                        if(ps_mode == 0) {
                            memcpy((char *)local_cmd_buf, (char *)pre_cmd_set_red[i], strlen(pre_cmd_set_red[i]));
                        } else if(ps_mode == 1) {
                            memcpy((char *)local_cmd_buf, (char *)ai_pre_cmd_set_red[i], strlen(ai_pre_cmd_set_red[i]));
                        }
                    }
                    
                    pos = str_contain_str(local_cmd_buf, (u8 *)"^");
                    if(pos) local_cmd_buf[pos-1] = '\0';
                    if(str_contain_str(local_cmd_buf, (u8 *)"$")) {
                        memcpy((char *)uart1_receive_buf, (char *)local_cmd_buf, strlen((char *)local_cmd_buf)+1);
                        uart1_get_ok = 1;
                        uart1_mode = 1;
                    } else if(str_contain_str(local_cmd_buf, (u8 *)"#") || str_contain_str(local_cmd_buf, (u8 *)"{")) {
                        memcpy((char *)uart1_receive_buf, (char *)local_cmd_buf, strlen((char *)local_cmd_buf)+1);
                        uart1_get_ok = 1;
                        uart1_mode = 2;
                    }
                    bak = 0xffff;
                } else { // 按键松开
                    memset(local_cmd_buf, 0, sizeof(local_cmd_buf));
                    if(mode == 0x41) { // PS2_LED_RED
                        if(ps_mode == 0) {
                            memcpy((char *)local_cmd_buf, (char *)pre_cmd_set_red[i], strlen(pre_cmd_set_red[i]));
                        } else if(ps_mode == 1) {
                            memcpy((char *)local_cmd_buf, (char *)ai_pre_cmd_set_red[i], strlen(ai_pre_cmd_set_red[i]));
                        }
                    }
                    
                    pos = str_contain_str(local_cmd_buf, (u8 *)"^");
                    if(pos) {
                        if(str_contain_str(local_cmd_buf+pos, (u8 *)"$")) {
                            strcpy((char *)local_cmd_buf, (char *)local_cmd_buf+pos);
                            local_cmd_buf[strlen((char *)local_cmd_buf) - 1] = '\0';
                            parse_cmd(local_cmd_buf);
                        } else if(str_contain_str(local_cmd_buf+pos, (u8 *)"#") || str_contain_str(local_cmd_buf+pos, (u8 *)"{")) {
                            strcpy((char *)local_cmd_buf, (char *)local_cmd_buf+pos);
                            local_cmd_buf[strlen((char *)local_cmd_buf) - 1] = '\0';
                            parse_action(local_cmd_buf);
                        }
                    }
                }
            }
        }
        bak = temp2;
        beep_on_times(1, 100);
    }
}

// 处理手柄按钮
void loop_ps2_button(void) {
    // 对比两次获取的按键值是否相同，相同就不处理，不相同则处理
    if((psx_button_bak[0] == ps2_buf[3]) && (psx_button_bak[1] == ps2_buf[4])) {
        return;
    } else {
        // 处理buf3和buf4两个字节，这两个字节存储这手柄16个按键的状态
        parse_psx_buf(ps2_buf+3, ps2_buf[1]);
        psx_button_bak[0] = ps2_buf[3];
        psx_button_bak[1] = ps2_buf[4];
    }
}

// 命令解析函数
void parse_cmd(u8 *cmd) {
    int pos;
    if(pos = str_contain_str(cmd, (u8 *)"$DJR!"), pos) {
        beep_on_times(1, 100);
        zx_uart_send_str((u8 *)"#255P1500T2000!\r\n");
    } else if(pos = str_contain_str(cmd, (u8 *)"$MODE!"), pos) {
        if(ps_mode == 0) {
            ps_mode = 1;
        } else if(ps_mode == 1) {
            ps_mode = 0;
        }
    } else if(pos = str_contain_str(cmd, (u8 *)"<G"), pos) {
        // 保存动作组
        save_action(cmd);
    }
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
