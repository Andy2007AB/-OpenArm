#include "z_w25q64.h"

// 声明hspi1，实际定义在spi.c中
extern SPI_HandleTypeDef hspi1;

/***********************************************
函数名称：SpiFlashInit() 
功能介绍：初始化SPI接口
函数参数：无
返回值：无
***********************************************/
void SpiFlashInit(void) {
    // SPI1已经在CubeMX中初始化
}

/***********************************************
函数名称：SpiWriteRead() 
功能介绍：SPI读写函数
函数参数：d - 要写入的数据
返回值：读取到的数据
***********************************************/
u8 SpiWriteRead(u8 d) {
    u8 receive = 0;
    HAL_SPI_TransmitReceive(&hspi1, &d, &receive, 1, 100);
    return receive;
}

/***********************************************
函数名称：SpiFlashReadID() 
功能介绍：读取FLASH ID
函数参数：无
返回值：FLASH ID
***********************************************/
u16 SpiFlashReadID(void) {
    u16 temp = 0;
    
    SPI_FLASH_CS(0);
    SpiWriteRead(W25X_JedecDeviceID);
    temp |= SpiWriteRead(0xFF) << 8;
    temp |= SpiWriteRead(0xFF);
    SPI_FLASH_CS(1);
    
    return temp;
}

/***********************************************
函数名称：SpiFlashWriteEnable() 
功能介绍：写使能
函数参数：无
返回值：无
***********************************************/
void SpiFlashWriteEnable(void) {
    SPI_FLASH_CS(0);
    SpiWriteRead(W25X_WriteEnable);
    SPI_FLASH_CS(1);
}

/***********************************************
函数名称：SpiFlashWriteDisable() 
功能介绍：写禁止
函数参数：无
返回值：无
***********************************************/
void SpiFlashWriteDisable(void) {
    SPI_FLASH_CS(0);
    SpiWriteRead(W25X_WriteDisable);
    SPI_FLASH_CS(1);
}

/***********************************************
函数名称：SpiFlashRead() 
功能介绍：读取flash
函数参数：pBuffer - 数据缓冲区，ReadAddr - 读取地址，NumByteToRead - 读取字节数
返回值：无
***********************************************/
void SpiFlashRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead) {
    SPI_FLASH_CS(0);
    SpiWriteRead(W25X_ReadData);
    SpiWriteRead((ReadAddr >> 16) & 0xFF);
    SpiWriteRead((ReadAddr >> 8) & 0xFF);
    SpiWriteRead(ReadAddr & 0xFF);
    
    while(NumByteToRead--) {
        *pBuffer++ = SpiWriteRead(0xFF);
    }
    
    SPI_FLASH_CS(1);
}

/***********************************************
函数名称：SpiFlashWrite() 
功能介绍：写入flash
函数参数：pBuffer - 数据缓冲区，WriteAddr - 写入地址，NumByteToWrite - 写入字节数
返回值：无
***********************************************/
void SpiFlashWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite) {
    u16 pageRemain = 256 - (WriteAddr % 256);
    
    if(NumByteToWrite <= pageRemain) pageRemain = NumByteToWrite;
    
    while(1) {
        SpiFlashWriteEnable();
        
        SPI_FLASH_CS(0);
        SpiWriteRead(W25X_PageProgram);
        SpiWriteRead((WriteAddr >> 16) & 0xFF);
        SpiWriteRead((WriteAddr >> 8) & 0xFF);
        SpiWriteRead(WriteAddr & 0xFF);
        
        for(u16 i=0; i<pageRemain; i++) {
            SpiWriteRead(pBuffer[i]);
        }
        SPI_FLASH_CS(1);
        
        SpiFlashWaitBusy();
        
        if(NumByteToWrite == pageRemain) break;
        else {
            pBuffer += pageRemain;
            WriteAddr += pageRemain;
            NumByteToWrite -= pageRemain;
            
            if(NumByteToWrite > 256) pageRemain = 256;
            else pageRemain = NumByteToWrite;
        }
    }
}

/***********************************************
函数名称：SpiFlashWriteS() 
功能介绍：写入flash（用于写入结构体）
函数参数：pBuffer - 数据缓冲区，WriteAddr - 写入地址，NumByteToWrite - 写入字节数
返回值：无
***********************************************/
void SpiFlashWriteS(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite) {
    SpiFlashWrite(pBuffer, WriteAddr, NumByteToWrite);
}

/***********************************************
函数名称：SpiFlashEraseSector() 
功能介绍：扇区擦除
函数参数：Dst_Addr - 扇区地址
返回值：无
***********************************************/
void SpiFlashEraseSector(u32 Dst_Addr) {
    Dst_Addr &= 0xFFFFFF00;
    
    SpiFlashWriteEnable();
    SpiFlashWaitBusy();
    
    SPI_FLASH_CS(0);
    SpiWriteRead(W25X_SectorErase);
    SpiWriteRead((Dst_Addr >> 16) & 0xFF);
    SpiWriteRead((Dst_Addr >> 8) & 0xFF);
    SpiWriteRead(Dst_Addr & 0xFF);
    SPI_FLASH_CS(1);
    
    SpiFlashWaitBusy();
}

/***********************************************
函数名称：SpiFlashWaitBusy() 
功能介绍：等待空闲
函数参数：无
返回值：无
***********************************************/
void SpiFlashWaitBusy(void) {
    while((SpiWriteRead(W25X_ReadStatusReg) & 0x01) == 0x01);
}

/***********************************************
函数名称：spiFlahsOn() 
功能介绍：SPI Flash使能
函数参数：x - 使能状态
返回值：无
***********************************************/
void spiFlahsOn(u8 x) {
    if(x) {
        // 使能SPI Flash
    } else {
        // 禁用SPI Flash
    }
}