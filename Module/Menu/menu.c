/************************************ IMPORT START **************************************/
#include "./menu.h"

// Bank
#include "../Module/Clock/clock.h"
#include "../Module/StopWatch/stopwatch.h"

/************************************ IMPORT END ****************************************/
static const char *menu[] = {
    "Clock",
    "ADC Checker",
    "StopWatch",
    "NoteBook",
    "ABC2",
    "ABC3",
    "ABC1ABC2",
};
static uint8_t menuPointerIndex = 0; // 三角指针的位置，0在上， 1在下
uint8_t selectAppIndex = 0;       // 应用程序选择的索引值
uint8_t currState = 0;

/****************************** STATIC FUNCTION START ***********************************/
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
/**
 * @brief 当三角形指针在 上方 的菜单更新
 *
 */
static void menu_pointer_up()
{
    oled_show_string(20, 2, menu[selectAppIndex], 12);
    oled_show_string(20, 16, menu[selectAppIndex + 1], 12);
}

/**
 * @brief 当三角形指针在下方的菜单更新
 *
 */
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

/******************************** STATIC FUNCTION END ***********************************/
/****************************** APPLICATION FUNCTION ************************************/
void menu_app_clock()
{
    clock_runing();
}

void menu_app_notebook()
{
    oled_show_string(40, 5, "NoteBook", 16);
    oled_refresh_gram();
}

void menu_judgeapp_handlekey(uint8_t selectAppIndex, uint8_t key)
{
    if (strcmp(menu[selectAppIndex], "Clock") == 0)
    {
        if (key) {
            clock_handle_key(key);
        }
        menu_app_clock();
    }
    else if (strcmp(menu[selectAppIndex], "NoteBook") == 0)
    {
        menu_app_notebook();
    }
    else if (strcmp(menu[selectAppIndex], "StopWatch") == 0)
    {
        if (key) {
            st_handle_key(key);
        }
        st_running();
    }
    else if (strcmp(menu[selectAppIndex], "ADC Checker") == 0)
    {
        oled_show_string(40, 5, "ADC Checker", 16);
        oled_refresh_gram();
    }
}

// void menu_running_app() {

// }

void menu_switch_key(uint8_t key)
{
    if (key == 0) return;
    switch (key)
    {
    case 1: // CONFIRM
        currState++;
        oled_clear();
        menu_judgeapp_handlekey(selectAppIndex, 0);
        delay_ms(100);
        break;
    case 2: // ADD
        menu_last_app();
        delay_ms(50);
        break;
    case 3: // SUB
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
    if(key == 6 && currState != 0) {// EXIT
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
