#include "./clock.h"

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/RTC/rtc.h"
#include "./USMART/usmart.h"
#include "./BSP/TIMER/timer.h"

#include "stdio.h"
#include <stdbool.h>

// ──────────────────────────────────────────────────────────────────────────────
// VAR & CONST DEF
// ──────────────────────────────────────────────────────────────────────────────

#define CLOCK_FONT_SIZE 12            // DEFUALT SIZE
#define CHAR_DX (CLOCK_FONT_SIZE / 2) // 每个字符宽度（像素）

// RTC - Time
static uint8_t rtc_h;
static uint8_t rtc_m;
static uint8_t rtc_s;
static uint8_t rtc_year;
static uint8_t rtc_month;
static uint8_t rtc_day;
static uint8_t rtc_week;
static uint8_t clock_ampm;
static uint8_t weekdays[7][4] = {
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

// Defines the order of editable fields
enum
{
    EDIT_IDX_HOUR = 0,
    EDIT_IDX_MIN, // 隐式等于 1
    EDIT_IDX_SEC,
    EDIT_IDX_YEAR,
    EDIT_IDX_MONTH,
    EDIT_IDX_DAY,
    EDIT_IDX_WEEK,
    EDIT_IDX_COUNT // count the total num of enum
};

// EDIT MODE save (clock_s use the rtc!)
uint8_t clockEditMode = 0;
uint8_t clockEdit_h;
uint8_t clockEdit_m;
uint8_t clockEdit_year;
uint8_t clockEdit_month;
uint8_t clockEdit_day;
uint8_t clockEdit_week;

// Set And Get
static int8_t curPosIndex = -1; // -1 : NULL >=0: Point to clock_?
static uint8_t *curPos = NULL;

static uint8_t *curVars[EDIT_IDX_COUNT];

// Blink
bool isBlink = false;
bool blinkTimerRunning = false;

// ──────────────────────────────────────────────────────────────────────────────
// Declaration: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

static void clock_update_cur_vars();
static inline void clock_choose_var(int8_t idx);
static inline uint8_t get_disp_val(uint8_t edit_val, uint8_t rtc_val);
static void clock_display_time12();
static void clock_curvar_add();
static void clock_curvar_sub();
static inline void clock_blink_pause();

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

/**
 * @brief      Init the curVars arrays，Make it point to the corresponding edit variable
 */
static void clock_update_cur_vars()
{
    curVars[EDIT_IDX_HOUR] = &clockEdit_h;
    curVars[EDIT_IDX_MIN] = &clockEdit_m;
    curVars[EDIT_IDX_SEC] = &rtc_s;
    curVars[EDIT_IDX_YEAR] = &clockEdit_year;
    curVars[EDIT_IDX_MONTH] = &clockEdit_month;
    curVars[EDIT_IDX_DAY] = &clockEdit_day;
    curVars[EDIT_IDX_WEEK] = &clockEdit_week;
}

/**
 * @brief      实现闪烁
 *
 * @param x         起始 X 坐标
 * @param y         起始 Y 坐标
 * @param value     要显示的数值
 * @param digits    位宽
 * @param blink_en  blink_en是否开启的监测
 *
 * @return          本次绘制占用的宽度 = digits * CHAR_DX
 */
static uint8_t draw_blink_num(uint8_t x, uint8_t y, uint16_t value, uint8_t digits, bool blink_en)
{
    if (blink_en && isBlink)
    {
        // 空白
        for (uint8_t i = 0; i < digits; i++)
        {
            oled_show_string(x + i * CHAR_DX, y, " ", CLOCK_FONT_SIZE);
        }
    }
    else
    {
        oled_show_num(x, y, value, digits, CLOCK_FONT_SIZE, 1);
    }
    return digits * CHAR_DX;
}

/**
 * @brief  用 12 号字体分两行显示：“YY-MM-DD-W”，“HH:MM:SS”
 */
static void clock_display_time12()
{
    uint8_t x = 25;
    uint8_t y_date = 0;
    uint8_t y_time = 15;

    uint8_t disp_year = get_disp_val(clockEdit_year, rtc_year);
    uint8_t disp_month = get_disp_val(clockEdit_month, rtc_month);
    uint8_t disp_day = get_disp_val(clockEdit_day, rtc_day);
    uint8_t disp_week = get_disp_val(clockEdit_week, rtc_week);

    uint8_t disp_h = get_disp_val(clockEdit_h, rtc_h);
    uint8_t disp_m = get_disp_val(clockEdit_m, rtc_m);
    uint8_t disp_s = rtc_s; // 秒始终取 RTC

    // Determine if field needs to blink
    bool blink_hour = (curPosIndex == EDIT_IDX_HOUR);
    bool blink_min = (curPosIndex == EDIT_IDX_MIN);
    bool blink_sec = (curPosIndex == EDIT_IDX_SEC);
    bool blink_yearB = (curPosIndex == EDIT_IDX_YEAR);
    bool blink_month = (curPosIndex == EDIT_IDX_MONTH);
    bool blink_day = (curPosIndex == EDIT_IDX_DAY);
    bool blink_week = (curPosIndex == EDIT_IDX_WEEK);

    // LINE1 YY-MM-DD
    oled_show_num(x, y_date, 20, 2, 12, 0);
    x += CHAR_DX * 2;
    x += draw_blink_num(x, y_date, disp_year, 2, blink_yearB);
    oled_show_string(x, y_date, "-", CLOCK_FONT_SIZE);
    x += CHAR_DX;

    x += draw_blink_num(x, y_date, disp_month, 2, blink_month);
    oled_show_string(x, y_date, "-", CLOCK_FONT_SIZE);
    x += CHAR_DX;

    x += draw_blink_num(x, y_date, disp_day, 2, blink_day);
    x += CHAR_DX; // ADD Blank

    // Display Week
    if (blink_week && isBlink)
    {
        oled_show_string(x, y_date, "   ", CLOCK_FONT_SIZE); // 显示空白，实现闪烁效果
    }
    else
    {
        oled_show_string(x, y_date, (char *)weekdays[disp_week], CLOCK_FONT_SIZE);
    }

    // LINE2 HH-MM-SS
    x = 35;
    x += draw_blink_num(x, y_time, disp_h, 2, blink_hour);
    oled_show_string(x, y_time, ":", CLOCK_FONT_SIZE);
    x += CHAR_DX;

    x += draw_blink_num(x, y_time, disp_m, 2, blink_min);
    oled_show_string(x, y_time, ":", CLOCK_FONT_SIZE);
    x += CHAR_DX;

    x += draw_blink_num(x, y_time, disp_s, 2, blink_sec);

    oled_refresh_gram();
}

/**
 * @brief      根据当前 clockEditMode 返回要显示的数值
 * @param edit_val 编辑缓存里的值
 * @param rtc_val  RTC 实时读取的值
 * @return     如果在编辑模式则返回 edit_val，否则返回 rtc_val
 */
static inline uint8_t get_disp_val(uint8_t edit_val, uint8_t rtc_val)
{
    return clockEditMode ? edit_val : rtc_val;
}

static inline void clock_choose_var(int8_t idx)
{
    if (idx >= 0 && idx < EDIT_IDX_COUNT)
    {
        curPosIndex = idx;
        curPos = curVars[idx];
    }
    else
    {
        curPosIndex = -1;
        curPos = NULL;
    }
}

static void clock_curvar_add()
{
    if (curPos == NULL) return;

    switch (curPosIndex)
    {
    case EDIT_IDX_HOUR:
        (*curPos) = ((*curPos) + 1) % 24;
        break;

    case EDIT_IDX_MIN:
        (*curPos) = ((*curPos) + 1) % 60;
        break;

    case EDIT_IDX_SEC: // SEC need ALWALS update let user know the accurate time
        (*curPos) = ((*curPos) + 1) % 60;
        rtc_set_time(clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
        break;

    case EDIT_IDX_YEAR:
        (*curPos) = ((*curPos) + 1) % 100;
        break;

    case EDIT_IDX_MONTH:
        (*curPos) = ((*curPos) == 12) ? 1 : (*curPos) + 1;
        break;

    case EDIT_IDX_DAY:
        (*curPos) = ((*curPos) == 31) ? 1 : (*curPos) + 1;
        break;

    case EDIT_IDX_WEEK:
        (*curPos) = ((*curPos) + 1) % 7;
        break;

    default:
        break;
    }
}

static void clock_curvar_sub()
{
    if (curPos == NULL) return;

    switch (curPosIndex)
    {
    case EDIT_IDX_HOUR:
        (*curPos) = ((*curPos) == 0) ? 23 : (*curPos) - 1;
        break;

    case EDIT_IDX_MIN:
        (*curPos) = ((*curPos) == 0) ? 59 : (*curPos) - 1;
        break;

    case EDIT_IDX_SEC:
        (*curPos) = ((*curPos) == 0) ? 59 : (*curPos) - 1;
        rtc_set_time(clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
        break;

    case EDIT_IDX_YEAR:
        (*curPos) = ((*curPos) == 0) ? 99 : (*curPos) - 1;
        break;

    case EDIT_IDX_MONTH:
        (*curPos) = ((*curPos) == 1) ? 12 : (*curPos) - 1;
        break;

    case EDIT_IDX_DAY:
        (*curPos) = ((*curPos) == 1) ? 31 : (*curPos) - 1;
        break;

    case EDIT_IDX_WEEK:
        (*curPos) = ((*curPos) == 0) ? 6 : (*curPos) - 1;
        break;

    default:
        break;
    }
}

static inline void clock_blink_pause()
{
    timer_disable(TIMERX_INT);
    blinkTimerRunning = false;
    isBlink = false;
}

// void clock_JudgeDisplayMode()
// {
//     if ((clock_getTime_count % 10) == 0) /* 每100ms更新一次显示数据 */
//     {
//         rtc_get_time(&clock_h, &clock_m, &clock_s, &clock_ampm);
//     }
//     switch (display_mode)
//     {
//     case 0:
//         clock_DisplaySize_12();
//         break;
//     case 1:
//         clock_DisplaySize_16();
//         break;
//     case 2:
//         clock_DisplaySize_24();
//         break;
//     }
// }

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: TIMMER
// ──────────────────────────────────────────────────────────────────────────────
void TIMERX_INT_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMERX_INT, TIMER_INT_FLAG_UP) == SET) /* 判断定时器更新中断是否发生 */
    {
        isBlink = !isBlink;
        timer_interrupt_flag_clear(TIMERX_INT, TIMER_INT_FLAG_UP); /* 清除定时器更新中断标志 */
    }
}

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: external call functions
// ──────────────────────────────────────────────────────────────────────────────

void clock_init()
{
    // Loding the curVar[]
    clock_update_cur_vars();

    // Init the Timer (Defualt: OFF)
    timerx_int_init(5000 - 1, 12000 - 1);

    // Get the RTC time and date
    rtc_get_time(&rtc_h, &rtc_m, &rtc_s, &clock_ampm);
    rtc_get_date(&rtc_year, &rtc_month, &rtc_day, &rtc_week);

    // Set curPos point to NULL
    curPosIndex = -1;
    curPos = NULL;
    clockEditMode = 0;
}

void clock_handle_key(uint8_t key)
{
    switch (key)
    {
    case 2:
    {
        int8_t next = curPosIndex + 1;
        // switch to the next index
        if (next >= EDIT_IDX_COUNT)
        {
            // Pass the EDIT_IDX_COUNT! EXIT Edit mode
            timer_disable(TIMERX_INT);
            blinkTimerRunning = false;
            next = -1;
        }

        if (next == -1)
        {
            // point to NULL
            clockEditMode = 0;
            // Save data
            rtc_set_time(clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
            rtc_set_date(clockEdit_year, clockEdit_month, clockEdit_day, clockEdit_week);
        }
        else
        {
            if (!blinkTimerRunning)
            {
                timer_enable(TIMERX_INT);
                blinkTimerRunning = true;
            }
            // Enter the edit mode. Cache RTC values
            if (curPosIndex == -1)
            {
                clockEditMode = 1;
                clockEdit_h = rtc_h;
                clockEdit_m = rtc_m;
                clockEdit_year = rtc_year;
                clockEdit_month = rtc_month;
                clockEdit_day = rtc_day;
                clockEdit_week = rtc_week;
            }
        }
        // Update curPosIndex and curPos
        clock_choose_var(next);
        delay_ms(150);
        break;
    }

    case 3: // ADD
        clock_curvar_add();
        clock_blink_pause();
        delay_ms(150);
        break;

    case 4: // SUB
        clock_curvar_sub();
        clock_blink_pause();
        delay_ms(150);
        break;
        
    case 0: // key is not pressed...
        // Recover the edit mode normal situtation.
        if (curPosIndex != -1 && blinkTimerRunning == false)
        {
            blinkTimerRunning = true;
            timer_enable(TIMERX_INT);
        }
        break;
    }
}

/**
 * @brief   Runing function, keep the clock read the correct data
 */
void clock_runing()
{
    static uint8_t clock_count_upt = 0; /* variable of the RTC to Update the time */
    if ((clock_count_upt % 10) == 0)
    {
        rtc_get_time(&rtc_h, &rtc_m, &rtc_s, &clock_ampm);
        rtc_get_date(&rtc_year, &rtc_month, &rtc_day, &rtc_week);
    }
    clock_count_upt++;
    clock_display_time12();
}

void clock_exit() {
    clockEditMode = 0;
    curPosIndex = -1;
}