#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "./SYSTEM/sys/sys.h"

void clock_handle_key(uint8_t key);
void clock_init();
void clock_runing();
void clock_exit();

#endif /* __CLOCK_H__ */