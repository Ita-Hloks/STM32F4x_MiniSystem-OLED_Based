/**
 ****************************************************************************************************
 * @file        myiic.h
 * @version     V1.0
 * @brief       OLED IICģʽ ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */	
 
#ifndef __MYIIC_H
#define __MYIIC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* OLED IICģʽ���� ���� */

#define OLED_IIC_SCL_GPIO_PORT               GPIOB
#define OLED_IIC_SCL_GPIO_PIN                GPIO_PIN_8
#define OLED_IIC_SCL_GPIO_CLK                RCU_GPIOB     /* GPIOBʱ��ʹ�� */

#define OLED_IIC_SDA_GPIO_PORT               GPIOB
#define OLED_IIC_SDA_GPIO_PIN                GPIO_PIN_9
#define OLED_IIC_SDA_GPIO_CLK                RCU_GPIOB     /* GPIOBʱ��ʹ�� */

/******************************************************************************************/

/* IO�������� */
#define IIC_SCL(x)      do{ x ? \
                            gpio_bit_write(OLED_IIC_SCL_GPIO_PORT, OLED_IIC_SCL_GPIO_PIN, SET) : \
                            gpio_bit_write(OLED_IIC_SCL_GPIO_PORT, OLED_IIC_SCL_GPIO_PIN, RESET); \
                        }while(0)         /* SCL */ 

#define IIC_SDA(x)      do{ x ? \
                            gpio_bit_write(OLED_IIC_SDA_GPIO_PORT, OLED_IIC_SDA_GPIO_PIN, SET) : \
                            gpio_bit_write(OLED_IIC_SDA_GPIO_PORT, OLED_IIC_SDA_GPIO_PIN, RESET); \
                        }while(0)         /* SDA */

#define IIC_READ_SDA    gpio_input_bit_get(OLED_IIC_SDA_GPIO_PORT, OLED_IIC_SDA_GPIO_PIN)  /* ��ȡSDA */

/******************************************************************************************/
                        
/* IIC���в������� */
void iic_init(void);                /* ��ʼ��IIC��IO�� */
void iic_start(void);               /* ����IIC��ʼ�ź� */
void iic_stop(void);                /* ����IICֹͣ�ź� */
void iic_ack(void);                 /* IIC����ACK�ź� */
void iic_nack(void);                /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);         /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t txd);    /* IIC����һ���ֽ� */
uint8_t iic_read_byte(uint8_t ack); /* IIC��ȡһ���ֽ� */
                        
#endif
















