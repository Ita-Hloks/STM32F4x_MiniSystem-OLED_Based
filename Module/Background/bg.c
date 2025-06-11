#include "bg.h"
#include "./USMART/usmart.h"

#include <string.h>
// ──────────────────────────────────────────────────────────────────────────────
// VAR & CONST DEF
// ──────────────────────────────────────────────────────────────────────────────

#define MAX_BACKGROUND_TASKS   8   //同时最多支持 8 个并发后台任务
// 已注册任务的数组
static BackgroundTask bg_tasks[MAX_BACKGROUND_TASKS] = {0};

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: external call functions
// ──────────────────────────────────────────────────────────────────────────────
void register_background_task(BackgroundTask task) {
    if (!task) return;
    // 查重 & 寻找空位
    for (int i = 0; i < MAX_BACKGROUND_TASKS; ++i) {
        if (bg_tasks[i] == task) {
            return;
        }
        if (bg_tasks[i] == NULL) {
            bg_tasks[i] = task;
            return;
        }
    }
    printf("[ERR] BG: The task Register FALL !");
}

void unregister_background_task(BackgroundTask task) {
    if (!task) return;
    for (int i = 0; i < MAX_BACKGROUND_TASKS; ++i) {
        if (bg_tasks[i] == task) {
            bg_tasks[i] = NULL;
            return;
        }
    }
    printf("[WAR] BG: The function is not found!");
}

void run_background_tasks(void) {
    for (int i = 0; i < MAX_BACKGROUND_TASKS; ++i) {
        if (bg_tasks[i]) {
            bg_tasks[i]();
        }
    }
}
