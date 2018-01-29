/**
 * Timer source file
 * @file 	timer.c
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 21, 2017
 */

#include "timer.h"

static uint32_t ticks = 0;
static uint32_t freq = HIGHEST_FREQ;

void timer_init(uint32_t freq_hz) {
	if (freq_hz < LOWEST_FREQ) {
		freq_hz = LOWEST_FREQ;
	} else if (freq_hz > HIGHEST_FREQ) {
		freq_hz = HIGHEST_FREQ;
	}
	uint16_t divide_value = (uint16_t) (HIGHEST_FREQ / freq_hz);
	freq = freq_hz;
	outb(TIMER_COMMAND_REGISTER, 0x36);
	outb(TIMER_PORT_0, (uint8_t) (divide_value & 0xFF));
	outb(TIMER_PORT_0, (uint8_t) ((divide_value >> 8) & 0xFF));
	ticks = 0;
}

void timer_handler() {
	ticks++;
}

uint get_ticks() {
	return ticks;
}

void sleep(uint ms) {
	uint32_t tick_time = 1000 / freq;
	uint32_t start_tick = get_ticks();

	while (((get_ticks() - start_tick) * tick_time) < ms) {}
}


