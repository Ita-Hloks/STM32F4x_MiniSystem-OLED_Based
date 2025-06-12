#include "./clock.h"
#include "../Module/AppFlash/uflash.h"

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

// KEY
#define KEY_SWITCH_DSP_MODE 1
#define KEY_TIME_SET 2
#define KEY_ADD 3
#define KEY_SUB 4
#define KEY_ALARMA_SET 5

// RTC - Time
static uint8_t rtc_h;
static uint8_t rtc_m;
static uint8_t rtc_s;
static uint8_t rtc_year;
static uint8_t rtc_month;
static uint8_t rtc_day;
static uint8_t rtc_week;
static uint8_t clock_ampm;
static uint8_t weekdays[8][4] = {
    "-No", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

// Defines the order of editable fields
enum
{
    T_EDIT_IDX_HOUR = 0,
    T_EDIT_IDX_MIN, // 隐式等于 1
    T_EDIT_IDX_SEC,
    T_EDIT_IDX_WEEK,
    T_EDIT_IDX_YEAR,
    T_EDIT_IDX_MONTH,
    T_EDIT_IDX_DAY,
    T_EDIT_IDX_COUNT // count the total num of enum
};

enum
{
    A_EDIT_IDX_HOUR = 0,
    A_EDIT_IDX_MIN,
    A_EDIT_IDX_SEC,
    A_EDIT_IDX_WEEK,
    A_EDIT_IDX_COUNT,
};

// EDIT MODE save (clock_s use the rtc!)
uint8_t clockEditMode = 0;
uint8_t clockEdit_h;
uint8_t clockEdit_m;
uint8_t clockEdit_year;
uint8_t clockEdit_month;
uint8_t clockEdit_day;
uint8_t clockEdit_week;

// MODE
#define DSP_CLOCK 0
#define DSP_ALARM 1
uint8_t dspMode = 0; // 0: Clock 1: Alarm

// Alarm
uint8_t alarm_h;
uint8_t alarm_m;
uint8_t alarm_s;
uint8_t alarm_w;

// Set And Get
static int8_t curPosIndex = -1; // -1 : NULL >=0: Point to clock_?
static uint8_t *curPos = NULL;

static uint8_t *curVarsT[T_EDIT_IDX_COUNT];
static uint8_t *curVarsA[A_EDIT_IDX_COUNT];

// Blink
bool isBlink = false;
bool blinkTimerRunning = false;

// ──────────────────────────────────────────────────────────────────────────────
// Declaration: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

// CLOCK
static void clock_update_cur_vars();
static inline void clock_choose_var(int8_t idx);
static inline uint8_t get_disp_val(uint8_t edit_val, uint8_t rtc_val);
static void clock_display_time12();
static void clock_curvar_add();
static void clock_curvar_sub();
static inline void clock_blink_pause();

// Alarm

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

/**
 * @brief      Init the curVarsT arrays，Make it point to the corresponding edit variable
 */
static void clock_update_cur_vars()
{
    // CLOKC
    curVarsT[T_EDIT_IDX_HOUR] = &clockEdit_h;
    curVarsT[T_EDIT_IDX_MIN] = &clockEdit_m;
    curVarsT[T_EDIT_IDX_SEC] = &rtc_s;
    curVarsT[T_EDIT_IDX_YEAR] = &clockEdit_year;
    curVarsT[T_EDIT_IDX_MONTH] = &clockEdit_month;
    curVarsT[T_EDIT_IDX_DAY] = &clockEdit_day;
    curVarsT[T_EDIT_IDX_WEEK] = &clockEdit_week;

    // Alarm
    curVarsA[A_EDIT_IDX_HOUR] = &alarm_h;
    curVarsA[A_EDIT_IDX_MIN] = &alarm_m;
    curVarsA[A_EDIT_IDX_SEC] = &alarm_s;
    curVarsA[A_EDIT_IDX_WEEK] = &alarm_w;  // 添加闹钟星期的指针
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
 * @brief  用 12 号字体分两行显示："YY-MM-DD-W"，"HH:MM:SS"
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
    bool blink_hour = (curPosIndex == T_EDIT_IDX_HOUR);
    bool blink_min = (curPosIndex == T_EDIT_IDX_MIN);
    bool blink_sec = (curPosIndex == T_EDIT_IDX_SEC);
    bool blink_yearB = (curPosIndex == T_EDIT_IDX_YEAR);
    bool blink_month = (curPosIndex == T_EDIT_IDX_MONTH);
    bool blink_day = (curPosIndex == T_EDIT_IDX_DAY);
    bool blink_week = (curPosIndex == T_EDIT_IDX_WEEK);

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
    if (dspMode == DSP_CLOCK)
    {
        if (idx >= 0 && idx < T_EDIT_IDX_COUNT)
        {
            curPosIndex = idx;
            curPos = curVarsT[idx];
        }
        else
        {
            curPosIndex = -1;
            curPos = NULL;
        }
    }
    else if (dspMode == DSP_ALARM)
    {
        if (idx >= 0 && idx < A_EDIT_IDX_COUNT)
        {
            curPosIndex = idx;
            curPos = curVarsA[idx];
        }
        else
        {
            curPosIndex = -1;
            curPos = NULL;
        }
    }
}

static void clock_curvar_add()
{
    if (curPos == NULL)
        return;

    switch (curPosIndex)
    {
    case T_EDIT_IDX_HOUR:
        *curPos = ((*curPos) + 1) % 24;
        break;

    case T_EDIT_IDX_MIN:
        *curPos = ((*curPos) + 1) % 60;
        break;

    case T_EDIT_IDX_SEC: // SEC need ALWALS update let user know the accurate time
        *curPos = ((*curPos) + 1) % 60;
        break;

    case T_EDIT_IDX_YEAR:
        *curPos = ((*curPos) + 1) % 100;
        break;

    case T_EDIT_IDX_MONTH:
        *curPos = ((*curPos) == 12) ? 1 : (*curPos) + 1;
        break;

    case T_EDIT_IDX_DAY:
        *curPos = ((*curPos) == 31) ? 1 : (*curPos) + 1;
        break;

    case T_EDIT_IDX_WEEK:
        *curPos = (*curPos % 7) + 1;
        break;

    default:
        break;
    }
}

static void clock_curvar_sub()
{
    if (curPos == NULL)
        return;

    switch (curPosIndex)
    {
    case T_EDIT_IDX_HOUR:
        *curPos = ((*curPos) == 0) ? 23 : (*curPos) - 1;
        break;

    case T_EDIT_IDX_MIN:
        *curPos = ((*curPos) == 0) ? 59 : (*curPos) - 1;
        break;

    case T_EDIT_IDX_SEC:
        *curPos = ((*curPos) == 0) ? 59 : (*curPos) - 1;
        rtc_set_time(clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
        break;

    case T_EDIT_IDX_YEAR:
        *curPos = ((*curPos) == 0) ? 99 : (*curPos) - 1;
        break;

    case T_EDIT_IDX_MONTH:
        *curPos = ((*curPos) == 1) ? 12 : (*curPos) - 1;
        break;

    case T_EDIT_IDX_DAY:
        *curPos = ((*curPos) == 1) ? 31 : (*curPos) - 1;
        break;

    case T_EDIT_IDX_WEEK:
        *curPos = (*curPos - 2 + 7) % 7 + 1;
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

// Alarm
static void alarm_display_time12()
{
    uint8_t x = 10;
    uint8_t y_time = 15;

    uint8_t disp_week = alarm_w;

    uint8_t disp_h = alarm_h;
    uint8_t disp_m = alarm_m;
    uint8_t disp_s = alarm_s;

    // Determine if field needs to blink
    bool blink_hour = (curPosIndex == A_EDIT_IDX_HOUR);
    bool blink_min = (curPosIndex == A_EDIT_IDX_MIN);
    bool blink_sec = (curPosIndex == A_EDIT_IDX_SEC);
    bool blink_week = (curPosIndex == A_EDIT_IDX_WEEK);

    oled_show_string(20, 0, "ALARM", 12);

    // Display Week
    if (blink_week && isBlink)
    {
        oled_show_string(x, y_time, "   ", CLOCK_FONT_SIZE); // 显示空白，实现闪烁效果
    }
    else
    {
        oled_show_string(x, y_time, (char *)weekdays[disp_week], CLOCK_FONT_SIZE);
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

    // 初始化 Flash 并加载闹钟数据
    if (fls_init() != FLS_OK) {
        while(1); // 如果初始化失败，进入死循环
    }
    clock_load_alarm();
}

void clock_handle_key(uint8_t key)
{
    switch (key)
    {
    case KEY_SWITCH_DSP_MODE:
        oled_clear();
        clockEditMode = 0;
        curPosIndex = -1;
        dspMode = (dspMode + 1) % 2;
        delay_ms(150);
        break;

    case KEY_TIME_SET:
    {
        int8_t next = curPosIndex + 1;
        // switch to the next index

        if (dspMode == DSP_CLOCK)
        {
            if (next >= T_EDIT_IDX_COUNT)
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
                printf("Save RTC Success!\r\n");
                printf("H:%d M:%d S:%d AMP:%d\r\n", clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
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
        } else if (dspMode == DSP_ALARM) {
            if (next >= A_EDIT_IDX_COUNT)
            {
                // Pass the EDIT_IDX_COUNT! EXIT Edit mode
                timer_disable(TIMERX_INT);
                blinkTimerRunning = false;
                next = -1;
            }

            if (next == -1)
            {
                // 保存闹钟数据到 Flash
                if (clock_save_alarm() != FLS_OK) {
                    // 保存失败处理
                    oled_show_string(10, 30, "Save Failed!", 12);
                    delay_ms(1000);
                }
                // 设置 RTC 闹钟
                rtc_set_alarma(alarm_h, alarm_h, alarm_s, alarm_w);
                printf("Save ALARM Success!\r\n");
                printf("H:%d M:%d S:%d AMP:%d\r\n", alarm_h, alarm_h, alarm_s, alarm_w);
            }
            else
            {
                if (!blinkTimerRunning)
                {
                    timer_enable(TIMERX_INT);
                    blinkTimerRunning = true;
                }
                // Enter the edit mode. Cache RTC values
            }
        }

        // Update curPosIndex and curPos
        clock_choose_var(next);
        delay_ms(150);
        break;
    }

    case KEY_ADD:
        clock_curvar_add();
        clock_blink_pause();
        delay_ms(150);
        break;

    case KEY_SUB:
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
    if (dspMode == DSP_CLOCK)
    {
        clock_display_time12();
    }
    else if (dspMode == DSP_ALARM)
    {
        alarm_display_time12();
    }
    else
    {
        oled_show_string(10, 15, "Opps, The dsp index ERR!", 12);
    }
}

void clock_exit()
{
    clockEditMode = 0;
    curPosIndex = -1;
}

// 新增：保存闹钟数据到 Flash
FLS_Status clock_save_alarm(void) {
    AlarmData alarmData = {
        .magic = CLOCK_MAGIC_NUMBER,
        .hour = alarm_h,
        .minute = alarm_m,
        .second = alarm_s,
        .weekday = alarm_w
    };
    
    FLS_Status status = fls_erase_sector(ALARM_FLASH_ADDR);
    if (status != FLS_OK) {
        return status;
    }
    
    return fls_write(ALARM_FLASH_ADDR, (uint8_t*)&alarmData, sizeof(AlarmData));
}

// 新增：从 Flash 加载闹钟数据
FLS_Status clock_load_alarm(void) {
    AlarmData alarmData;
    
    // 读取数据
    FLS_Status status = fls_read(ALARM_FLASH_ADDR, (uint8_t*)&alarmData, sizeof(AlarmData));
    if (status != FLS_OK) {
        return status;
    }
    
    if (alarmData.magic != CLOCK_MAGIC_NUMBER) {
        // 数据无效，使用默认值
        alarm_h = 8;
        alarm_m = 29;
        alarm_s = 50;
        alarm_w = 4;
        return FLS_OK;
    }
    
    // 加载数据
    alarm_h = alarmData.hour;
    alarm_m = alarmData.minute;
    alarm_s = alarmData.second;
    alarm_w = alarmData.weekday;
    
    return FLS_OK;
}