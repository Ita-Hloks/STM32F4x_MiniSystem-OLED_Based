#include "bg.h"
#include "./USMART/usmart.h"

#include <string.h>
// ──────────────────────────────────────────────────────────────────────────────
// VAR & CONST DEF
// ──────────────────────────────────────────────────────────────────────────────

#define MAX_BACKGROUND_TASKS   8   // 根据实际需求可调整：同时最多支持 8 个并发后台任务
// 存储已注册任务的数组
static BackgroundTask bg_tasks[MAX_BACKGROUND_TASKS] = {0};

// ──────────────────────────────────────────────────────────────────────────────
// Implementation: external call functions
// ──────────────────────────────────────────────────────────────────────────────
void register_background_task(BackgroundTask task) {
    if (!task) return;
    // 查重 & 寻找空位
    for (int i = 0; i < MAX_BACKGROUND_TASKS; ++i) {
        if (bg_tasks[i] == task) {
            return; // 已注册：直接返回
        }
        if (bg_tasks[i] == NULL) {
            bg_tasks[i] = task;
            return; // 注册成功
        }
    }
    printf("[ERR] BG: The task Register FALL !");
}

void unregister_background_task(BackgroundTask task) {
    if (!task) return;
    for (int i = 0; i < MAX_BACKGROUND_TASKS; ++i) {
        if (bg_tasks[i] == task) {
            bg_tasks[i] = NULL;
            return; // 注销成功
        }
    }
    // 若没找到，无动作
}

void run_background_tasks(void) {
    // 遍历固定长度数组，一一调用
    for (int i = 0; i < MAX_BACKGROUND_TASKS; ++i) {
        if (bg_tasks[i]) {
            bg_tasks[i]();
        }
    }
}
