/**
 ****************************************************************************************************
 * @file        key.h
 * @version     V1.0
 * @brief       �������� ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */	

#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ���� ���� */

#define KEY1_GPIO_PORT                  GPIOE
#define KEY1_GPIO_PIN                   GPIO_PIN_15
#define KEY1_GPIO_CLK                   RCU_GPIOE     /* GPIOEʱ��ʹ�� */

#define KEY2_GPIO_PORT                  GPIOE
#define KEY2_GPIO_PIN                   GPIO_PIN_13
#define KEY2_GPIO_CLK                   RCU_GPIOE     /* GPIOEʱ��ʹ�� */

#define KEY3_GPIO_PORT                  GPIOE
#define KEY3_GPIO_PIN                   GPIO_PIN_11
#define KEY3_GPIO_CLK                   RCU_GPIOE     /* GPIOEʱ��ʹ�� */

#define KEY4_GPIO_PORT                  GPIOE
#define KEY4_GPIO_PIN                   GPIO_PIN_9
#define KEY4_GPIO_CLK                   RCU_GPIOE     /* GPIOEʱ��ʹ�� */

#define KEY5_GPIO_PORT                  GPIOE
#define KEY5_GPIO_PIN                   GPIO_PIN_7
#define KEY5_GPIO_CLK                   RCU_GPIOE     /* GPIOEʱ��ʹ�� */

#define KEY6_GPIO_PORT                  GPIOB
#define KEY6_GPIO_PIN                   GPIO_PIN_0
#define KEY6_GPIO_CLK                   RCU_GPIOB     /* GPIOBʱ��ʹ�� */

#define WKUP_GPIO_PORT                  GPIOA
#define WKUP_GPIO_PIN                   GPIO_PIN_0
#define WKUP_GPIO_CLK                   RCU_GPIOA     /* GPIOAʱ��ʹ�� */

#define KEY1        gpio_input_bit_get(KEY1_GPIO_PORT, KEY1_GPIO_PIN)   /* ��ȡKEY1���� */
#define KEY2        gpio_input_bit_get(KEY2_GPIO_PORT, KEY2_GPIO_PIN)   /* ��ȡKEY2���� */
#define KEY3        gpio_input_bit_get(KEY3_GPIO_PORT, KEY3_GPIO_PIN)   /* ��ȡKEY3���� */
#define KEY4        gpio_input_bit_get(KEY4_GPIO_PORT, KEY4_GPIO_PIN)   /* ��ȡKEY4���� */
#define KEY5        gpio_input_bit_get(KEY5_GPIO_PORT, KEY5_GPIO_PIN)   /* ��ȡKEY5���� */
#define KEY6        gpio_input_bit_get(KEY6_GPIO_PORT, KEY6_GPIO_PIN)   /* ��ȡKEY6���� */
#define WK_UP       gpio_input_bit_get(WKUP_GPIO_PORT, WKUP_GPIO_PIN)   /* ��ȡWKUP���� */

#define KEY1_PRES 	1             /* KEY1���� */
#define KEY2_PRES 	2             /* KEY2���� */
#define KEY3_PRES 	3             /* KEY3���� */
#define KEY4_PRES 	4             /* KEY4���� */
#define KEY5_PRES 	5             /* KEY5���� */
#define KEY6_PRES 	6             /* KEY6���� */
#define WKUP_PRES   7             /* WK_UP���� */

/******************************************************************************************/

void key_init(void);              /* ������ʼ������ */
uint8_t key_scan(uint8_t mode);   /* ����ɨ�躯�� */

#endif
