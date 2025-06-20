/**
 ****************************************************************************************************
 * @file        norflash.c
 * @version     V1.0
 * @brief       NOR FLASH(25QXX) ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */

#ifndef __norflash_H
#define __norflash_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* NORFLASH Ƭѡ ���� ���� */

#define NORFLASH_CS_GPIO_PORT           GPIOB
#define NORFLASH_CS_GPIO_PIN            GPIO_PIN_12
#define NORFLASH_CS_GPIO_CLK            RCU_GPIOB     /* GPIOBʱ��ʹ�� */

/******************************************************************************************/

/* NORFLASH Ƭѡ�ź� */
#define NORFLASH_CS(x)      do{ x ? \
                                gpio_bit_write(NORFLASH_CS_GPIO_PORT, NORFLASH_CS_GPIO_PIN, SET) : \
                                gpio_bit_write(NORFLASH_CS_GPIO_PORT, NORFLASH_CS_GPIO_PIN, RESET); \
                            }while(0)    


/* FLASHоƬ�б� */
#define GD25Q40     0xC812          /* GD25Q40E оƬID */																
#define W25Q80      0XEF13          /* W25Q80   оƬID */
#define W25Q16      0XEF14          /* W25Q16   оƬID */
#define W25Q32      0XEF15          /* W25Q32   оƬID */
#define W25Q64      0XEF16          /* W25Q64   оƬID */
#define W25Q128     0XEF17          /* W25Q128  оƬID */
#define W25Q256     0XEF18          /* W25Q256  оƬID */
#define BY25Q64     0X6816          /* BY25Q64  оƬID */
#define BY25Q128    0X6817          /* BY25Q128 оƬID */
#define BY25Q256    0X6818          /* BY25Q256 оƬID */
#define NM25Q64     0X5216          /* NM25Q64  оƬID */
#define NM25Q128    0X5217          /* NM25Q128 оƬID */

extern uint16_t g_norflash_type;    /* ����FLASHоƬ�ͺ� */
 
/* ָ��� */
#define FLASH_WriteEnable           0x06 
#define FLASH_WriteDisable          0x04 
#define FLASH_ReadStatusReg1        0x05 
#define FLASH_ReadStatusReg2        0x35 
#define FLASH_ReadStatusReg3        0x15 
#define FLASH_WriteStatusReg1       0x01 
#define FLASH_WriteStatusReg2       0x31 
#define FLASH_WriteStatusReg3       0x11 
#define FLASH_ReadData              0x03 
#define FLASH_FastReadData          0x0B 
#define FLASH_FastReadDual          0x3B 
#define FLASH_FastReadQuad          0x6B  
#define FLASH_FastReadDual_IO       0xBB 
#define FLASH_FastReadQuad_IO       0xEB 
#define FLASH_PageProgram           0x02 
#define FLASH_PageProgramQuad       0x32 
#define FLASH_BlockErase            0xD8 
#define FLASH_SectorErase           0x20 
#define FLASH_ChipErase             0xC7 
#define FLASH_PowerDown             0xB9 
#define FLASH_ReleasePowerDown      0xAB 
#define FLASH_DeviceID              0xAB 
#define FLASH_ManufactDeviceID      0x90 
#define FLASH_JedecDeviceID         0x9F 
#define FLASH_Enable4ByteAddr       0xB7
#define FLASH_Exit4ByteAddr         0xE9
#define FLASH_SetReadParam          0xC0 
#define FLASH_EnterQPIMode          0x38
#define FLASH_ExitQPIMode           0xFF

/******************************************************************************************/

/* ��ͨ���� */
void norflash_init(void);                                                    /* ��ʼ��25QXX */
uint16_t norflash_read_id(void);                                             /* ��ȡFLASH ID */
void norflash_write_enable(void);                                            /* дʹ�� */
void norflash_write_disable(void);                                           /* д���� */
uint8_t norflash_read_sr(uint8_t regno);                                     /* ��ȡ״̬�Ĵ��� */
void norflash_write_sr(uint8_t regno, uint8_t sr);                           /* д״̬�Ĵ��� */

void norflash_wait_busy(void);                                               /* �ȴ����� */
void norflash_send_address(uint32_t address);                                /* ���͵�ַ */
void norflash_write_page(uint8_t *pbuf, uint32_t addr, uint16_t datalen);    /* д��page */
void norflash_write_nocheck(uint8_t *pbuf, uint32_t addr, uint32_t datalen); /* дflash,�������� */

void norflash_erase_chip(void);                                              /* ��Ƭ���� */
void norflash_erase_sector(uint32_t saddr);                                  /* �������� */
void norflash_read(uint8_t *pbuf, uint32_t addr, uint32_t datalen);          /* ��ȡflash */
void norflash_write(uint8_t *pbuf, uint32_t addr, uint32_t datalen);         /* д��flash */

#endif















