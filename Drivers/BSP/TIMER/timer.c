/**
 ****************************************************************************************************
 * @file        timer.c
 * @version     V1.0
 * @brief       ��ʱ���ж� ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6Сϵͳ��
 *
 ****************************************************************************************************
 */
 
#include "./BSP/TIMER/timer.h"
// #include "./BSP/LED/led.h"


/**
 * @brief       ��ʱ��TIMERX��ʱ�жϳ�ʼ������
 * @note
 *              ��ʱ����ʱ������APB1��APB2,���CK_APBx = CK_AHB��CK_APBx = CK_AHB/2��
 *              ��ʱ����ʱ�ӵ���CK_AHB��CK_TIMERx = CK_AHB��,����ʱ��ʱ�ӵ���APBʱ�ӵ�������CK_TIMERx = 2*CK_APBx��
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void    timerx_int_init(uint32_t arr, uint16_t psc)
{
    timer_parameter_struct timer_initpara;                      /* timer_initpara���ڴ�Ŷ�ʱ���Ĳ��� */

    /* ʹ�����趨ʱ��ʱ�� */ 
    rcu_periph_clock_enable(TIMERX_INT_CLK);                    /* ʹ��TIMERX��ʱ�� */

    /* ��λ��ʱ�� */
    timer_deinit(TIMERX_INT);                                   /* ��λTIMERX */
    timer_struct_para_init(&timer_initpara);                    /* ��ʼ��timer_initparaΪĬ��ֵ */

    /* ���ö�ʱ������ */
    timer_initpara.prescaler         = psc;                     /* ����Ԥ��Ƶֵ */
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;      /* ���ö���ģʽΪ���ض���ģʽ */
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;        /* �������ϼ���ģʽ */
    timer_initpara.period            = arr;                     /* �����Զ���װ��ֵ */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;        /* ����ʱ�ӷ�Ƶ���� */
    timer_initpara.repetitioncounter = 0;                       /* �����ظ�������ֵ */
    timer_init(TIMERX_INT, &timer_initpara);                    /* ���ݲ�����ʼ����ʱ�� */

    /* ʹ�ܶ�ʱ�������ж� */
    timer_interrupt_flag_clear(TIMERX_INT, TIMER_INT_FLAG_UP);  /* �����ʱ�������жϱ�־ */
  
    timer_interrupt_enable(TIMERX_INT, TIMER_INT_UP);           /* ʹ�ܶ�ʱ���ĸ����ж� */
    
    nvic_irq_enable(TIMERX_INT_IRQn, 1, 3);                     /* ����NVIC�������ȼ�����ռ���ȼ�1����Ӧ���ȼ�3 */
    
    timer_enable(TIMERX_INT);                                   /* ʹ�ܶ�ʱ��TIMERX */
}

void timer1_int_init(uint32_t arr, uint16_t psc)
{
    timer_parameter_struct timer_initpara;                      /* timer_initpara���ڴ�Ŷ�ʱ���Ĳ��� */

    /* ʹ�����趨ʱ��ʱ�� */ 
    rcu_periph_clock_enable(RCU_TIMER1);                    /* ʹ��TIMER1��ʱ�� */

    /* ��λ��ʱ�� */
    timer_deinit(TIMER1);                                   /* ��λTIMER1 */
    timer_struct_para_init(&timer_initpara);                    /* ��ʼ��timer_initparaΪĬ��ֵ */

    /* ���ö�ʱ������ */
    timer_initpara.prescaler         = psc;                     /* ����Ԥ��Ƶֵ */
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;      /* ���ö���ģʽΪ���ض���ģʽ */
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;        /* �������ϼ���ģʽ */
    timer_initpara.period            = arr;                     /* �����Զ���װ��ֵ(32λ) */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;        /* ����ʱ�ӷ�Ƶ���� */
    timer_initpara.repetitioncounter = 0;                       /* �����ظ�������ֵ */
    timer_init(TIMER1, &timer_initpara);                    /* ���ݲ�����ʼ����ʱ�� */

    /* ʹ�ܶ�ʱ�������ж� */
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);  /* �����ʱ�������жϱ�־ */
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);           /* ʹ�ܶ�ʱ���ĸ����ж� */
    nvic_irq_enable(TIMER1_IRQn, 1, 3);                     /* ����NVIC�������ȼ�����ռ���ȼ�1����Ӧ���ȼ�3 */
    
    timer_disable(TIMER1);  
}

// /**
//  * @brief       ��ʱ��TIMERX�жϷ�����
//  * @param       ��
//  * @retval      ��
//  */
// void TIMERX_INT_IRQHandler(void)
// {
//     if (timer_interrupt_flag_get(TIMERX_INT, TIMER_INT_FLAG_UP) == SET)     /* �ж϶�ʱ�������ж��Ƿ��� */
//     {
//         timer_interrupt_flag_clear(TIMERX_INT, TIMER_INT_FLAG_UP);          /* �����ʱ�������жϱ�־ */
//     }
// }














