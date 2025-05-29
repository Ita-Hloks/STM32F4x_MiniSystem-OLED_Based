/*************************************** README ******************************************
 * 
 * 小型菜单的实现 框架
 * KEY1 : 确认
 * KEY2 : 上翻
 * KEY3 : 下翻 
 *
 * KEY6 : 退出
 *
 */
/************************************ IMPORT START **************************************/
// Bank
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/ADC/adc.h"
#include "./USMART/usmart.h"
#include "./BSP/TIMER/timer.h"

// Module 
#include "../Module/OLED_Menu/menu.h"
#include "../Module/Clock/clock.h"

// Other
#include <string.h>

/************************************ IMPORT END ****************************************/
/********************************* GLOBALVAR START **************************************/
uint8_t key = 0;

// Menu
uint8_t currState = 0;         // 当前的状态 0: 菜单栏 1: ???
uint8_t selectAppIndex = 0;

/********************************* GLOBALVAR END ***************************************/




int main(void)
{
    // Init
    delay_init(240);    /* 延时初始化 */
    usart_init(115200); /* 初始化USART */
    usmart_init(120);
    led_init();         /* 初始化LED */
    oled_init();
    key_init();
    rtc_config();
    rtc_set_wakeup(WAKEUP_CKSPRE, 0); /* 配置WAKE UP中断,1秒钟中断一次 */

    // Moudle_Init
    menu_init();
    clock_init();

    while (1)
    {
        key = key_scan(1);

        menu_exit(key);
        if (currState == 0)
        { // Menu
            menu_switch_key(key);
        } else if (currState == 1)
        { // App
            menu_judgeapp_handlekey(selectAppIndex, key);
        }
        delay_ms(100);
    }
}


