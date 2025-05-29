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
#include "../Module/OLED_Menu/menu.h"
#include "../Module/Clock/clock.h"

// Other
#include <string.h>

/************************************ IMPORT END ****************************************/
/********************************* GLOBALVAR START **************************************/
uint8_t key = 0;

// Menu
uint8_t currState = 0;         // ��ǰ��״̬ 0: �˵��� 1: ???
uint8_t selectAppIndex = 0;

/********************************* GLOBALVAR END ***************************************/




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
    rtc_set_wakeup(WAKEUP_CKSPRE, 0); /* ����WAKE UP�ж�,1�����ж�һ�� */

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


