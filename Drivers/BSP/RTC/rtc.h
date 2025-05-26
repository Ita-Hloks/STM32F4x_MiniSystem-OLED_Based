/**
 ****************************************************************************************************
 * @file        rtc.h
 * @version     V1.0
 * @brief       RTC ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */

#ifndef __RTC_H
#define __RTC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/

uint8_t rtc_config(void);                                                       /* ��ʼ��RTC */
void rtc_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *ampm);    /* ��ȡʱ�� */
uint8_t rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm);     /* ����ʱ�� */
void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week); /* ��ȡ���� */
uint8_t rtc_set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week);  /* �������� */

void rtc_set_wakeup(uint8_t wksel, uint16_t cnt);                               /* ���������Ի��� */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);                /* ��ȡ���� */
void rtc_set_alarma(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec);      /* �������� */

#endif

















