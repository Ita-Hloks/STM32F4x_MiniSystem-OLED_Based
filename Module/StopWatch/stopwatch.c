#include "./stopwatch.h"

#include "./BSP/TIMER/timer.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OLED/oled.h"
#include "./SYSTEM/usart/usart.h"
#include "stdbool.h"

// ──────────────────────────────────────────────────────────────────────────────
// VAR & CONST DEF
// ──────────────────────────────────────────────────────────────────────────────

static uint8_t st_h = 0;
static uint8_t st_m = 0;
static uint8_t st_s = 0;
static uint8_t st_cs = 0;
static bool stMode = false; // Control the Timer

#define ST_FONT_SIZE 16
#define CHAR_DX (ST_FONT_SIZE / 2)

// ──────────────────────────────────────────────────────────────────────────────
// Declaration: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

static void st_update_oled();
static void st_zeroing();
static void st_promotion();

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────
static void st_update_oled()
{
    uint8_t x = 20;
    oled_show_num(x, 8, st_h, 2, ST_FONT_SIZE, 0);
    x += CHAR_DX * 2;
    oled_show_string(x, ST_FONT_SIZE / 2, ":", ST_FONT_SIZE);
    x += CHAR_DX;

    oled_show_num(x, ST_FONT_SIZE / 2, st_m, 2, ST_FONT_SIZE, 0);
    x += CHAR_DX * 2;
    oled_show_string(x, ST_FONT_SIZE / 2, ":", ST_FONT_SIZE);
    x += CHAR_DX;

    oled_show_num(x, ST_FONT_SIZE / 2, st_s, 2, ST_FONT_SIZE, 0);
    x += CHAR_DX * 2;
    oled_show_string(x, ST_FONT_SIZE / 2, ":", ST_FONT_SIZE);
    x += CHAR_DX;

    oled_show_num(x, ST_FONT_SIZE / 2, st_cs, 2, ST_FONT_SIZE, 0);
    x += CHAR_DX * 2;
    oled_refresh_gram(); /* Update and display to OLED */
}

static void st_zeroing()
{
    timer_disable(TIMER1);
    st_h = 0;
    st_m = 0;
    st_s = 0;
    st_cs = 0;
    stMode = 0;
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

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: TIMMER
// ──────────────────────────────────────────────────────────────────────────────

void TIMER1_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP) == SET) /* 判断定时器更新中断是否发生 */
    {
        st_cs++;
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP); /* 清除定时器更新中断标志 */
    }
}

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: external call functions
// ──────────────────────────────────────────────────────────────────────────────

void st_init()
{
    timer1_int_init(100 - 1, 12000 - 1);
}

void st_handle_key(uint8_t key)
{
    switch (key)
    {
    case 1:
        if (stMode)
        {
            timer_disable(TIMER1);
            st_update_oled();
        }
        else
        {
            timer_enable(TIMER1);
        }
        stMode = !stMode;
        delay_ms(50);
        break;

    case 2:
        st_zeroing();
        delay_ms(50);
        break;
    }
}

void st_running()
{
    st_promotion();
    st_update_oled();
}
