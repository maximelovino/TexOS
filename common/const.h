/**
 * Constants header file
 * @file 	const.h
 * @project	TexOS
 * @author	Maxime Lovino, Loic Willy
 * @date	November 3, 2017
 */

#ifndef _CONST_H_
#define _CONST_H_

#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 80
#endif

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 25
#endif

#define DISPLAY_SIZE  (DISPLAY_WIDTH*DISPLAY_HEIGHT)
#define DISPLAY_SIZE_WITHOUT_LAST_LINE  (DISPLAY_WIDTH*(DISPLAY_HEIGHT-1))

#define ESCAPE_CHAR 27

#endif //_CONST_H_