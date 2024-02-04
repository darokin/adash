#include <ctime>
#ifdef __unix__
    #include <ncursesw/curses.h>
#else //if //defined(_WIN32)
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif
#include <vector>
#include "screen.hpp"
#include "widget.hpp"
#include "widgetClock.hpp"

WidgetClock::WidgetClock(std::wstring _title, v2d _pos, v2d _size) : Widget::Widget(_title, _pos, _size) {
}
WidgetClock::WidgetClock(std::wstring _title) : Widget::Widget(_title) {
}
WidgetClock::~WidgetClock() {
}

#define CHARBIG_COLS    5
#define CHARBIG_LINES    11

#define CHARSMALL_COLS    3
#define CHARSMALL_LINES    11

std::wstring arrNum[CHARBIG_LINES][CHARBIG_COLS] = { 
	{L"████████", L"██⠀⠀⠀⠀██", L"██⠀⠀⠀⠀██", L"██⠀⠀⠀⠀██", L"████████"},
	{L"██", L"██", L"██", L"██", L"██"},
	{L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████", L"██      ", L"████████"},
	{L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████"},
	{L"██⠀⠀⠀⠀██", L"██⠀⠀⠀⠀██", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██"},
	{L"████████", L"██⠀⠀⠀⠀", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████"},
	{L"████████", L"██⠀⠀⠀⠀", L"████████", L"██⠀⠀⠀⠀██", L"████████"},
	{L"████████", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██", L"⠀⠀⠀⠀⠀⠀██"},
	{L"████████", L"██⠀⠀⠀⠀██", L"████████", L"██⠀⠀⠀⠀██", L"████████"},
	{L"████████", L"██⠀⠀⠀⠀██", L"████████", L"⠀⠀⠀⠀⠀⠀██", L"████████"},
	{L"⠀⠀⠀", L"███", L"⠀⠀⠀", L"███", L"⠀⠀⠀"}
 };

// # SMALL ASCII DIGITS
std::wstring arrNumSmall[CHARSMALL_LINES][CHARSMALL_COLS] = { 
	{L"▄▄▄▄", L"█⠀⠀█", L"█▄▄█"},
	{L"▄", L"█", L"█"},
	{L"▄▄▄▄", L"▄▄▄█", L"█▄▄▄"},
	{L"▄▄▄▄", L"⠀▄▄█", L"▄▄▄█"},
	{L"▄⠀⠀▄", L"█▄▄█", L"⠀⠀⠀█"},
	{L"▄▄▄▄", L"█▄▄▄", L"▄▄▄█"},
	{L"▄▄▄▄", L"█▄▄▄", L"█▄▄█"},
	{L"▄▄▄▄", L"⠀⠀⠀█", L"⠀⠀⠀█"},
	{L"▄▄▄▄", L"█▄▄█", L"█▄▄█"},
	{L"▄▄▄▄", L"█▄▄█", L"▄▄▄█"},
	{L"⠀", L"■", L"▀"}
};

inline static void drawNumber(int _num, int _posx, int _posy, WINDOW *_win) {
	for (auto row : arrNum[_num]) {
		wmove(_win, _posy, _posx);
		waddwstr(_win, row.c_str());
		_posy = _posy + 1;
    }
};

void WidgetClock::draw() {
 
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

    /*
    wchar_t strTime[100];
    swprintf(strTime, 100, L"%02d/%02d/%d %02d:%02d:%02d", now->tm_mday,
            now->tm_mon + 1, now->tm_year + 1900,
            now->tm_hour, now->tm_min, now->tm_sec);
    */
    char hourMinute[5];
    snprintf(hourMinute, 5, "%02d%02d", now->tm_hour, now->tm_min);

    // == Parcours pour calcul du centrage;
    int val;
    int sizeX = 0;
    bool isBig = true;
    int sizeY = (isBig ? CHARBIG_COLS : CHARSMALL_COLS);
    for (int i = 0; i < 4; i++) {
		val = (int)hourMinute[i] - 48;
        sizeX += (arrNum[val][0].length() + (isBig ? 2 : 1));
    }
    // == Center start in win
    int start_x = (this->size.x / 2.0) - (sizeX / 2);
    int start_y = (this->size.y / 2.0) - (sizeY / 2);
    
    // == Parcours pour affichage
    int curX = start_x;
    for (int i = 0; i < 4; i++) {
		val = (int)hourMinute[i] - 48;
        if (val < 0 || val > 9)
            val = 9;
		drawNumber(val, curX, start_y, this->win);
        curX += (arrNum[val][0].length() + (isBig ? 2 : 1));
        if (i == 1) { 
            drawNumber(10, curX, start_y, this->win);
            curX += (arrNum[10][0].length() + (isBig ? 2 : 1));
        }
    }

    wattron(this->win, A_REVERSE);
    wchar_t strDate[100];
    swprintf(strDate, 100, L" %02d/%02d/%d ", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    wmove(this->win, start_y + sizeY, start_x + sizeX - 9);
    waddwstr(this->win, strDate);
    wattroff(this->win, A_REVERSE);
}
