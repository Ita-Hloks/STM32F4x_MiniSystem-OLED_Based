/*************************************** README ******************************************
 * 
 * 小型菜单的实现 框架
 * KEY1 : 确认
 * KEY2 : 上翻
 * KEY3 : 下翻 
 *
 * KEY6 : 退出
 * KEY_WAKE_UP : 待机(会阻断烧录)
 * 
 */

// Bank
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/ADC/adc.h"
#include "./USMART/usmart.h"
#include "./BSP/TIMER/timer.h"
#include "./BSP/PMU/pmu.h"

// Module 
#include "../Module/Menu/menu.h"
#include "../Module/Clock/clock.h"
#include "../Module/StopWatch/stopwatch.h"

// Other
#include <string.h>

uint8_t key = 0;

int main(void)
{
    // Init
    delay_init(240);
    usart_init(115200);
    usmart_init(120);
    led_init();
    oled_init();
    key_init();
    rtc_config();
    adc_init();
    pmu_wkup_key_init();

    rtc_set_wakeup(WAKEUP_CKSPRE, 0); /* 配置WAKE UP中断,1秒钟中断一次 */

    // Moudle_Init
    clock_init();
    st_init();
    menu_init();

    while (1)
    {
        key = key_scan(1);
        menu_main_running(key);
        if (key == WKUP_PRES) {
            oled_clear();
            led_close_all();
            pmu_enter_standby();
        }
    }
}


