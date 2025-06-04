#ifndef __U_MENU_H__
#define __U_MENU_H__

#include "../Drivers/BSP/OLED/oled.h"
#include "./BSP/RTC/rtc.h"


#include <string.h>

void menu_init();
void menu_main_running(uint8_t key);

#endif /* __U_MENU_H__ */