/**
 ****************************************************************************************************
 * @file        oled.c
 * @version     V1.0
 * @brief       OLED 驱动代码
 ****************************************************************************************************
 * @attention   Waiken-Smart 慧勤智远
 *
 * 实验平台:    GD32F470VET6
 *
 ****************************************************************************************************
 */
 
#include "stdlib.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/OLED/oledfont.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/IIC/myiic.h"


/* 
 * OLED的显存
 * 每个字节表示8个像素, 128,表示有128列, 4表示有32行, 高位表示高行数. 
 * 比如:g_oled_gram[0][0],包含了第一列,第1~8行的数据. g_oled_gram[0][0].0,即表示坐标(0,0)
 * 类似的: g_oled_gram[1][0].1,表示坐标(1,1), g_oled_gram[10][1].2,表示坐标(10,10), 
 * 
 * 存放格式如下(高位表示高行数).
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 */
static uint8_t g_oled_gram[128][4];
 
/**
 * @brief       更新显存到OLED
 * @param       无
 * @retval      无
 */
void oled_refresh_gram(void)
{
    uint8_t i, n;

    for (i = 0; i < 4; i++)
    {
        oled_wr_byte (0xb0 + i, OLED_CMD); /* 设置页地址（0~3） */
        oled_wr_byte (0x00, OLED_CMD);     /* 设置显示位置—列低地址 */
        oled_wr_byte (0x10, OLED_CMD);     /* 设置显示位置—列高地址 */
 
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
 * @brief       向OLED写入一个字节
 * @param       data: 要写入的数据/命令
 * @param       cmd: 数据/命令标志 0,表示命令;1,表示数据;
 * @retval      无
 */
static void oled_wr_byte(uint8_t data, uint8_t cmd)
{
	    iic_start();             /* 产生起始信号 */
	    iic_send_byte(0X78);     /* 发送从机地址 */
	    iic_wait_ack();          /* 每次发送完一个字节,都要等待ACK时钟脉冲 */
  
	    if(cmd == OLED_CMD)
      {
          iic_send_byte(0x00); /* 发送写入命令时的控制字节 */
      }
			else
      {
          iic_send_byte(0x40); /* 发送写入数据时的控制字节 */
      }
      
			iic_wait_ack();          /* 每次发送完一个字节,都要等待ACK时钟脉冲 */
			iic_send_byte(data);	   /* 发送数据字节 */
			iic_wait_ack();          /* 等待ACK时钟脉冲 */
			iic_stop();              /* 产生停止信号 */
}
 
/**
 * @brief       开启OLED显示
 * @param       无
 * @retval      无
 */
void oled_display_on(void)
{
    oled_wr_byte(0X8D, OLED_CMD);   /* SET DCDC命令 */
    oled_wr_byte(0X14, OLED_CMD);   /* DCDC ON */
    oled_wr_byte(0XAF, OLED_CMD);   /* DISPLAY ON */
}
 
/**
 * @brief       关闭OLED显示
 * @param       无
 * @retval      无
 */
void oled_display_off(void)
{
    oled_wr_byte(0X8D, OLED_CMD);   /* SET DCDC命令 */
    oled_wr_byte(0X10, OLED_CMD);   /* DCDC OFF */
    oled_wr_byte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}
 
/**
 * @brief       清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
 * @param       无
 * @retval      无
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

    oled_refresh_gram();    /* 更新显示 */
}

/**
 * @brief       OLED画点 
 * @param       x  : 0~127
 * @param       y  : 0~63
 * @param       dot: 1 填充 0,清空
 * @retval      无
 */ 
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot)
{
    uint8_t pos, bx, temp = 0;

    if (x > 127 || y > 31)
    {
        return;                     /* 超出范围了. */
    }

    pos = y / 8;                    /* 计算GRAM里面的y坐标所在的字节, 每个字节可以存储8个行坐标 */

    bx = y % 8;                     /* 取余数,方便计算y在对应字节里面的位置,及行(y)位置 */
    temp = 1 << bx;                 /* 高位表示高行号, 得到y对应的bit位置,将该bit先置1 */

    if (dot)                        /* 画实心点 */
    {
        g_oled_gram[x][pos] |= temp;
    }
    else                            /* 画空点,即不显示 */
    {
        g_oled_gram[x][pos] &= ~temp;
    }
}

/**
 * @brief       OLED填充区域填充 
 * @note:       注意:需要确保: x1<=x2; y1<=y2  0<=x1<=127  0<=y1<=63
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标 
 * @param       dot: 1 填充 0,清空
 * @retval      无
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

    oled_refresh_gram();    /* 更新显示 */
}

/**
 * @brief       在指定位置显示一个字符,包括部分字符 
 * @param       x   : 0~127
 * @param       y   : 0~63
 * @param       size: 选择字体 12/16/24
 * @param       mode: 0,反白显示;1,正常显示
 * @retval      无
 */ 
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t *pfont = 0;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* 得到字体一个字符对应点阵集所占的字节数 */
    chr = chr - ' ';                                                /* 得到偏移后的值,因为字库是从空格开始存储的,第一个字符是空格 */

    if (y + size > 32)    return;                                   /* 高度限制 */
	
    if (size == 12)                                                 /* 调用1206字体 */
    {
        pfont = (uint8_t *)oled_asc2_1206[chr];        
    }
    else if (size == 16)                                            /* 调用1608字体 */ 
    {
        pfont = (uint8_t *)oled_asc2_1608[chr];
    }
    else if (size == 24)                                            /* 调用2412字体 */
    { 
        pfont = (uint8_t *)oled_asc2_2412[chr];
    }
    else                                                            /* 没有的字库 */
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
 * @brief       平方函数, m^n
 * @param       m: 底数
 * @param       n: 指数 
 * @retval      无
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
 * @brief          显示len个数字 
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       len : 显示数字的位数
 * @param       size: 选择字体 12/16/24
 * @param       fill_zero: 是否补0？(如2位数的1，显示 01 而不是 1)
 * @retval      无
 */  
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t fill_zero)
{
    uint8_t t, temp;
    uint8_t enshow = !fill_zero;    /* 如果fill_zero为1，enshow始终为0，强制显示所有位 */

    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;

        if (enshow == 0 || temp != 0 || t == (len - 1))
        {
            oled_show_char(x + (size / 2) * t, y, temp + '0', size, 1);
            enshow = 1;     /* 一旦开始显示，后续都显示 */
        }
        else
        {
            oled_show_char(x + (size / 2) * t, y, '0', size, 1);   /* 显示0而不是空格 */
        }
    }
}

/**
 * @brief       显示字符串 
 * @param       x,y : 起始坐标
 * @param       size: 选择字体 12/16/24
 * @param       *p  : 字符串指针,指向字符串首地址 
 * @retval      无
 */ 
void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size)
{
    while ((*p <= '~') && (*p >= ' '))      /* 判断是不是非法字符! */
    {
        if (x > (128 - (size / 2)))         /* 宽度越界 */
        {
            x = 0;
            y += size;                      /* 换行 */
        }

        if (y > (64 - size))                /* 高度越界 */
        {
            y = x = 0;
            oled_clear();
        }

        oled_show_char(x, y, *p, size, 1);  /* 显示一个字符 */
        x += size / 2;                      /* ASCII字符宽度为汉字宽度的一半 */
        p++;
    }
}
 
/**
 * @brief       在指定位置，显示一个24*24大小的汉字 
 * @param       x,y : 起始坐标
 * @param       fnum:汉字编号（在字库数组里面的编号）
 * @retval      无
 */ 
/* The range of the y is [0-8]*/
void OLED_Show_Font(uint16_t x,uint16_t y,uint8_t fnum)
{
	if (y + 24 > 32)   return;  /* 限制汉字显示在屏幕范围内 */
        
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t *dzk;   
	uint8_t csize=72;					/* 一个24*24的汉字72字节 */
	
	dzk=(uint8_t*)OLED_HZK_TBL[fnum];	/* 得到汉字编号对应的点阵库 */
	
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];				/* 得到点阵数据 */                          
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
 * @brief       初始化OLED(SSD1306)  
 * @param       无
 * @retval      无
 */
void oled_init(void)
{ 
		iic_init();     /* IIC接口初始化 */

    oled_wr_byte(0xAE, OLED_CMD);   /* 关闭显示 */
    oled_wr_byte(0xD5, OLED_CMD);   /* 设置时钟分频因子,震荡频率 */
    oled_wr_byte(80, OLED_CMD);     /* [3:0],分频因子;[7:4],震荡频率 */
    oled_wr_byte(0xA8, OLED_CMD);   /* 设置驱动路数 */
    oled_wr_byte(0X1F, OLED_CMD);   /* 默认0X1F(1/32) */
    oled_wr_byte(0xD3, OLED_CMD);   /* 设置显示偏移 */
    oled_wr_byte(0X00, OLED_CMD);   /* 默认为0 */

    oled_wr_byte(0x40, OLED_CMD);   /* 设置显示开始行 [5:0],行数. */

    oled_wr_byte(0x8D, OLED_CMD);   /* 电荷泵设置 */
    oled_wr_byte(0x14, OLED_CMD);   /* bit2，开启/关闭 */
    oled_wr_byte(0x20, OLED_CMD);   /* 设置内存地址模式 */
    oled_wr_byte(0x02, OLED_CMD);   /* [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10; */
    oled_wr_byte(0xA1, OLED_CMD);   /* 段重定义设置,bit0:0,0->0;1,0->127; */
    oled_wr_byte(0xC8, OLED_CMD);   /* 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 */
    oled_wr_byte(0xDA, OLED_CMD);   /* 设置COM硬件引脚配置 */
    oled_wr_byte(0x02, OLED_CMD);   /* 32行配置 */

    oled_wr_byte(0x81, OLED_CMD);   /* 对比度设置 */
    oled_wr_byte(0xEF, OLED_CMD);   /* 1~255;默认0X7F (亮度设置,越大越亮) */
    oled_wr_byte(0xD9, OLED_CMD);   /* 设置预充电周期 */
    oled_wr_byte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oled_wr_byte(0xDB, OLED_CMD);   /* 设置VCOMH 电压倍率 */
    oled_wr_byte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

    oled_wr_byte(0xA4, OLED_CMD);   /* 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) */
    oled_wr_byte(0xA6, OLED_CMD);   /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
    oled_wr_byte(0xAF, OLED_CMD);   /* 开启显示 */
    oled_clear();
}





























