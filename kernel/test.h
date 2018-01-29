/**
 * Test header file
 * @file 	test.h
 * @project	TexOS
 * @author	Maxime Lovino, Marco Lopes, Loic Willy
 * @date	November 21, 2017
 */

#ifndef _TEST_H_
#define _TEST_H_

#include "../common/min_std_lib.h"
#include "display.h"
#include "timer.h"

/**
 * Function to run the kernel demo mode
 */
extern void demo_mode();

/**
 * Function that runs the automated tests and displays results on screen
 */
extern void automated_tests();

#endif
