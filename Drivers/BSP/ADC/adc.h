/**
 ****************************************************************************************************
 * @file        adc.h
 * @version     V1.0
 * @brief       ADC ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */

#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ADC������ ���� */

#define ADC_ADCX_CHY_GPIO_PORT              GPIOC
#define ADC_ADCX_CHY_GPIO_PIN               GPIO_PIN_0
#define ADC_ADCX_CHY_GPIO_CLK               RCU_GPIOC       /* GPIOCʱ��ʹ�� */

#define ADC_ADCX                            ADC0
#define ADC_ADCX_CHY                        ADC_CHANNEL_10   /* ADC_CHANNEL_x(x=0..18) */        
#define ADC_ADCX_CHY_CLK                    RCU_ADC0        /* ADC0 ʱ��ʹ�� */

/******************************************************************************************/

void adc_init(void);                                             /* ADC��ʼ�� */
uint16_t adc_get_result(uint8_t channel);                        /* ���ĳ��ͨ����ADCת����� */
uint32_t adc_get_result_average(uint8_t channel, uint8_t times); /* �õ�ĳ��ͨ����������������ƽ��ֵ */

#endif 















