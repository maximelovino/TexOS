#ifndef _TIMER_H_
#define _TIMER_H_

#include "../common/types.h"
#include "pio_asm.h"

#define TIMER_COMMAND_REGISTER 0x43

#define TIMER_PORT_0 0x40

#define HIGHEST_FREQ 1193180

extern void timer_init(uint32_t freq_hz);
extern void timer_handler();
extern uint get_ticks();
extern void sleep(uint ms);

#endif
