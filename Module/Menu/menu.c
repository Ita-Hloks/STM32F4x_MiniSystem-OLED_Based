#include "./menu.h"

#include "./SYSTEM/delay/delay.h"
#include "../Module/Clock/clock.h"
#include "../Module/StopWatch/stopwatch.h"
#include "../Module/ADCChecker/adcc.h"
#include "./BSP/RTC/rtc.h"
#include "../Drivers/BSP/OLED/oled.h"
#include "./SYSTEM/sys/sys.h"

#include <string.h>

// ──────────────────────────────────────────────────────────────────────────────
// VAR & CONST DEF
// ──────────────────────────────────────────────────────────────────────────────

// KEY
#define KEY_CONFIRM 1
#define KEY_LAST 2
#define KEY_NEXT 3
#define KEY_EXIT 6

typedef void (*AppRunFunc)(uint8_t key);

typedef struct {
    const char *name;
    AppRunFunc run;
} AppEntry;

#define MENU_LEN (sizeof(menu) / sizeof(menu[0]))

static const char *menu[] = {
    "Clock",
    "ADC Checker",
    "StopWatch",
    "NoteBook",
    "ABC2",
    "ABC3",
    "ABC4",
    "ABC5",
};
static uint8_t menuPointerIndex = 0; // 三角指针的位置，0: Up， 1: Down
uint8_t selectAppIndex = 0;       // 应用程序选择的索引值
uint8_t currState = 0;

// ──────────────────────────────────────────────────────────────────────────────
// Declaration: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────
static void menu_skeleton();
static void menu_pointer();
static void menu_pointer_up();
static void menu_pointer_down();
static void menu_contain();
static void menu_last_app();
static void menu_next_app();
// Application:
void app_clock(uint8_t key);
void app_stopwatch(uint8_t key);
void app_adc_checker(uint8_t key);
void app_notebook(uint8_t key);

// Menu Construct Array:
static const AppEntry appList[] = {
    {"Clock",        app_clock},
    {"ADC Checker",  app_adc_checker},
    {"StopWatch",    app_stopwatch},
    {"NoteBook",     app_notebook},
    {"ABC2",         NULL},
    {"ABC3",         NULL},
    {"ABC1ABC2",     NULL},
};

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: STATTIC FUNCTIONS
// ──────────────────────────────────────────────────────────────────────────────

static void menu_skeleton()
{
    oled_fill(0, 0, 130, 0, 1);
    oled_fill(0, 30, 130, 30, 1);
    oled_fill(0, 0, 0, 30, 1);
    oled_fill(127, 0, 127, 30, 1);
    oled_refresh_gram();
}

static void menu_pointer()
{
    static uint8_t add = 0;
    add = menuPointerIndex == 0 ? 0 : 15;

    // 三角形
    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t y_start = 7 + add - i;
        uint8_t y_end = 8 + add + i;
        oled_fill(15 - i * 2, y_start, 16 - i * 2, y_end, 1);
    }
}

static void menu_pointer_up()
{
    oled_show_string(20, 2, menu[selectAppIndex], 12);
    oled_show_string(20, 16, menu[selectAppIndex + 1], 12);
}

static void menu_pointer_down()
{
    oled_show_string(20, 2, menu[selectAppIndex - 1], 12);
    oled_show_string(20, 16, menu[selectAppIndex], 12);
}

static void menu_contain()
{
    oled_fill(5, 2, 126, 29, 0);
    menu_pointer();
    if (menuPointerIndex == 0)
    {
        menu_pointer_up();
    }
    else
    {
        menu_pointer_down();
    }
    oled_refresh_gram();
}

static void menu_last_app()
{
    oled_fill(5, 2, 126, 29, 0);
    if (selectAppIndex != 0)
    {
        selectAppIndex--;
    } else {
        selectAppIndex = MENU_LEN - 1;
    }
    oled_fill(5, 17, 126, 29, 0);
    oled_fill(5, 2, 126, 14, 0);

    menuPointerIndex = 0;
    menu_pointer();
    menu_pointer_up();
    oled_refresh_gram();
}

static void menu_next_app()
{
    oled_fill(5, 2, 126, 29, 0);
    if (selectAppIndex + 1 < MENU_LEN)
    {
        selectAppIndex++;
    } else {
        selectAppIndex = 0;
    }

    menuPointerIndex = 1;
    menu_pointer();
    menu_pointer_down();
    oled_refresh_gram();
}

// Application:
void app_clock(uint8_t key) {
    if (key) clock_handle_key(key);
    clock_runing();
}

void app_stopwatch(uint8_t key) {
    if (key) st_handle_key(key);
    st_running();
}

void app_adc_checker(uint8_t key) {
    if (key) adcc_handle_key(key);
    adcc_running();
}

void app_notebook(uint8_t key) {
    oled_show_string(40, 5, "NoteBook", 16);
    oled_refresh_gram();
}


// ──────────────────────────────────────────────────────────────────────────────
// Implementation: external call functions
// ──────────────────────────────────────────────────────────────────────────────

void menu_judgeapp_handlekey(uint8_t selectAppIndex, uint8_t key)
{
    if (appList[selectAppIndex].run) {
        appList[selectAppIndex].run(key);
    }
}

void menu_switch_key(uint8_t key)
{
    if (key == 0) return;
    switch (key)
    {
    case KEY_CONFIRM: // CONFIRM
        currState++;
        oled_clear();
        menu_judgeapp_handlekey(selectAppIndex, 0);
        delay_ms(100);
        break;
    case KEY_LAST: // LAST
        menu_last_app();
        delay_ms(50);
        break;
    case KEY_NEXT: // NEXT
        menu_next_app();
        delay_ms(50);
        break;
    }
}

void menu_init() {
    menu_skeleton();
    menu_contain();
}

void menu_exit(uint8_t key)
{
    if (key == 0) return;
    if(key == KEY_EXIT && currState != 0) {// EXIT
        clock_exit();
        currState = 0;
        oled_clear();
        menu_skeleton();
        menu_contain();
    }
}

void menu_main_running(uint8_t key)
{
    menu_exit(key); 
    if (currState == 0)
    { // Menu
        menu_switch_key(key);
    }
    else if (currState == 1)
    { // App
        menu_judgeapp_handlekey(selectAppIndex, key);
    }
}
