#ifndef __BG_H__
#define __BG_H__

typedef void (*BackgroundTask)(void);

// 注册一个后台任务，后续会在 run_background_tasks() 中被周期性调用
// 已注册过的任务不会重复添加；超过最大数时自动忽略
void register_background_task(BackgroundTask task);

// 注销一个已注册的后台任务
void unregister_background_task(BackgroundTask task);

// 在主循环最前面调用，执行所有已注册任务
void run_background_tasks(void);

#endif /* __BG_H__ */