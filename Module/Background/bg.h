#ifndef __BG_H__
#define __BG_H__

typedef void (*BackgroundTask)(void);

void register_background_task(BackgroundTask task);
void unregister_background_task(BackgroundTask task);
void run_background_tasks(void);

#endif /* __BG_H__ */