/**
 ****************************************************************************************************
 * @file        rtc.c
 * @version     V1.0
 * @brief       RTC ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */
 
#include "./BSP/RTC/rtc.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


rtc_parameter_struct rtc_initpara;  /* RTC�����ṹ�� */
    
/**
 * @brief       ʮ����ת��ΪBCD��
 * @param       val : Ҫת����ʮ������ 
 * @retval      BCD��
 */
static uint8_t rtc_dec2bcd(uint8_t val)
{
    uint8_t bcdhigh = 0;

    while (val >= 10)
    {
        bcdhigh++;
        val -= 10;
    }

    return ((uint8_t)(bcdhigh << 4) | val);
}

/**
 * @brief       BCD��ת��Ϊʮ��������
 * @param       val : Ҫת����BCD�� 
 * @retval      ʮ��������
 */
static uint8_t rtc_bcd2dec(uint8_t val)
{
    uint8_t temp = 0;
    temp = (val >> 4) * 10;
    return (temp + (val & 0X0F));
}

/**
 * @brief       RTCʱ������
 * @param       hour,min,sec: Сʱ,����,���� 
 * @param       ampm        : AM/PM, 0=AM/24H; 1=PM/12H;
 * @retval      0,�ɹ�
 *              1,��ʼ��ʧ��
 */
uint8_t rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm)
{
    ErrStatus error_status = ERROR;   
  
    rtc_initpara.hour   = rtc_dec2bcd(hour);       
    rtc_initpara.minute = rtc_dec2bcd(min);       
    rtc_initpara.second = rtc_dec2bcd(sec);      
    rtc_initpara.am_pm  = ((uint32_t)ampm & 0X01);
    
    error_status = rtc_init(&rtc_initpara);   /* ���ݲ�����ʼ��RTC�Ĵ��� */
    
    if (ERROR == error_status) return 1;
    
    return 0;  
}

/**
 * @brief       RTC��������
 * @param       year,month,date : ��(0~99),��(1~12),��(1~31)
 * @param       week            : ����(1~7,������һ~����;0,�Ƿ�!)
 * @retval      0,�ɹ�
 *              1,��ʼ��ʧ��
 */
uint8_t rtc_set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week)
{
   ErrStatus error_status = ERROR;   
  
    rtc_initpara.year        = rtc_dec2bcd(year);       
    rtc_initpara.month       = rtc_dec2bcd(month);       
    rtc_initpara.date        = rtc_dec2bcd(date);      
    rtc_initpara.day_of_week = (week & 0X07);
    
    error_status = rtc_init(&rtc_initpara);   /* ���ݲ�����ʼ��RTC�Ĵ��� */
    
    if (ERROR == error_status) return 1;
    
    return 0;  
}

/**
 * @brief       ��ȡRTCʱ��
 * @param       *hour,*min,*sec : Сʱ,����,����
 * @param       *ampm           : AM/PM,0=AM/24H,1=PM/12H.
 * @retval      ��
 */
void rtc_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *ampm)
{
    rtc_current_time_get(&rtc_initpara);   /* ��ȡ��ǰ���ں�ʱ�� */
  
    *hour = rtc_bcd2dec(rtc_initpara.hour);
    *min  = rtc_bcd2dec(rtc_initpara.minute);
    *sec  = rtc_bcd2dec(rtc_initpara.second);
    *ampm = (uint8_t)(rtc_initpara.am_pm >> 22);
}

/**
 * @brief       ��ȡRTC����
 * @param       *year,*mon,*date: ��,��,��
 * @param       *week           : ����
 * @retval      ��
 */
void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week)
{
    rtc_current_time_get(&rtc_initpara);   /* ��ȡ��ǰ���ں�ʱ�� */  
  
    *year  = rtc_bcd2dec(rtc_initpara.year);
    *month = rtc_bcd2dec(rtc_initpara.month);
    *date  = rtc_bcd2dec(rtc_initpara.date);
    *week  = rtc_initpara.day_of_week;
}

/* ���������ݱ� */
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

/**
 * @brief       ������������ڼ�, ���빫�����ڵõ�����(ֻ����1901-2099��)
 * @param       year,month,day������������
 * @retval      ���ں�(1~7,������1~����)
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
    uint16_t temp;
    uint8_t yearh, yearl;

    yearh = year / 100;
    yearl = year % 100;

    /*  ���Ϊ21����,�������100 */
    if (yearh > 19)
    {
        yearl += 100;
    }

    /*  ����������ֻ��1900��֮��� */
    temp = yearl + yearl / 4;
    temp = temp % 7;
    temp = temp + day + table_week[month - 1];

    if (yearl % 4 == 0 && month < 3)
    {
        temp--;
    }

    temp %= 7;

    if (temp == 0)
    {
        temp = 7;
    }

    return temp;
}

/**
 * @brief       RTC��ʼ��
 * @note 
 *              Ĭ�ϳ���ʹ��LSE,��LSE����ʧ�ܺ�,�л�ΪLSI.
 *              ͨ��BKP�Ĵ���0��ֵ,�����ж�RTCʹ�õ���LSE/LSI:
 *              ��BKP0==0X7050ʱ,ʹ�õ���LSE
 *              ��BKP0==0X7051ʱ,ʹ�õ���LSI
 *              ע��:�л�LSI/LSE������ʱ��/���ڶ�ʧ,�л�������������.
 *
 * @param       ��
 * @retval      0,�ɹ�
 *              1,����ʱ��ʧ��
 */
uint8_t rtc_config(void)
{
    uint16_t prescaler_a = 0;                           /* RTC�첽��Ƶֵ */
    uint16_t prescaler_s  = 0;                          /* RTCͬ����Ƶֵ */
    uint8_t RTCSRC_FLAG = 0;
    uint16_t bkpflag = 0;
  
    rcu_periph_clock_enable(RCU_PMU);                   /* ʹ�ܵ�Դ�ӿ�ʱ�� */
    pmu_backup_write_enable();                          /* ������дʹ�� */   
    bkpflag = RTC_BKP0;                                 /* ��ȡBKP0��ֵ */
    rcu_osci_on(RCU_LXTAL);                             /* ���ⲿ���پ��� */
		
    if ((rcu_osci_stab_wait(RCU_LXTAL) == ERROR))       /* ����CK_LXTALʧ��? */
    { 
        rcu_osci_on(RCU_IRC32K);                        /* ��CK_IRC32Kʱ�� */     
      
        if ((rcu_osci_stab_wait(RCU_IRC32K) == ERROR))  /* ����CK_IRC32Kʧ��? */
        {
            return 1;                                   /* ����RTCʱ��Դʧ�� */
        }
        
        rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);        /* ѡ�� CK_IRC32Kʱ����ΪRTC��ʱ��Դ */
        prescaler_s = 0x13F;                            /* RTCͬ����Ƶֵ(0~7FFF) */
        prescaler_a = 0x63;                             /* RTC�첽��Ƶֵ(0~0X7F) */
        RTC_BKP0 = 0X7051;                              /* ���RTCʹ��LSI */
    }	
    else
    {  
        rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);         /* ѡ��RCU_LXTALʱ����ΪRTC��ʱ��Դ */
        prescaler_s = 0xFF;
        prescaler_a = 0x7F;
        RTC_BKP0 = 0X7050;                              /* ���RTCʹ��LSE */
    }
    
    rcu_periph_clock_enable(RCU_RTC);                   /* ʹ��RTCʱ�� */
    rtc_register_sync_wait();                           /* �ȴ�ͬ�� */                  
    RTCSRC_FLAG = GET_BITS(RCU_BDCTL, 8, 9);            /* ��ȡRTCʱ��Դѡ�� */ 
    
    /* BKP0�����ݼȲ���0X7050,Ҳ����0X7051,˵���ǵ�һ������,����RTCʱ��Դû�����ã���Ҫ����ʱ������. */
    if(((bkpflag != 0x7050) && (bkpflag != 0x7051)) || (0x00 == RTCSRC_FLAG))    
    {  
        rtc_initpara.factor_asyn = prescaler_a;         /* ����RTC�첽��Ƶϵ�� */
        rtc_initpara.factor_syn = prescaler_s;          /* ����RTCͬ����Ƶϵ�� */
        rtc_initpara.display_format = RTC_24HOUR;       /* RTCʱ���ʽΪ24Сʱ��ʽ */
        rtc_set_time(23, 59, 50, 0);                    /* ����ʱ�� */
        rtc_set_date(24, 11, 13, 3);                    /* �������� */
    
    }  
    
    return 0;    
}

/**
 * @brief       ��������ʱ��(����������,24Сʱ��)
 * @param       week        : ���ڼ�(1~7) 
 * @param       hour,min,sec: Сʱ,����,����
 * @retval      ��
 */
void rtc_set_alarma(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)
{
    rtc_alarm_struct  rtc_alarm; 
  
    rtc_alarm_disable(RTC_ALARM0);                           /* �ر�RTC����0 */
    rtc_alarm.alarm_mask = 0;
    rtc_alarm.weekday_or_date = RTC_ALARM_WEEKDAY_SELECTED;  /* ���������� */
    rtc_alarm.alarm_day    = rtc_dec2bcd(week) ;             /* �������� */
    rtc_alarm.alarm_hour   = rtc_dec2bcd(hour) ;             /* Сʱ���� */
    rtc_alarm.alarm_minute = rtc_dec2bcd(min) ;              /* �������� */
    rtc_alarm.alarm_second = rtc_dec2bcd(sec);               /* �������� */
    rtc_alarm.am_pm = RTC_AM;                                /* 24Сʱ�� */  
    rtc_alarm_config(RTC_ALARM0, &rtc_alarm);                /* ����RTC����0 */ 
  
    rtc_interrupt_enable(RTC_INT_ALARM0);                    /* ʹ������0�ж� */
    rtc_alarm_enable(RTC_ALARM0);                            /* ʹ��RTC����0 */  
    rtc_flag_clear(RTC_FLAG_ALRM0);                          /* ���RTC����0������־ */   
  
    exti_flag_clear(EXTI_17);                                /* ���Line17�ϵ��жϱ�־λ */
    exti_init(EXTI_17, EXTI_INTERRUPT, EXTI_TRIG_RISING);    /* ����Line17�ϵ��жϣ�����Ϊ�����ش����ж� */
    nvic_irq_enable(RTC_Alarm_IRQn, 1, 2);                   /* �������ȼ�����ռ���ȼ�Ϊ1����Ӧ���ȼ�Ϊ2 */
}

/**
 * @brief       �����Ի��Ѷ�ʱ������
 * @param       wksel
 *   @arg       000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
 *   @arg       10x,ck_spre,1Hz;11x,1Hz,��cntֵ����2^16(��cnt+2^16)
 * @note        ע��:RTC����RTC��ʱ��Ƶ��,��RTCCLK!
 * @param       cnt: �Զ���װ��ֵ.����0,�����ж�.
 * @retval      ��
 */
void rtc_set_wakeup(uint8_t wksel, uint16_t cnt)
{    
    rtc_wakeup_disable();                  /* ʧ��RTC�Զ����� */
   
    rtc_wakeup_clock_set(wksel);           /* ����RTC�Զ����Ѷ�ʱ��ʱ�� */
    
    rtc_wakeup_timer_set(cnt);             /* �����Զ����Ѷ�ʱ��ֵ */
    
    rtc_flag_clear(RTC_FLAG_WT);           /* ����Զ����ѷ�����־ */
   
    rtc_interrupt_enable(RTC_INT_WAKEUP);  /* ʹ��RTC�Զ������ж� */
  
    rtc_wakeup_enable();                   /* ʹ��RTC�Զ����ѹ��� */
  
    exti_flag_clear(EXTI_22);                               /* ���Line22�ϵ��жϱ�־λ */
    exti_init(EXTI_22, EXTI_INTERRUPT, EXTI_TRIG_RISING);   /* ����Line22�ϵ��жϣ�����Ϊ�����ش����ж� */
    nvic_irq_enable(RTC_WKUP_IRQn, 2, 2);                   /* �������ȼ�����ռ���ȼ�Ϊ2����Ӧ���ȼ�Ϊ2 */
}

/**
 * @brief       RTC�����жϷ�����
 * @param       ��
 * @retval      ��
 */
void RTC_Alarm_IRQHandler(void)
{
    if (RESET != rtc_flag_get(RTC_FLAG_ALRM0)) /* ALARM0�ж�? */
    {
        rtc_flag_clear(RTC_FLAG_ALRM0);        /* ����жϱ�־ */   
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        LED5_TOGGLE();
        LED6_TOGGLE();
    } 
    
    exti_flag_clear(EXTI_17);                  /* ����ж���17���жϱ�־ */
}

/**
 * @brief       RTC WAKE UP�жϷ�����
 * @param       ��
 * @retval      ��
 */
void RTC_WKUP_IRQHandler(void)
{   
    if (RESET != rtc_flag_get(RTC_FLAG_WT))    /* WK_UP�ж�? */   
    {       
        rtc_flag_clear(RTC_FLAG_WT);           /* ����жϱ�־ */
        // LED1_TOGGLE();
    } 
    
    exti_flag_clear(EXTI_22);                  /* ����ж���22���жϱ�־ */
}















