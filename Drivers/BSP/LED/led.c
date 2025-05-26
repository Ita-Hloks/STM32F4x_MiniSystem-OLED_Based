/**
 ****************************************************************************************************
 * @file        led.c
 * @version     V1.0
 * @brief       LED ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */
 
#include "./BSP/LED/led.h"


/**
 * @brief       ��ʼ��LED���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void led_init(void)
{
    rcu_periph_clock_enable(LED1_GPIO_CLK);     /* ʹ��LED1ʱ�� */
    rcu_periph_clock_enable(LED2_GPIO_CLK);     /* ʹ��LED2ʱ�� */
	  rcu_periph_clock_enable(LED3_GPIO_CLK);     /* ʹ��LED3ʱ�� */
    rcu_periph_clock_enable(LED4_GPIO_CLK);     /* ʹ��LED4ʱ�� */
	  rcu_periph_clock_enable(LED5_GPIO_CLK);     /* ʹ��LED5ʱ�� */
    rcu_periph_clock_enable(LED6_GPIO_CLK);     /* ʹ��LED6ʱ�� */
    
    /* LED1����ģʽ���� */
    gpio_mode_set(LED1_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED1_GPIO_PIN);
    gpio_output_options_set(LED1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_GPIO_PIN);
  
    /* LED2����ģʽ���� */
    gpio_mode_set(LED2_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED2_GPIO_PIN);
    gpio_output_options_set(LED2_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED2_GPIO_PIN);
	
	  /* LED3����ģʽ���� */
    gpio_mode_set(LED3_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED3_GPIO_PIN);
    gpio_output_options_set(LED3_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED3_GPIO_PIN);
  
    /* LED4����ģʽ���� */
    gpio_mode_set(LED4_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED4_GPIO_PIN);
    gpio_output_options_set(LED4_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED4_GPIO_PIN);
	
	  /* LED5����ģʽ���� */
    gpio_mode_set(LED5_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED5_GPIO_PIN);
    gpio_output_options_set(LED5_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED5_GPIO_PIN);
  
    /* LED6����ģʽ���� */
    gpio_mode_set(LED6_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED6_GPIO_PIN);
    gpio_output_options_set(LED6_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED6_GPIO_PIN);
	  
	  LED1(0);                                    /* �ر� LED1 */ 
	  LED2(0);                                    /* �ر� LED2 */
		LED3(0);                                    /* �ر� LED3 */ 
	  LED4(0);                                    /* �ر� LED4 */
		LED5(0);                                    /* �ر� LED5 */ 
	  LED6(0);                                    /* �ر� LED6 */
}

