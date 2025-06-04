#include "./adcc.h"

#include "./BSP/ADC/adc.h"
#include "./BSP/OLED/oled.h"

// ──────────────────────────────────────────────────────────────────────────────
// VAR & CONST DEF
// ──────────────────────────────────────────────────────────────────────────────

static float adcx;
static float temp;
static uint8_t displayMode = 0; // 0: 数字模式 1:可视化图标 2:混合模式
// static uint8_t adk_ledDisplay = 0;

// ──────────────────────────────────────────────────────────────────────────────
// Declaration: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

static void adcc_combination_mode();
static void adcc_display_graph_mode();
static void adcc_value_mode();
static void adcc_switch_mode();

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

// void adcc_led_control(uint8_t led_num, uint8_t state)
// {
//     switch (led_num)
//     {
//     case 1:
//         LED1(state);
//         break;
//     case 2:
//         LED2(state);
//         break;
//     case 3:
//         LED3(state);
//         break;
//     case 4:
//         LED4(state);
//         break;
//     case 5:
//         LED5(state);
//         break;
//     case 6:
//         LED6(state);
//         break;
//     default:
//         break;
//     }
// }

// void adcc_led_mode()
// {
//     static uint8_t i = 0;
//     static uint8_t led_num = 0;
//     // 先关闭所有 LED
//     for (int i = 1; i <= 6; i++)
//     {
//         adcc_led_control(i, 0); // 关闭 LED1~LED6
//     }
//     // 根据电压范围点亮对应的 LED
//     if (adcx == 0)
//     {
//         led_num = 0;
//     }
//     else if (adcx < 0.550f && adcx != 0)
//     {
//         led_num = 1;
//     }
//     else if (adcx < 1.100f)
//     {
//         led_num = 2;
//     }
//     else if (adcx < 1.650f)
//     {
//         led_num = 3;
//     }
//     else if (adcx < 2.200f)
//     {
//         led_num = 4;
//     }
//     else if (adcx < 2.750f)
//     {
//         led_num = 5;
//     }
//     else
//     {
//         led_num = 6;
//     }
//     for (i = 1; i <= led_num; i++)
//     {
//         adcc_led_control(i, 1);
//     }
// }

static void adcc_combination_mode()
{
    static uint8_t xpos = 0;
    static uint8_t ypos = 0;
    const float adc_max = 3.3;

    static uint8_t yps = 0;
    static uint8_t xps = 25;

    oled_show_string(xps, yps, "ADC:", 12);
    oled_show_num(xps + 30, yps, adcx, 2, 12, 1);
    oled_show_string(xps + 45, yps, ".", 12);
    oled_show_num(xps + 50, yps, (int)(adcx * 100) % 100, 2, 12, 1);

    xpos++;
    if (xpos > 128)
    {
        oled_clear();
        xpos = 0;
    }
    ypos = (uint8_t)30 - (adcx / adc_max * 16);
    oled_draw_point(xpos, ypos, 1);
    oled_refresh_gram();
}

static void adcc_display_graph_mode()
{
    static uint8_t xpos = 0;
    static uint8_t ypos = 0;
    const float adc_max = 3.3;
    xpos++;
    if (xpos > 128)
    {
        oled_clear();
        xpos = 0;
    }
    ypos = (uint8_t)30 - (adcx / adc_max * 30);
    oled_draw_point(xpos, ypos, 1);
    oled_refresh_gram();
}

static void adcc_value_mode()
{
    static uint8_t yps = 10;
    static uint8_t xps = 30;
    oled_show_string(xps, yps, "ADC:", 16);
    oled_show_num(xps + 30, yps, adcx, 2, 16, 1);
    oled_show_string(xps + 50, yps, ".", 16);
    oled_show_num(xps + 55, yps, (int)(adcx * 1000) % 1000, 3, 16, 1);

    oled_refresh_gram();
}

static void adcc_switch_mode()
{
    if (!displayMode)
    {
        adcc_combination_mode();
    }
    else if (displayMode == 1)
    {
        adcc_display_graph_mode();
    }
    else
    {
        adcc_value_mode();
    }
}

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: external call functions
// ──────────────────────────────────────────────────────────────────────────────

void adcc_handle_key(uint8_t key)
{
    if (key == 0)
        return;

    switch (key)
    {
    case 1:
        displayMode++;
        if (displayMode >= 3)
        {
            displayMode = 0;
        }
        oled_clear();
        break;
    }
}

void adcc_running()
{
    adcx = adc_get_result_average(ADC_ADCX_CHY, 20); /* 获取通道CHY的转换值，20次取平均 */
    temp = (float)adcx * (3.3 / 4096);               /* 获取计算后的带小数的实际电压值(12位分辨率)，比如3.1111 */
    adcx = temp;
    adcc_switch_mode();
}
