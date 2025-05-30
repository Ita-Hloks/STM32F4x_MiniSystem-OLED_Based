#include "./clock.h"

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/RTC/rtc.h"
#include "./USMART/usmart.h"
#include "./BSP/TIMER/timer.h"

// static uint8_t display_mode = 1;  /* 3 types display size 12, 16, 24 correspond to 0, 1, 2 */

// Clock - Time
static uint8_t rtc_h;
static uint8_t rtc_m;
static uint8_t rtc_s;
static uint8_t clock_ampm;

// EDIT MODE save
static uint8_t clockEditMode = 0; // 编辑时间模式
static uint8_t clockEdit_h;
static uint8_t clockEdit_m;

// Set And Get  
static uint8_t curPosIndex = 0; /* 0: null 1: hour 2: minus 3: secound */
static uint8_t *curPos;          /* Pointer to the current editing variable h/m/s */

// Display
static uint8_t display_h;
static uint8_t display_m;
static uint8_t countGetTime = 0; /* variable of the RTC to GET the time */
static bool isBlink = false;

/****************************** STATIC FUNCTION START ***********************************/
static void clock_blink_num(uint8_t x, uint8_t y, uint8_t value, bool blinkCondition, uint8_t size, uint8_t mode) {
    if (blinkCondition && isBlink) {
        oled_show_string(x, y, "  ", size);
    } else {
        oled_show_num(x, y, value, 2, size, mode);
    }
}

static void clock_display_time16()
{
    static uint8_t size = 16;

    display_h = clockEditMode ? clockEdit_h : rtc_h;
    display_m = clockEditMode ? clockEdit_m : rtc_m;
    uint8_t display_s = rtc_s;

    clock_blink_num(0, size / 2, display_h, curPos == &clockEdit_h, size, 1);
    oled_show_string(0 + 16, size / 2, ":", size);

    clock_blink_num(16 + 8, size / 2, display_m, curPos == &clockEdit_m, size, 1);
    oled_show_string(24 + 16, size / 2, ":", size);

    clock_blink_num(40 + 8, size / 2, display_s, curPos == &rtc_s, size, 0);
    
    oled_refresh_gram();
}


/**
 * @brief Update curPos to point to the correct variable (rtc_h, rtc_m, rtc_s)
 *
 * @param i 1: hour, 2: minute, 3: second
 */
static void clock_choose_var(uint8_t i)
{
    switch (i)
    {
    case 1:
        curPos = &clockEdit_h;
        break;
    case 2:
        curPos = &clockEdit_m;
        break;
    case 3:
        curPos = &rtc_s;
        break;
    default:
        curPos = NULL; // No selection
        break;
    }
}

static void clock_curvar_add(uint8_t *pos)
{
    if (pos == NULL)
        return; // Safety check

    if (pos == &clockEdit_h)
    {
        if (++(*pos) == 24)
            *pos = 0; // 0-23
    }
    else if (pos == &clockEdit_m || pos == &rtc_s)
    {
        if (++(*pos) == 60)
            *pos = 0; // 0-59
        if (pos == &rtc_s)
        {
            rtc_set_time(clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
        }
    }
    oled_refresh_gram();
}

static void clock_curvar_sub(uint8_t *pos)
{
    if (pos == NULL)
        return; // Safety check

    if (pos == &clockEdit_h)
    {
        if (*pos == 0)
            *pos = 23;
        else
            (*pos)--;
    }

    else if (pos == &clockEdit_m || pos == &rtc_s)
    {
        *pos == 0 ? *pos = 59 : (*pos)--;
        if (pos == &rtc_s)
        {
            rtc_set_time(clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
        }
    }
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
/******************************** STATIC FUNCTION END ***********************************/
/************************************ TIMER START ***************************************/
/**
 * @brief       定时器TIMERX中断服务函数
 * @param       无
 * @retval      无
 */
void TIMERX_INT_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMERX_INT, TIMER_INT_FLAG_UP) == SET)     /* 判断定时器更新中断是否发生 */
    {
        isBlink = !isBlink;
        timer_interrupt_flag_clear(TIMERX_INT, TIMER_INT_FLAG_UP);          /* 清除定时器更新中断标志 */
    }
}
/************************************ TIMER END *****************************************/

void clock_init()
{
    rtc_get_time(&rtc_h, &rtc_m, &rtc_s, &clock_ampm);
    timerx_int_init(5000 - 1, 12000 - 1);
    timer_disable(TIMERX_INT);
}

void clock_handle_key(uint8_t key)
{

    switch (key)
    {
    // case KEY1_PRES: // Switch display size (12/16/24)
    //     delay_ms(200);
    //     break;
    case 2: // Switch editing position (h/m/s)
        if (++curPosIndex == 4)
            curPosIndex = 0;

        if (curPosIndex == 1)
        { // entering edit mode
            clockEditMode = 1;
            timer_enable(TIMERX_INT);
            clockEdit_h = rtc_h;
            clockEdit_m = rtc_m;
        } else if (curPosIndex == 0)
        {
            rtc_set_time(clockEdit_h, clockEdit_m, rtc_s, clock_ampm);
            clockEditMode = 0;
            timer_disable(TIMERX_INT);
        }
        clock_choose_var(curPosIndex);
        oled_clear();
        clock_display_time16();
        delay_ms(200);
        break;

    case 3: // Add
        clock_curvar_add(curPos);
        delay_ms(100);
        break;

    case 4: // Subtraction
        clock_curvar_sub(curPos);
        delay_ms(100);
        break;
    }
}

void clock_runing()
{

    countGetTime++;
    if ((countGetTime % 2) == 0) /* 每100ms更新一次显示数据 */
    {
        rtc_get_time(&rtc_h, &rtc_m, &rtc_s, &clock_ampm);
    }
    clock_display_time16();
}