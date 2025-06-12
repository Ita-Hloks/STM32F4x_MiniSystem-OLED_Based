#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "./SYSTEM/sys/sys.h"
#include "../AppFlash/uflash.h"

// 闹钟数据存储地址
#define ALARM_FLASH_ADDR  0x00000000  // 使用第一个扇区存储闹钟数据

// 魔数定义，用于验证数据有效性
#define CLOCK_MAGIC_NUMBER  0xAA55

// 闹钟数据结构
typedef struct {
    uint16_t magic;    // 魔数，用于验证数据有效性
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t weekday;
} AlarmData;

void clock_handle_key(uint8_t key);
void clock_init();
void clock_runing();
void clock_exit();

FLS_Status clock_save_alarm(void);
FLS_Status clock_load_alarm(void);

#endif /* __CLOCK_H__ */