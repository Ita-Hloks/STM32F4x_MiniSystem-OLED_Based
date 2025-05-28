#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"

#include "stdio.h"
#include <stdbool.h>

// void clock_JudgeDisplayMode();
void clock_handle_key(uint8_t key);
void clock_init();
void clock_runing();

#endif /* __CLOCK_H__ */