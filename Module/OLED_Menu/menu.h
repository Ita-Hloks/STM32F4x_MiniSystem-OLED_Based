#ifndef __U_MENU_H__
#define __U_MENU_H__

#include "../Drivers/BSP/OLED/oled.h"
#include "./BSP/RTC/rtc.h"

#include <string.h>

extern const char *menu[];
extern uint8_t menu_select_index;       // 应用程序选择的索引值
extern uint8_t menu_pointer_index; // 三角指针的位置，0在上， 1在下
extern uint8_t curr_state;

#define MENU_LEN (sizeof(menu) / sizeof(menu[0]))


void Menu_Exit(uint8_t);
void Menu_Init();
void Menu_LastApp(void);
void Menu_NextApp(void);
void Menu_JudgeApp_HandleKey(uint8_t select_index, uint8_t key);

#endif /* __U_MENU_H__ */