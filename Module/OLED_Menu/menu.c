/************************************ IMPORT START **************************************/
#include "./menu.h"

// Bank
#include "../Module/Clock/clock.h"
#include "menu.h"

/************************************ IMPORT END ****************************************/
const char *menu[] = {
    "Clock",
    "NoteBook",
    "ADC Checker",
    "ABC1",
    "ABC2",
    "ABC3",
    "ABC1ABC2",
};
uint8_t menu_pointer_index = 0; // 三角指针的位置，0在上， 1在下

/****************************** STATIC FUNCTION START ***********************************/
static void Menu_Skeletons()
{
    oled_fill(0, 0, 130, 0, 1);
    oled_fill(0, 30, 130, 30, 1);
    oled_fill(0, 0, 0, 30, 1);
    oled_fill(127, 0, 127, 30, 1);
    oled_refresh_gram();
}

static void Menu_Pointer()
{
    static uint8_t add = 0;
    add = menu_pointer_index == 0 ? 0 : 15;

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
static void Menu_DisplayPointUp()
{
    oled_show_string(20, 2, menu[menu_select_index], 12);
    oled_show_string(20, 16, menu[menu_select_index + 1], 12);
}

/**
 * @brief 当三角形指针在下方的菜单更新
 *
 */
static void Menu_DisplayPointDown()
{
    oled_show_string(20, 2, menu[menu_select_index - 1], 12);
    oled_show_string(20, 16, menu[menu_select_index], 12);
}

static void Menu_Contain()
{
    oled_fill(5, 2, 126, 29, 0);
    Menu_Pointer();
    if (menu_pointer_index == 0)
    {
        Menu_DisplayPointUp();
    }
    else
    {
        Menu_DisplayPointDown();
    }
    oled_refresh_gram();
}

/******************************** STATIC FUNCTION END ***********************************/

void Menu_Init() {
    Menu_Skeletons();
    Menu_Contain();
}

void Menu_Exit(uint8_t key)
{
    if(key == 6 && curr_state != 0) {// EXIT
        curr_state = 0;
        oled_clear();
        Menu_Skeletons();
        Menu_Contain();
    }
}


void Menu_LastApp()
{
    oled_fill(5, 2, 126, 29, 0);
    if (menu_select_index != 0)
    {
        menu_select_index--;
    } else {
        menu_select_index = MENU_LEN - 1;
    }
    oled_fill(5, 17, 126, 29, 0);
    oled_fill(5, 2, 126, 14, 0);

    menu_pointer_index = 0;
    Menu_Pointer();
    Menu_DisplayPointUp();
    oled_refresh_gram();
}

void Menu_NextApp()
{
    oled_fill(5, 2, 126, 29, 0);
    if (menu_select_index + 1 < MENU_LEN)
    {
        menu_select_index++;
    } else {
        menu_select_index = 0;
    }

    menu_pointer_index = 1;
    Menu_Pointer();
    Menu_DisplayPointDown();

    oled_refresh_gram();
}

/****************************** APPLICATION FUNCTION ************************************/
void Menu_App_Clock()
{
    clock_runing();
}

void Menu_App_noteBook()
{
    oled_clear();
    oled_show_string(40, 5, "NoteBook", 16);
    oled_refresh_gram();
}

void Menu_JudgeApp_HandleKey(uint8_t menu_select_index, uint8_t key)
{
    if (strcmp(menu[menu_select_index], "Clock") == 0)
    {
        clock_handle_key(key);
        Menu_App_Clock();
    }
    else if (strcmp(menu[menu_select_index], "NoteBook") == 0)
    {
        Menu_App_noteBook();
    }
    else if (strcmp(menu[menu_select_index], "ADC Checker") == 0)
    {
        oled_show_string(40, 5, "ADC Checker", 16);
        oled_refresh_gram();
    }
}
