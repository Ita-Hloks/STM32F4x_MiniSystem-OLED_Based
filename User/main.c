/*************************************** README ******************************************
 * 
 * С�Ͳ˵���ʵ�� ���
 * KEY1 : ȷ��
 * KEY2 : �Ϸ�
 * KEY3 : �·� 
 *
 * KEY6 : �˳�
 * KEY_WAKE_UP : ����(�������¼)
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

    rtc_set_wakeup(WAKEUP_CKSPRE, 0); /* ����WAKE UP�ж�,1�����ж�һ�� */

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


