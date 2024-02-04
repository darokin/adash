#ifndef ADASH_SCREEN_H
#define ADASH_SCREEN_H

#ifdef _WIN32
	#include <windows.h>
    #define msleep(x) Sleep((x))
#else // __unix__
    #define msleep(x) usleep((x) * 100)   // todo faire inline à la place ?
#endif

enum colorPairs {
	WHITE_ON_BLACK,
	BLACK_ON_WHITE,
	YELLOW_ON_WHITE,
	YELLOW_ON_BLACK,
	BLACK_ON_YELLOW,
	PINK_ON_WHITE,
	PINK_ON_BLACK,
	BLACK_ON_PINK,
	BLUE_ON_WHITE,
	BLUE_ON_BLACK,
	BLACK_ON_BLUE
};

void screenInit();
void screenLoop(); // TODO faire une main game loop à la place
void screenExit();
//void screenRefresh();
void screenResize();
void initWidgets();
void drawWidgets();

#endif