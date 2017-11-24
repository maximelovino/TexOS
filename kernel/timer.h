/**
 * Timer header file
 * @file 	timer.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	November 21, 2017
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include "../common/types.h"
#include "pio_asm.h"

#define TIMER_COMMAND_REGISTER 0x43

#define TIMER_PORT_0 0x40

#define HIGHEST_FREQ 1193180
#define LOWEST_FREQ 19

/**
 * Function to initialize the timer
 * @param freq_hz The frequency to set for the timer
 */
extern void timer_init(uint32_t freq_hz);

/**
 * Function called at each timer tick, dispatched from irq_handler
 */
extern void timer_handler();

/**
 * Function to get the current tick count of the timer
 * @return The current tick count of the timer
 */
extern uint get_ticks();

/**
 * Function that implements an active wait of a given time
 * @param ms The time to wait, in ms
 */
extern void sleep(uint ms);

#endif
