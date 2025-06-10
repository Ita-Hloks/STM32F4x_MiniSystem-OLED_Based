/**
 ****************************************************************************************************
 * @file        pmu.c
 * @version     V1.0
 * @brief       ����ģʽ ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6Сϵͳ��
 *
 ****************************************************************************************************
 */
 
#include "./BSP/PMU/pmu.h"
#include "./BSP/LED/led.h"


/**
 * @brief       �͹���ģʽ�µİ�����ʼ��(���ڻ��Ѵ���ģʽ)
 * @param       ��
 * @retval      ��
 */
void pmu_wkup_key_init(void)
{
    rcu_periph_clock_enable(PMU_WKUP_GPIO_CLK);     /* ʹ��WKUP����ʱ�� */
  
    gpio_mode_set(PMU_WKUP_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, PMU_WKUP_GPIO_PIN);  /* WKUP����ģʽ����,�������� */
}

/**
 * @brief       �������ģʽ
 * @param       ��
 * @retval      ��
 */
void pmu_enter_standby(void)
{
    rcu_periph_clock_enable(RCU_PMU);               /* ʹ��PMUʱ�� */     

    pmu_wakeup_pin_enable();                        /* WKUP���Ż���ʹ�� */
  
    pmu_flag_clear(PMU_FLAG_RESET_STANDBY);         /* ��λ������־ */
  
    pmu_to_standbymode();                           /* �������ģʽ */
}
















