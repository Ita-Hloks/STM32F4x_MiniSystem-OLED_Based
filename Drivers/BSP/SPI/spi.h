/**
 ****************************************************************************************************
 * @file        spi.h
 * @version     V1.0
 * @brief       SPI ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */

#ifndef __SPI_H
#define __SPI_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* SPI1 ���� ���� */

#define SPI1_SCK_GPIO_PORT           GPIOB
#define SPI1_SCK_GPIO_PIN            GPIO_PIN_13
#define SPI1_SCK_GPIO_AF             GPIO_AF_5
#define SPI1_SCK_GPIO_CLK            RCU_GPIOB     /* GPIOBʱ��ʹ�� */

#define SPI1_MISO_GPIO_PORT          GPIOB
#define SPI1_MISO_GPIO_PIN           GPIO_PIN_14
#define SPI1_MISO_GPIO_AF            GPIO_AF_5
#define SPI1_MISO_GPIO_CLK           RCU_GPIOB     /* GPIOBʱ��ʹ�� */

#define SPI1_MOSI_GPIO_PORT          GPIOB
#define SPI1_MOSI_GPIO_PIN           GPIO_PIN_15
#define SPI1_MOSI_GPIO_AF            GPIO_AF_5
#define SPI1_MOSI_GPIO_CLK           RCU_GPIOB     /* GPIOBʱ��ʹ�� */

/* SPI1��ض��� */
#define SPI1_SPI                     SPI1
#define SPI1_SPI_CLK                 RCU_SPI1      /* SPI1ʱ��ʹ�� */
  
/******************************************************************************************/

/* SPI�����ٶ����� */
#define SPI_SPEED_2         0
#define SPI_SPEED_4         1
#define SPI_SPEED_8         2
#define SPI_SPEED_16        3
#define SPI_SPEED_32        4
#define SPI_SPEED_64        5
#define SPI_SPEED_128       6
#define SPI_SPEED_256       7


void spi1_init(void);                              /* SPI1��ʼ�� */               
void spi1_set_speed(uint8_t speed);                /* ����SPI1�ٶ� */
uint8_t spi1_read_write_byte(uint8_t txdata);      /* SPI1��дһ���ֽ� */

#endif
























