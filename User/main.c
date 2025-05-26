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

// Module 
#include "../Module/OLED_Menu/menu.h"

// Other
#include <string.h>

/************************************ IMPORT END ****************************************/
/********************************* GLOBALVAR START **************************************/
// Menu
uint8_t key = 0;
uint8_t curr_state = 0;         // ��ǰ��״̬ 0: �˵��� 1: ???
uint8_t menu_select_index = 0;

// Clock
uint8_t clock_h;
uint8_t clock_m;
uint8_t clock_s;
uint8_t clock_ampm = 0;
uint8_t clock_getTime_count = 0;
uint8_t *cur_pos = NULL;
uint8_t cur_pos_index = NULL; /* 0: null 1: hour 2: minus 3: secound */

/********************************* GLOBALVAR END ***************************************/

void menu_switch_key(uint8_t key)
{
    switch (key)
    {
    case 1: // CONFIRM
        curr_state++;
        oled_clear();
        Menu_JudgeApp_HandleKey(menu_select_index, 0);
        break;
    case 2: // ADD
        Menu_LastApp();
        delay_ms(50);
        printf("[DBG] Pointer now points to: %s\r\n", menu[menu_select_index]);
        break;
    case 3: // SUB
        Menu_NextApp();
        delay_ms(50);
        printf("[DBG] Pointer now points to: %s\r\n", menu[menu_select_index]);
        break;
    }
}


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
    Menu_Init();

    while (1)
    {
        key = key_scan(1);

        // �˳�����ʼ�ռ��
        Menu_Exit(key);

        if (curr_state == 0)
        { // Menu
            menu_switch_key(key);
        } else if (curr_state == 1)
        { // App
            Menu_JudgeApp_HandleKey(menu_select_index, key);
        }
        delay_ms(100);
    }
}


