/**
 ****************************************************************************************************
 * @file        key.c
 * @version     V1.0
 * @brief       �������� ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */
 
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       ������ʼ������
 * @param       ��
 * @retval      ��
 */
void key_init(void)
{
    rcu_periph_clock_enable(KEY1_GPIO_CLK);     /* ʹ��KEY1ʱ�� */
		rcu_periph_clock_enable(KEY2_GPIO_CLK);     /* ʹ��KEY2ʱ�� */
		rcu_periph_clock_enable(KEY3_GPIO_CLK);     /* ʹ��KEY3ʱ�� */
		rcu_periph_clock_enable(KEY4_GPIO_CLK);     /* ʹ��KEY4ʱ�� */
		rcu_periph_clock_enable(KEY5_GPIO_CLK);     /* ʹ��KEY5ʱ�� */
		rcu_periph_clock_enable(KEY6_GPIO_CLK);     /* ʹ��KEY6ʱ�� */
	  rcu_periph_clock_enable(WKUP_GPIO_CLK);     /* ʹ��WKUPʱ�� */
    
    gpio_mode_set(KEY1_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY1_GPIO_PIN);   /* KEY1����ģʽ����,�������� */
		gpio_mode_set(KEY2_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY2_GPIO_PIN);   /* KEY2����ģʽ����,�������� */
		gpio_mode_set(KEY3_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY3_GPIO_PIN);   /* KEY3����ģʽ����,�������� */
		gpio_mode_set(KEY4_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY4_GPIO_PIN);   /* KEY4����ģʽ����,�������� */
		gpio_mode_set(KEY5_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY5_GPIO_PIN);   /* KEY5����ģʽ����,�������� */
		gpio_mode_set(KEY6_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY6_GPIO_PIN);   /* KEY6����ģʽ����,�������� */
	  gpio_mode_set(WKUP_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, WKUP_GPIO_PIN);   /* WKUP����ģʽ����,�������� */  	
}

/**
 * @brief       ����ɨ�躯��
 * @note        �ú�������Ӧ���ȼ�(ͬʱ���¶������): WK_UP > KEY6 > KEY5 > KEY4 > KEY3 > KEY2 > KEY1!!
 * @param       mode:0 / 1, ���庬������:
 *   @arg       0,  ��֧��������(���������²���ʱ, ֻ�е�һ�ε��û᷵�ؼ�ֵ,
 *                  �����ɿ��Ժ�, �ٴΰ��²Ż᷵��������ֵ)
 *   @arg       1,  ֧��������(���������²���ʱ, ÿ�ε��øú������᷵�ؼ�ֵ)
 * @retval      ��ֵ, ��������:
 *              KEY1_PRES, 1, KEY1����
 *              KEY2_PRES, 2, KEY2����
 *              KEY3_PRES, 3, KEY3����
 *              KEY4_PRES, 4, KEY4����
 *              KEY5_PRES, 5, KEY5����
 *              KEY6_PRES, 6, KEY6����
 *              WKUP_PRES, 7, WKUP����
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;  /* �������ɿ���־ */
    uint8_t keyval = 0;

    if (mode) key_up = 1;       /* ֧������ */
  
    if (key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0 || KEY5 == 0 || KEY6 == 0 || WK_UP == 0))     /* �����ɿ���־Ϊ1, ��������һ������������ */
    {
        delay_ms(10);           /* ȥ���� */
        key_up = 0;

        if (KEY1 == 0)  keyval = KEY1_PRES;
			  if (KEY2 == 0)  keyval = KEY2_PRES;
				if (KEY3 == 0)  keyval = KEY3_PRES;
				if (KEY4 == 0)  keyval = KEY4_PRES;
				if (KEY5 == 0)  keyval = KEY5_PRES;
				if (KEY6 == 0)  keyval = KEY6_PRES;
			  if (WK_UP == 0) keyval = WKUP_PRES;
    }
    else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 && KEY5 == 1 && KEY6 == 1 && WK_UP == 1)            /* û���κΰ�������, ��ǰ����ɿ� */
    {
        key_up = 1;
    }

    return keyval;              /* ���ؼ�ֵ */
}


