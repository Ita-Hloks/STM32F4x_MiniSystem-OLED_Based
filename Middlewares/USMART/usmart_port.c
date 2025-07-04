/**
 ****************************************************************************************************
 * @file        usmart_port.c
 * @version     V1.0
 * @brief       USMART 移植文件
 *
 *              通过修改该文件,可以方便的将USMART移植到其他工程
 *              当:USMART_ENTIMX_SCAN == 0时,仅需要实现: usmart_get_input_string函数.
 *              当:USMART_ENTIMX_SCAN == 1时,需要多实现4个函数:
 *              usmart_timerx_reset_runtime
 *              usmart_timerx_get_runtime
 *              usmart_timerx_init
 *              USMART_TIMERX_IRQHandler
 *
 ****************************************************************************************************
 */

#include "./USMART/usmart.h"
#include "./USMART/usmart_port.h"


/**
 * @brief       获取输入数据流(字符串)
 * @note        USMART通过解析该函数返回的字符串以获取函数名及参数等信息
 * @param       无
 * @retval
 *   @arg       0,  没有接收到数据
 *   @arg       其他,数据流首地址(不能是0)
 */
char *usmart_get_input_string(void)
{
    uint8_t len;
    char *pbuf = 0;

    if (g_usart_rx_sta & 0x8000)        /* 串口接收完成？ */
    {
        len = g_usart_rx_sta & 0x3fff;  /* 得到此次接收到的数据长度 */
        g_usart_rx_buf[len] = '\0';     /* 在末尾加入结束符. */
        pbuf = (char *)g_usart_rx_buf;
        g_usart_rx_sta = 0;             /* 开启下一次接收 */
    }

    return pbuf;
}

/* 如果使能了定时器扫描, 则需要定义如下函数 */
#if USMART_ENTIMX_SCAN == 1

/**
 * 移植注意:本例是以GD32为例,如果要移植到其他mcu,请做相应修改.
 * usmart_reset_runtime,清除函数运行时间,连同定时器的计数寄存器以及标志位一起清零.并设置重装载值为最大,以最大限度的延长计时时间.
 * usmart_get_runtime,获取函数运行时间,通过读取CNT值获取,由于usmart是通过中断调用的函数,所以定时器中断不再有效,此时最大限度
 * 只能统计2次CNT的值,也就是清零后+溢出一次,当溢出超过2次,没法处理,所以最大延时,控制在:2*计数器CNT*0.1ms.对GD32定时器(16位)来说,是:13.1s左右
 * 其他的:USMART_TIMERX_IRQHandler和usmart_timerx_init,需要根据MCU特点自行修改.确保计数器计数频率为:10Khz即可.另外,定时器不要开启自动重装载功能!!
 */

/**
 * @brief       复位runtime
 * @note        需要根据所移植到的MCU的定时器参数进行修改
 * @param       无
 * @retval      无
 */
void usmart_timerx_reset_runtime(void)
{
    timer_interrupt_flag_clear(USMART_TIMERX, TIMER_INT_FLAG_UP);   /* 清除定时器更新中断标志位 */
    timer_autoreload_value_config(USMART_TIMERX, 0XFFFF);           /* 将重装载值设置到最大 */
    timer_counter_value_config(USMART_TIMERX, 0);                   /* 清空定时器的CNT */
    usmart_dev.runtime = 0;
}

/**
 * @brief       获得runtime时间
 * @note        需要根据所移植到的MCU的定时器参数进行修改
 * @param       无
 * @retval      执行时间,单位:0.1ms,最大延时时间为定时器CNT值的2倍*0.1ms
 */
uint32_t usmart_timerx_get_runtime(void)
{
    if(timer_interrupt_flag_get(USMART_TIMERX, TIMER_INT_FLAG_UP) == SET)   /* 在运行期间,产生了定时器溢出 */
    {
        usmart_dev.runtime += 0XFFFF;
    }

    usmart_dev.runtime += timer_counter_read(USMART_TIMERX);
    return usmart_dev.runtime;      /* 返回计数值 */
}

/**
 * @brief       定时器初始化函数
 * @param       tclk: 定时器的工作频率(单位:Mhz)
 * @retval      无
 */
void usmart_timerx_init(uint16_t tclk)
{
    timer_parameter_struct timer_initpara;                        /* timer_initpara用于存放定时器的参数 */
  
    rcu_periph_clock_enable(USMART_TIMERX_CLK);                   /* 使能TIMERX的时钟 */

    timer_deinit(USMART_TIMERX);                                  /* 复位TIMERX */
    timer_struct_para_init(&timer_initpara);                      /* 初始化timer_initpara为默认值 */

    /* 根据tclk的值(单位:Mhz),将定时器的输入时钟分频到10KHz ,100ms中断一次
     * 注意,计数频率必须为10Khz,以和runtime单位(0.1ms)同步.
     */
    timer_initpara.prescaler         = (tclk * 100) - 1;          /* 设置预分频值 */
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;        /* 设置对齐模式为边沿对齐模式 */
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;          /* 设置向上计数模式 */
    timer_initpara.period            = 1000;                      /* 设置自动重装载值 */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;          /* 设置时钟分频因子 */
    timer_initpara.repetitioncounter = 0;                         /* 设置重复计数器值 */
    timer_init(USMART_TIMERX, &timer_initpara);                   /* 根据参数初始化定时器 */

    /* 使能定时器及其中断 */
    timer_interrupt_flag_clear(USMART_TIMERX, TIMER_INT_FLAG_UP); /* 清除定时器更新中断标志 */
  
    timer_interrupt_enable(USMART_TIMERX, TIMER_INT_UP);          /* 使能定时器的更新中断 */
    
    nvic_irq_enable(USMART_TIMERX_IRQn, 3, 3);                    /* 配置NVIC设置优先级，抢占优先级3，响应优先级3(组2中优先级最低的) */
    
    timer_enable(USMART_TIMERX);                                  /* 使能定时器TIMERX */	
}

/**
 * @brief       USMART定时器中断服务函数
 * @param       无
 * @retval      无
 */
void USMART_TIMERX_IRQHandler(void)
{
    if (timer_interrupt_flag_get(USMART_TIMERX, TIMER_INT_FLAG_UP) == SET)  /* 溢出中断 */
    {   
        usmart_dev.scan();                                                  /* 执行usmart扫描 */
        timer_counter_value_config(USMART_TIMERX, 0);                       /* 清空定时器的CNT */
        timer_autoreload_value_config(USMART_TIMERX, 1000);                 /* 恢复原来的设置 */
    }

    timer_interrupt_flag_clear(USMART_TIMERX, TIMER_INT_FLAG_UP);           /* 清除定时器更新中断标志位 */
}

#endif
















