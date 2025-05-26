#include "./clock.h"

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/RTC/rtc.h"
#include "./USMART/usmart.h"

static uint8_t display_mode = 1;  /* 3 types display size 12, 16, 24 correspond to 0, 1, 2 */

/****************************** STATIC FUNCTION START ***********************************/

static void clock_DisplaySize_12()
{
    static uint8_t size = 12; /* Select Only 12/16/24 */
    if (cur_pos == &clock_h || cur_pos == NULL)
        oled_show_num(0, size / 2, clock_h, 2, size, 0);
    oled_show_string(0 + 12, size / 2, ":", size);
    if (cur_pos == &clock_m || cur_pos == NULL)
        oled_show_num(12 + 6, size / 2, clock_m, 2, size, 0);
    oled_show_string(18 + 12, size / 2, ":", size);
    if (cur_pos == &clock_s || cur_pos == NULL)
        oled_show_num(30 + 6, size / 2, clock_s, 2, size, 0);
    oled_refresh_gram(); /* Update and display to OLED */
    delay_ms(100);
}

static void clock_DisplaySize_16()
{
    static uint8_t size = 16; /* Select Only 12/16/24 */
    clock_getTime_count++;

    if (cur_pos == &clock_h || cur_pos == NULL)
    {
        oled_show_num(0, size / 2, clock_h, 2, size, 0);
        oled_show_string(0 + 16, size / 2, ":", size);
    }
    if (cur_pos == &clock_m || cur_pos == NULL)
    {
        oled_show_num(16 + 8, size / 2, clock_m, 2, size, 0);
        oled_show_string(24 + 16, size / 2, ":", size);
    }
    if (cur_pos == &clock_s || cur_pos == NULL)
    {
        oled_show_num(40 + 8, size / 2, clock_s, 2, size, 0);
    }
    oled_refresh_gram(); /* Update and display to OLED */
    delay_ms(100);
}

static void clock_DisplaySize_24()
{
    static uint8_t size = 24; /* Select Only 12/16/24 */

    if (cur_pos == &clock_h || cur_pos == NULL)
        oled_show_num(0, 8, clock_h, 2, size, 0);
    oled_show_string(0 + 24, 8, ":", size);
    if (cur_pos == &clock_m || cur_pos == NULL)
        oled_show_num(24 + 12, 8, clock_m, 2, size, 0);
    oled_show_string(36 + 24, 8, ":", size);
    if (cur_pos == &clock_s || cur_pos == NULL)
        oled_show_num(60 + 12, 8, clock_s, 2, size, 0);
    oled_refresh_gram(); /* Update and display to OLED */
    delay_ms(100);
}

/**
 * @brief Update cur_pos to point to the correct variable (clock_h, clock_m, clock_s)
 *
 * @param i 1: hour, 2: minute, 3: second
 */
static void choose_pos(uint8_t i)
{
    switch (i)
    {
    case 1:
        cur_pos = &clock_h;
        break;
    case 2:
        cur_pos = &clock_m;
        break;
    case 3:
        cur_pos = &clock_s;
        break;
    default:
        cur_pos = NULL; // No selection
        break;
    }
}

static void cur_pos_add(uint8_t *pos)
{
    if (pos == NULL)
        return; // Safety check

    if (pos == &clock_h)
    {
        if (++(*pos) == 24)
            *pos = 0; // 0-23
    }
    else if (pos == &clock_m || pos == &clock_s)
    {
        if (++(*pos) == 60)
            *pos = 0; // 0-59
    }
}

static void cur_pos_sub(uint8_t *pos)
{
    if (pos == NULL)
        return; // Safety check

    if (pos == &clock_h)
    {
        if (*pos == 0)
            *pos = 23;
        else
            (*pos)--;
    }
    else if (pos == &clock_m || pos == &clock_s)
    {
        if (*pos == 0)
            *pos = 59;
        else
            (*pos)--;
    }
}

/******************************** STATIC FUNCTION END ***********************************/

void clock_JudgeDisplayMode()
{
    if ((clock_getTime_count % 10) == 0) /* 每100ms更新一次显示数据 */
    {
        rtc_get_time(&clock_h, &clock_m, &clock_s, &clock_ampm);
    }

    switch (display_mode)
    {
    case 0:
        clock_DisplaySize_12();
        break;
    case 1:
        clock_DisplaySize_16();
        break;
    case 2:
        clock_DisplaySize_24();
        break;
    }
}

void clock_HandleKeyPress(uint8_t key) {
    switch (key)
    {
    // case 1: // Switch display size (12/16/24)
    //     if (++display_mode == 3)
    //         display_mode = 0;
    //     delay_ms(200);
    //     break;

    case 2: // Add
        cur_pos_add(cur_pos);
        rtc_set_time(clock_h, clock_m, clock_s, clock_ampm);
        break;

    case 3: // Subtraction
        cur_pos_sub(cur_pos);
        rtc_set_time(clock_h, clock_m, clock_s, clock_ampm);
        break;

    case 4: // Switch editing position (h/m/s)
        oled_clear();
        if (++cur_pos_index == 4)
            cur_pos_index = 0;
        choose_pos(cur_pos_index);
        // printf("%s%d\r\n", "Current Value:", cur_pos_index);
        // printf("%s%d\r\n", "[DBG] cur_pos = &clock_h ? ", cur_pos == &clock_h);
        // printf("%s%d\r\n", "[DBG] cur_pos = &clock_m ? ", cur_pos == &clock_m);
        // printf("%s%d\r\n", "[DBG] cur_pos = &clock_s ? ", cur_pos == &clock_s);
        break;
    }
}


void clock_CurVar_Add(uint8_t *pos)
{
    if (pos == NULL)
        return; // Safety check

    if (pos == &clock_h)
    {
        if (++(*pos) == 24)
            *pos = 0; // 0-23
    }
    else if (pos == &clock_m || pos == &clock_s)
    {
        if (++(*pos) == 60)
            *pos = 0; // 0-59
    }
}

void clock_CurVar_Sub(uint8_t *pos)
{
    if (pos == NULL)
        return; // Safety check

    if (pos == &clock_h)
    {
        if (*pos == 0)
            *pos = 23;
        else
            (*pos)--;
    }
    else if (pos == &clock_m || pos == &clock_s)
    {
        if (*pos == 0)
            *pos = 59;
        else
            (*pos)--;
    }
}
