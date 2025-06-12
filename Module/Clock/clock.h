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
    uint8_t hour;      // 小时
    uint8_t minute;    // 分钟
    uint8_t second;    // 秒
    uint8_t weekday;   // 星期
} AlarmData;

void clock_handle_key(uint8_t key);
void clock_init();
void clock_runing();
void clock_exit();

// 新增：保存闹钟数据到Flash
FLS_Status clock_save_alarm(void);

// 新增：从Flash加载闹钟数据
FLS_Status clock_load_alarm(void);

#endif /* __CLOCK_H__ */