#ifndef __U_MENU_H__
#define __U_MENU_H__

#include "../Drivers/BSP/OLED/oled.h"
#include "./BSP/RTC/rtc.h"

#include <string.h>

extern uint8_t selectAppIndex;       // 应用程序选择的索引值
extern uint8_t currState;

#define MENU_LEN (sizeof(menu) / sizeof(menu[0]))


void menu_exit(uint8_t key);
void menu_init();
void menu_judgeapp_handlekey(uint8_t select_index, uint8_t key);
void menu_switch_key(uint8_t key);

#endif /* __U_MENU_H__ */