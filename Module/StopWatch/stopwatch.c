#include "./stopwatch.h"
#include "./BSP/TIMER/timer.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OLED/oled.h"
#include "./SYSTEM/usart/usart.h"

#include "stdbool.h"

// ST
static uint8_t st_h = 0;
static uint8_t st_m = 0;
static uint8_t st_s = 0;
static uint8_t st_cs = 0;
static bool stMode = false; // Control the Timer

static void st_update_oled() {
    static uint8_t size = 16; /* Select Only 12/16/24 */
    oled_show_num(0, size / 2, st_h, 2, size, 0);
    oled_show_string(0 + 16, size / 2, ":", size);
    oled_show_num(16 + 8, size / 2, st_m, 2, size, 0);
    oled_show_string(24 + 16, size / 2, ":", size);
    oled_show_num(40 + 8, size / 2, st_s, 2, size, 0);    
    oled_show_string(48 + 16, size / 2, ":", size);
    oled_show_num(64 + 8, size / 2, st_cs, 2, size, 0);
    oled_refresh_gram(); /* Update and display to OLED */
}

static void st_zeroing() {
    st_h = 0;
    st_m = 0;
    st_s = 0;
    st_cs = 0;
    stMode = 0;
    timer_disable(TIMERX_INT);
    oled_refresh_gram();
}

static void st_promotion()
{
    if (st_cs >= 100)
    {
        st_cs = 0;
        st_s++;
        if (st_s >= 60) // 秒进位到分
        {
            st_s = 0;
            st_m++;

            if (st_m >= 60) // 分进位到时
            {
                st_m = 0;
                st_h++;

                if (st_h >= 24) // 时进位到24小时制循环
                {
                    st_h = 0;
                }
            }
        }
    }
}

/************************************* TIMER SET ****************************************/
void TIMER1_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP) == SET)     /* 判断定时器更新中断是否发生 */
    {
        st_cs++;
        st_promotion();
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);          /* 清除定时器更新中断标志 */
    }
}
/************************************* TIMER SET ****************************************/

void st_init() {
    timer1_int_init(100 - 1, 12000 - 1);
}

void st_handle_key(uint8_t key) {
    switch (key)
    {
    case 1:
        if (stMode) {
            timer_disable(TIMER1);
            st_update_oled();
        } 
        else {
            timer_enable(TIMER1);
        }
        stMode = !stMode;
        printf("%d", stMode);
        break;

    case 2:
        st_zeroing();
        break;
    }
    delay_ms(200);
}

void st_running() {
    // if (stMode) {
        st_update_oled();
    // }
}
