/*************************************** README ******************************************
 * 
 * С�Ͳ˵���ʵ�� ���
 * KEY1 : ȷ��
 * KEY2 : �Ϸ�
 * KEY3 : �·� 
 *
 * KEY6 : �˳�
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
#include "../Module/Menu/menu.h"
#include "../Module/Clock/clock.h"
#include "../Module/StopWatch/stopwatch.h"

// Other
#include <string.h>

/************************************ IMPORT END ****************************************/

uint8_t key = 0;

int main(void)
{
    // Init
    delay_init(240);    /* ��ʱ��ʼ�� */
    usart_init(115200); /* ��ʼ��USART */
    usmart_init(120);
    led_init();         /* ��ʼ��LED */
    oled_init();
    key_init();
    rtc_config();
    adc_init();
    
    rtc_set_wakeup(WAKEUP_CKSPRE, 0); /* ����WAKE UP�ж�,1�����ж�һ�� */

    // Moudle_Init
    clock_init();
    st_init();

    // Menu Main
    menu_init();

    while (1)
    {
        key = key_scan(1);
        menu_main_running(key);
    }
}


