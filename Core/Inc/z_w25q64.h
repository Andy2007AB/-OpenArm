#ifndef __W25Q64_H__
#define __W25Q64_H__

#include "main.h"
#include "z_gpio.h"

/*******SPI快捷指令表*******/
#define SPI_FLASH_CS(x) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, (GPIO_PinState)x)

/*******W25Q系列芯片ID*******/
#define W25Q80 0XEF13 
#define W25Q16 0XEF14
#define W25Q32 0XEF15
#define W25Q64 0XEF16

/*******W25Q64芯片变量宏定义*******/
#define W25Q64_SECTOR_SIZE 4096 //4K
#define W25Q64_SECTOR_NUM 2048 //8*1024/4 = 2048

/*******W25Q64芯片地址存储表*******/
#define W25Q64_INFO_ADDR_SAVE_STR 0x7E000
#define ACTION_SIZE 512

/*******FLASH相关指令表*******/
#define W25X_WriteEnable 0x06 
#define W25X_WriteDisable 0x04 
#define W25X_ReadStatusReg 0x05 
#define W25X_WriteStatusReg 0x01 
#define W25X_ReadData 0x03 
#define W25X_FastReadData 0x0B 
#define W25X_FastReadDual 0x3B 
#define W25X_PageProgram 0x02 
#define W25X_SectorErase 0x20 
#define W25X_BlockErase 0xD8 
#define W25X_ChipErase 0xC7 
#define W25X_PowerDown 0xB9 
#define W25X_ReleasePowerDown 0xAB 
#define W25X_DeviceID 0xAB 
#define W25X_ManufactDeviceID 0x90 
#define W25X_JedecDeviceID 0x9F 

/*******FLASH相关函数声明*******/
void SpiFlashInit(void);
u8 SpiWriteRead(u8 d);
u16 SpiFlashReadID(void); //读取FLASH ID
void SpiFlashWriteEnable(void); //写使能 
void SpiFlashWriteDisable(void); //写禁止
void SpiFlashRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead); //读取flash
void SpiFlashWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite); //写入flash
void SpiFlashWriteS(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SpiFlashEraseSector(u32 Dst_Addr); //扇区擦除
void SpiFlashWaitBusy(void); //等待空闲
void spiFlahsOn(u8 x);

#define w25x_init() SpiFlashInit()
#define w25x_readId() SpiFlashReadID()
#define w25x_read(buf, addr, len) SpiFlashRead(buf, addr, len)
#define w25x_write(buf, addr, len) SpiFlashWrite(buf, addr, len)
#define w25x_writeS(buf, addr, len) SpiFlashWriteS(buf, addr, len)
#define w25x_erase_sector(addr) SpiFlashEraseSector(addr)
#define w25x_wait_busy() SpiFlashWaitBusy()

#endif