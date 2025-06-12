#include "./uflash.h"

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include <string.h>
#include "stdbool.h"

// ──────────────────────────────────────────────────────────────────────────────
// VAR & CONST DEF
// ──────────────────────────────────────────────────────────────────────────────

// Flash 状态机当前状态
static FLS_State currentState = FLS_STATE_IDLE;

// Flash 存储区域定义
#define FLS_START_ADDR    0x00000000  // Flash 起始地址
#define FLS_END_ADDR      0x000FFFFF  // Flash 结束地址
#define FLS_SECTOR_SIZE   0x1000      // 扇区大小 (4KB)
#define FLS_PAGE_SIZE     0x100       // 页大小 (256B)

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: STATIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

/**
 * @brief 检查地址是否在有效范围内
 * @param addr 起始地址
 * @param len 数据长度
 * @return FLS_Status 状态码
 */
static FLS_Status fls_check_addr(uint32_t addr, uint16_t len) {
    if (addr > FLS_END_ADDR || 
        (addr + len) > FLS_END_ADDR || 
        (addr % FLS_PAGE_SIZE) + len > FLS_PAGE_SIZE) {
        return FLS_ERR_ADDR;
    }
    return FLS_OK;
}

/**
 * @brief 等待操作完成
 * @param timeout 超时时间(ms)
 * @return FLS_Status 状态码
 */
static FLS_Status fls_wait_ready(uint32_t timeout) {
    uint32_t startTime = 0;
    while ((norflash_read_sr(1) & 0x01) == 0x01) {  // 检查 BUSY 位
        if (startTime++ > timeout) {
            return FLS_ERR_BUSY;
        }
        delay_ms(1);
    }
    return FLS_OK;
}

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: EXTERNAL FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

FLS_Status fls_init(void) {
    norflash_init();

    // 等待设备就绪并验证初始化是否成功
    printf("[FLS] Waiting for device ready...\r\n");
    if (fls_wait_ready(1000) != FLS_OK) {
        printf("[FLS] Error: Device not ready after timeout\r\n");
        currentState = FLS_STATE_ERROR;
        return FLS_ERR_INIT;
    }
    printf("[FLS] Device is ready\r\n");
    
    // 尝试读取一个字节来验证初始化是否成功
    uint8_t test_buf[10];
    uint32_t test_addr = 0x1000;  // 使用一个更靠后的地址
    printf("[FLS] Reading test bytes from address 0x%08X\r\n", test_addr);
    norflash_read(test_buf, test_addr, 10);
    
    // 打印所有读取的字节
    printf("[FLS] Test bytes: ");
    for(int i = 0; i < 10; i++) {
        printf("0x%02X ", test_buf[i]);
    }
    printf("\r\n");
    
    // 检查是否所有字节都是 0xFF
    bool all_ff = true;
    for(int i = 0; i < 10; i++) {
        if(test_buf[i] != 0xFF) {
            all_ff = false;
            break;
        }
    }
    
    if (all_ff) {
        printf("[FLS] Initialization successful\r\n");
        currentState = FLS_STATE_IDLE;
        return FLS_OK;
    } else {
        printf("[FLS] Error: Test bytes are not all 0xFF\r\n");
        currentState = FLS_STATE_ERROR;
        return FLS_ERR_INIT;
    }
}

FLS_Status fls_write(uint32_t addr, uint8_t *data, uint16_t len) {
    // 参数检查
    if (!data || len == 0) {
        return FLS_ERR_ADDR;
    }
    
    // 地址检查
    FLS_Status status = fls_check_addr(addr, len);
    if (status != FLS_OK) {
        return status;
    }
    
    // 状态检查
    if (currentState != FLS_STATE_IDLE) {
        return FLS_ERR_BUSY;
    }
    
    currentState = FLS_STATE_WRITING;
    
    // 写入数据
    norflash_write(data, addr, len);
    
    // 等待写入完成
    status = fls_wait_ready(1000);
    if (status != FLS_OK) {
        currentState = FLS_STATE_ERROR;
        return status;
    }
    
    currentState = FLS_STATE_IDLE;
    return FLS_OK;
}

FLS_Status fls_read(uint32_t addr, uint8_t *data, uint16_t len) {
    // 参数检查
    if (!data || len == 0) {
        return FLS_ERR_ADDR;
    }
    
    // 地址检查
    FLS_Status status = fls_check_addr(addr, len);
    if (status != FLS_OK) {
        return status;
    }
    
    // 状态检查
    if (currentState != FLS_STATE_IDLE) {
        return FLS_ERR_BUSY;
    }
    
    currentState = FLS_STATE_READING;
    
    // 读取数据
    norflash_read(data, addr, len);
    
    currentState = FLS_STATE_IDLE;
    return FLS_OK;
}

FLS_Status fls_erase_sector(uint32_t addr) {
    // 地址检查
    if (addr > FLS_END_ADDR || (addr % FLS_SECTOR_SIZE) != 0) {
        return FLS_ERR_ADDR;
    }
    
    // 状态检查
    if (currentState != FLS_STATE_IDLE) {
        return FLS_ERR_BUSY;
    }
    
    currentState = FLS_STATE_ERASING;
    
    // 擦除扇区
    norflash_erase_sector(addr / FLS_SECTOR_SIZE);  // 转换为扇区号
    
    // 等待擦除完成
    FLS_Status status = fls_wait_ready(2000);
    if (status != FLS_OK) {
        currentState = FLS_STATE_ERROR;
        return status;
    }
    
    currentState = FLS_STATE_IDLE;
    return FLS_OK;
}

FLS_State fls_get_state(void) {
    return currentState;
} 