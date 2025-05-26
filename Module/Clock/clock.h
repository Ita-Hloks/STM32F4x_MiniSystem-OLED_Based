#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"

#include "stdio.h"

extern uint8_t clock_h;
extern uint8_t clock_m;
extern uint8_t clock_s;
extern uint8_t clock_ampm;
extern uint8_t clock_getTime_count;
extern uint8_t select_index;
extern uint8_t *cur_pos;   /* Pointer to the current editing variable h/m/s */
extern uint8_t cur_pos_index; /* 0: null 1: hour 2: minus 3: secound */

void clock_JudgeDisplayMode();
void clock_ChooseCurVar(uint8_t i);
void clock_CurVar_Add(uint8_t *pos);
void clock_CurVar_Sub(uint8_t *pos);
void clock_HandleKeyPress (uint8_t key);


#endif /* __CLOCK_H__ */