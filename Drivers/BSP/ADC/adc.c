/**
 ****************************************************************************************************
 * @file        adc.c
 * @version     V1.0
 * @brief       ADC ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */
 
#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       ADC��ʼ������
 *              ����ʹ��12λ�ֱ���, ADC����ʱ��=20M, ת��ʱ��Ϊ: ����ʱ�� + 12��ADC����
 *              ��������������: 480, ��ת��ʱ�� = 492��ADC���� = 24.6us
 * @param       ��
 * @retval      ��
 */
void adc_init(void)
{
    rcu_periph_clock_enable(ADC_ADCX_CHY_CLK);           /* ʹ��ADCXʱ�� */
    rcu_periph_clock_enable(ADC_ADCX_CHY_GPIO_CLK);      /* ʹ��ADC IO��ʱ�� */
    gpio_mode_set(ADC_ADCX_CHY_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_ADCX_CHY_GPIO_PIN);  /* ADC�ɼ�����ģʽ����,ģ������ */    

    adc_deinit();   /* ��λ���е�ADC���� */

    /* ADCʱ��ѡ������ APB2, ��PCLK2, Ƶ��Ϊ120Mhz, ADC���ʱ��Ƶ��Ϊ40Mhz
     * ��120M PCLK2������, ����ʹ��6��Ƶ, �õ�PCLK2 / 6 = 20Mhz ��ADCʱ��
     */    
    adc_clock_config(ADC_ADCCK_PCLK2_DIV6);                                  /* ��������ADCʱ��ΪPCLK2ʱ��6��Ƶ */
  
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);                         /* ADCͬ��ģʽʧ�ܣ�����ADC���������� */
          
    adc_special_function_config(ADC_ADCX, ADC_SCAN_MODE, DISABLE);           /* ��ɨ��ģʽ */	
  
    adc_special_function_config(ADC_ADCX, ADC_CONTINUOUS_MODE, DISABLE);     /* ������ģʽ */	 
 
    adc_resolution_config(ADC_ADCX, ADC_RESOLUTION_12B);                     /* 12λ�ֱ��� */
  
    adc_data_alignment_config(ADC_ADCX, ADC_DATAALIGN_RIGHT);                /* ���������Чλ����(�Ҷ���) */

    adc_external_trigger_config(ADC_ADCX, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);  /* ���������ⲿ����ʧ�� */
       
    adc_channel_length_config(ADC_ADCX, ADC_ROUTINE_CHANNEL, 1);             /* ���ó������г���Ϊ1��1��ת���ڳ���ͨ��ת�������� Ҳ����ֻת����������0 */

    adc_enable(ADC_ADCX);                                                    /* ʹ��ADCX */
    delay_ms(10);                                                            /* �ȴ�ADC�ȶ� */
    
    adc_calibration_enable(ADC_ADCX);                                        /* ADCXУ׼��λ */    
}

/**
 * @brief       ���ADCת����Ľ��
 * @param       channel: ͨ����� 0~18��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_18 
 * @retval      ת�����
 */
uint16_t adc_get_result(uint8_t channel)
{
    adc_routine_channel_config(ADC_ADCX, 0U, channel, ADC_SAMPLETIME_480);   /* ����ADCX����ͨ���飬����0��Ӧͨ������ʱ��Ϊ480��ʱ������ */

    adc_software_trigger_enable(ADC_ADCX, ADC_ROUTINE_CHANNEL);  /* �������������������ת�� */
 
    while (!adc_flag_get(ADC_ADCX, ADC_FLAG_EOC));               /* �ȴ���������ת������ */

    adc_flag_clear(ADC_ADCX, ADC_FLAG_EOC);                      /* �����������ת��������־λ */
 
    return (adc_routine_data_read(ADC_ADCX));                    /* ����ADCX����ͨ����ת����� */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��
 * @param       channel: ͨ����� 0~18��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_18 
 * @param       times  : ��ȡ����
 * @retval      ͨ��channel��times��ת�����ƽ��ֵ
 */
uint32_t adc_get_result_average(uint8_t channel, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)             /* ��ȡtimes������ */
    {
        temp_val += adc_get_result(channel);
        delay_ms(5);
    }

    return temp_val / times;                /* ����ƽ��ֵ */
}









