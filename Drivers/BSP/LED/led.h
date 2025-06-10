/**
 ****************************************************************************************************
 * @file        led.h
 * @version     V1.0
 * @brief       LED ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */	

#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ���� ���� */

#define LED1_GPIO_PORT                  GPIOD
#define LED1_GPIO_PIN                   GPIO_PIN_1
#define LED1_GPIO_CLK                   RCU_GPIOD     /* GPIODʱ��ʹ�� */

#define LED2_GPIO_PORT                  GPIOD
#define LED2_GPIO_PIN                   GPIO_PIN_3
#define LED2_GPIO_CLK                   RCU_GPIOD     /* GPIODʱ��ʹ�� */

#define LED3_GPIO_PORT                  GPIOD
#define LED3_GPIO_PIN                   GPIO_PIN_5
#define LED3_GPIO_CLK                   RCU_GPIOD     /* GPIODʱ��ʹ�� */

#define LED4_GPIO_PORT                  GPIOD
#define LED4_GPIO_PIN                   GPIO_PIN_7
#define LED4_GPIO_CLK                   RCU_GPIOD     /* GPIODʱ��ʹ�� */

#define LED5_GPIO_PORT                  GPIOB
#define LED5_GPIO_PIN                   GPIO_PIN_4
#define LED5_GPIO_CLK                   RCU_GPIOB     /* GPIOBʱ��ʹ�� */

#define LED6_GPIO_PORT                  GPIOB
#define LED6_GPIO_PIN                   GPIO_PIN_6
#define LED6_GPIO_CLK                   RCU_GPIOB     /* GPIOBʱ��ʹ�� */


/* LED�˿ڶ��� */
#define LED1(x)   do{ x ? \
                      gpio_bit_write(LED1_GPIO_PORT, LED1_GPIO_PIN, SET) : \
                      gpio_bit_write(LED1_GPIO_PORT, LED1_GPIO_PIN, RESET); \
                  }while(0)      

#define LED2(x)   do{ x ? \
                      gpio_bit_write(LED2_GPIO_PORT, LED2_GPIO_PIN, SET) : \
                      gpio_bit_write(LED2_GPIO_PORT, LED2_GPIO_PIN, RESET); \
                  }while(0)     

#define LED3(x)   do{ x ? \
                      gpio_bit_write(LED3_GPIO_PORT, LED3_GPIO_PIN, SET) : \
                      gpio_bit_write(LED3_GPIO_PORT, LED3_GPIO_PIN, RESET); \
                  }while(0)      

#define LED4(x)   do{ x ? \
                      gpio_bit_write(LED4_GPIO_PORT, LED4_GPIO_PIN, SET) : \
                      gpio_bit_write(LED4_GPIO_PORT, LED4_GPIO_PIN, RESET); \
                  }while(0)    

#define LED5(x)   do{ x ? \
                      gpio_bit_write(LED5_GPIO_PORT, LED5_GPIO_PIN, SET) : \
                      gpio_bit_write(LED5_GPIO_PORT, LED5_GPIO_PIN, RESET); \
                  }while(0)      

#define LED6(x)   do{ x ? \
                      gpio_bit_write(LED6_GPIO_PORT, LED6_GPIO_PIN, SET) : \
                      gpio_bit_write(LED6_GPIO_PORT, LED6_GPIO_PIN, RESET); \
                  }while(0)     

                  
/* LEDȡ������ */
#define LED1_TOGGLE()   do{ gpio_bit_toggle(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)      /* ��תLED1 */
#define LED2_TOGGLE()   do{ gpio_bit_toggle(LED2_GPIO_PORT, LED2_GPIO_PIN); }while(0)      /* ��תLED2 */
#define LED3_TOGGLE()   do{ gpio_bit_toggle(LED3_GPIO_PORT, LED3_GPIO_PIN); }while(0)      /* ��תLED3 */
#define LED4_TOGGLE()   do{ gpio_bit_toggle(LED4_GPIO_PORT, LED4_GPIO_PIN); }while(0)      /* ��תLED4 */
#define LED5_TOGGLE()   do{ gpio_bit_toggle(LED5_GPIO_PORT, LED5_GPIO_PIN); }while(0)      /* ��תLED5 */
#define LED6_TOGGLE()   do{ gpio_bit_toggle(LED6_GPIO_PORT, LED6_GPIO_PIN); }while(0)      /* ��תLED6 */

/******************************************************************************************/

void led_init(void);             /* ��ʼ��LED */
void led_close_all(void);

#endif





