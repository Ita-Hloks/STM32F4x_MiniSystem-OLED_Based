/**
 ****************************************************************************************************
 * @file        oled.c
 * @version     V1.0
 * @brief       OLED ��������
 ****************************************************************************************************
 * @attention   Waiken-Smart ������Զ
 *
 * ʵ��ƽ̨:    GD32F470VET6
 *
 ****************************************************************************************************
 */
 
#include "stdlib.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/OLED/oledfont.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/IIC/myiic.h"


/* 
 * OLED���Դ�
 * ÿ���ֽڱ�ʾ8������, 128,��ʾ��128��, 4��ʾ��32��, ��λ��ʾ������. 
 * ����:g_oled_gram[0][0],�����˵�һ��,��1~8�е�����. g_oled_gram[0][0].0,����ʾ����(0,0)
 * ���Ƶ�: g_oled_gram[1][0].1,��ʾ����(1,1), g_oled_gram[10][1].2,��ʾ����(10,10), 
 * 
 * ��Ÿ�ʽ����(��λ��ʾ������).
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 */
static uint8_t g_oled_gram[128][4];
 
/**
 * @brief       �����Դ浽OLED
 * @param       ��
 * @retval      ��
 */
void oled_refresh_gram(void)
{
    uint8_t i, n;

    for (i = 0; i < 4; i++)
    {
        oled_wr_byte (0xb0 + i, OLED_CMD); /* ����ҳ��ַ��0~3�� */
        oled_wr_byte (0x00, OLED_CMD);     /* ������ʾλ�á��е͵�ַ */
        oled_wr_byte (0x10, OLED_CMD);     /* ������ʾλ�á��иߵ�ַ */
 
        iic_start();
			  iic_send_byte(0X78);
			  iic_wait_ack();
			  iic_send_byte(0x40);
				iic_wait_ack();
       
			  for (n = 0; n < 128; n++)
			  {         	
						iic_send_byte(g_oled_gram[n][i]);	
						iic_wait_ack();
			  }
			  iic_stop();
    }
}
  
/**
 * @brief       ��OLEDд��һ���ֽ�
 * @param       data: Ҫд�������/����
 * @param       cmd: ����/�����־ 0,��ʾ����;1,��ʾ����;
 * @retval      ��
 */
static void oled_wr_byte(uint8_t data, uint8_t cmd)
{
	    iic_start();             /* ������ʼ�ź� */
	    iic_send_byte(0X78);     /* ���ʹӻ���ַ */
	    iic_wait_ack();          /* ÿ�η�����һ���ֽ�,��Ҫ�ȴ�ACKʱ������ */
  
	    if(cmd == OLED_CMD)
      {
          iic_send_byte(0x00); /* ����д������ʱ�Ŀ����ֽ� */
      }
			else
      {
          iic_send_byte(0x40); /* ����д������ʱ�Ŀ����ֽ� */
      }
      
			iic_wait_ack();          /* ÿ�η�����һ���ֽ�,��Ҫ�ȴ�ACKʱ������ */
			iic_send_byte(data);	   /* ���������ֽ� */
			iic_wait_ack();          /* �ȴ�ACKʱ������ */
			iic_stop();              /* ����ֹͣ�ź� */
}
 
/**
 * @brief       ����OLED��ʾ
 * @param       ��
 * @retval      ��
 */
void oled_display_on(void)
{
    oled_wr_byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    oled_wr_byte(0X14, OLED_CMD);   /* DCDC ON */
    oled_wr_byte(0XAF, OLED_CMD);   /* DISPLAY ON */
}
 
/**
 * @brief       �ر�OLED��ʾ
 * @param       ��
 * @retval      ��
 */
void oled_display_off(void)
{
    oled_wr_byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    oled_wr_byte(0X10, OLED_CMD);   /* DCDC OFF */
    oled_wr_byte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}
 
/**
 * @brief       ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
 * @param       ��
 * @retval      ��
 */
void oled_clear(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            g_oled_gram[n][i] = 0X00;
        }
    }

    oled_refresh_gram();    /* ������ʾ */
}

/**
 * @brief       OLED���� 
 * @param       x  : 0~127
 * @param       y  : 0~63
 * @param       dot: 1 ��� 0,���
 * @retval      ��
 */ 
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot)
{
    uint8_t pos, bx, temp = 0;

    if (x > 127 || y > 31)
    {
        return;                     /* ������Χ��. */
    }

    pos = y / 8;                    /* ����GRAM�����y�������ڵ��ֽ�, ÿ���ֽڿ��Դ洢8�������� */

    bx = y % 8;                     /* ȡ����,�������y�ڶ�Ӧ�ֽ������λ��,����(y)λ�� */
    temp = 1 << bx;                 /* ��λ��ʾ���к�, �õ�y��Ӧ��bitλ��,����bit����1 */

    if (dot)                        /* ��ʵ�ĵ� */
    {
        g_oled_gram[x][pos] |= temp;
    }
    else                            /* ���յ�,������ʾ */
    {
        g_oled_gram[x][pos] &= ~temp;
    }
}

/**
 * @brief       OLED���������� 
 * @note:       ע��:��Ҫȷ��: x1<=x2; y1<=y2  0<=x1<=127  0<=y1<=63
 * @param       x1,y1: �������
 * @param       x2,y2: �յ����� 
 * @param       dot: 1 ��� 0,���
 * @retval      ��
 */ 
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{
    uint8_t x, y;

    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
        {
            oled_draw_point(x, y, dot);
        }
    }

    oled_refresh_gram();    /* ������ʾ */
}

/**
 * @brief       ��ָ��λ����ʾһ���ַ�,���������ַ� 
 * @param       x   : 0~127
 * @param       y   : 0~63
 * @param       size: ѡ������ 12/16/24
 * @param       mode: 0,������ʾ;1,������ʾ
 * @retval      ��
 */ 
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t *pfont = 0;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* �õ�����һ���ַ���Ӧ������ռ���ֽ��� */
    chr = chr - ' ';                                                /* �õ�ƫ�ƺ��ֵ,��Ϊ�ֿ��Ǵӿո�ʼ�洢��,��һ���ַ��ǿո� */

    if (y + size > 32)    return;                                   /* �߶����� */
	
    if (size == 12)                                                 /* ����1206���� */
    {
        pfont = (uint8_t *)oled_asc2_1206[chr];        
    }
    else if (size == 16)                                            /* ����1608���� */ 
    {
        pfont = (uint8_t *)oled_asc2_1608[chr];
    }
    else if (size == 24)                                            /* ����2412���� */
    { 
        pfont = (uint8_t *)oled_asc2_2412[chr];
    }
    else                                                            /* û�е��ֿ� */
    {
        return;   
    }
    
    for (t = 0; t < csize; t++)
    { 
        temp = pfont[t];
      
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
            {
                oled_draw_point(x, y, mode);
            }
            else
            {
                oled_draw_point(x, y, !mode);
            }

            temp <<= 1;
            y++;

            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

/**
 * @brief       ƽ������, m^n
 * @param       m: ����
 * @param       n: ָ�� 
 * @retval      ��
 */
static uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)
    {
        result *= m;
    }

    return result;
}

/**
 * @brief          ��ʾlen������ 
 * @param       x,y : ��ʼ����
 * @param       num : ��ֵ(0 ~ 2^32)
 * @param       len : ��ʾ���ֵ�λ��
 * @param       size: ѡ������ 12/16/24
 * @param       fill_zero: �Ƿ�0��(��2λ����1����ʾ 01 ������ 1)
 * @retval      ��
 */  
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t fill_zero)
{
    uint8_t t, temp;
    uint8_t enshow = !fill_zero;    /* ���fill_zeroΪ1��enshowʼ��Ϊ0��ǿ����ʾ����λ */

    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;

        if (enshow == 0 || temp != 0 || t == (len - 1))
        {
            oled_show_char(x + (size / 2) * t, y, temp + '0', size, 1);
            enshow = 1;     /* һ����ʼ��ʾ����������ʾ */
        }
        else
        {
            oled_show_char(x + (size / 2) * t, y, '0', size, 1);   /* ��ʾ0�����ǿո� */
        }
    }
}

/**
 * @brief       ��ʾ�ַ��� 
 * @param       x,y : ��ʼ����
 * @param       size: ѡ������ 12/16/24
 * @param       *p  : �ַ���ָ��,ָ���ַ����׵�ַ 
 * @retval      ��
 */ 
void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size)
{
    while ((*p <= '~') && (*p >= ' '))      /* �ж��ǲ��ǷǷ��ַ�! */
    {
        if (x > (128 - (size / 2)))         /* ���Խ�� */
        {
            x = 0;
            y += size;                      /* ���� */
        }

        if (y > (64 - size))                /* �߶�Խ�� */
        {
            y = x = 0;
            oled_clear();
        }

        oled_show_char(x, y, *p, size, 1);  /* ��ʾһ���ַ� */
        x += size / 2;                      /* ASCII�ַ����Ϊ���ֿ�ȵ�һ�� */
        p++;
    }
}
 
/**
 * @brief       ��ָ��λ�ã���ʾһ��24*24��С�ĺ��� 
 * @param       x,y : ��ʼ����
 * @param       fnum:���ֱ�ţ����ֿ���������ı�ţ�
 * @retval      ��
 */ 
/* The range of the y is [0-8]*/
void OLED_Show_Font(uint16_t x,uint16_t y,uint8_t fnum)
{
	if (y + 24 > 32)   return;  /* ���ƺ�����ʾ����Ļ��Χ�� */
        
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t *dzk;   
	uint8_t csize=72;					/* һ��24*24�ĺ���72�ֽ� */
	
	dzk=(uint8_t*)OLED_HZK_TBL[fnum];	/* �õ����ֱ�Ŷ�Ӧ�ĵ���� */
	
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];				/* �õ��������� */                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)oled_draw_point(x,y,1);
			else oled_draw_point(x,y,0); 
			temp<<=1;
			y++;
			if((y-y0)==24)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}

/**
 * @brief       ��ʼ��OLED(SSD1306)  
 * @param       ��
 * @retval      ��
 */
void oled_init(void)
{ 
		iic_init();     /* IIC�ӿڳ�ʼ�� */

    oled_wr_byte(0xAE, OLED_CMD);   /* �ر���ʾ */
    oled_wr_byte(0xD5, OLED_CMD);   /* ����ʱ�ӷ�Ƶ����,��Ƶ�� */
    oled_wr_byte(80, OLED_CMD);     /* [3:0],��Ƶ����;[7:4],��Ƶ�� */
    oled_wr_byte(0xA8, OLED_CMD);   /* ��������·�� */
    oled_wr_byte(0X1F, OLED_CMD);   /* Ĭ��0X1F(1/32) */
    oled_wr_byte(0xD3, OLED_CMD);   /* ������ʾƫ�� */
    oled_wr_byte(0X00, OLED_CMD);   /* Ĭ��Ϊ0 */

    oled_wr_byte(0x40, OLED_CMD);   /* ������ʾ��ʼ�� [5:0],����. */

    oled_wr_byte(0x8D, OLED_CMD);   /* ��ɱ����� */
    oled_wr_byte(0x14, OLED_CMD);   /* bit2������/�ر� */
    oled_wr_byte(0x20, OLED_CMD);   /* �����ڴ��ַģʽ */
    oled_wr_byte(0x02, OLED_CMD);   /* [1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10; */
    oled_wr_byte(0xA1, OLED_CMD);   /* ���ض�������,bit0:0,0->0;1,0->127; */
    oled_wr_byte(0xC8, OLED_CMD);   /* ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� */
    oled_wr_byte(0xDA, OLED_CMD);   /* ����COMӲ���������� */
    oled_wr_byte(0x02, OLED_CMD);   /* 32������ */

    oled_wr_byte(0x81, OLED_CMD);   /* �Աȶ����� */
    oled_wr_byte(0xEF, OLED_CMD);   /* 1~255;Ĭ��0X7F (��������,Խ��Խ��) */
    oled_wr_byte(0xD9, OLED_CMD);   /* ����Ԥ������� */
    oled_wr_byte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oled_wr_byte(0xDB, OLED_CMD);   /* ����VCOMH ��ѹ���� */
    oled_wr_byte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

    oled_wr_byte(0xA4, OLED_CMD);   /* ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) */
    oled_wr_byte(0xA6, OLED_CMD);   /* ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ */
    oled_wr_byte(0xAF, OLED_CMD);   /* ������ʾ */
    oled_clear();
}





























