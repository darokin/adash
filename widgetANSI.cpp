#include <time.h>
//#include <iostream>
#ifdef __unix__
    #include <ncursesw/curses.h>
#else //if //defined(_WIN32)
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif
#include "screen.hpp"
#include "widget.hpp"
#include "widgetANSI.hpp"
#include "cursesAnsi.hpp"

WidgetANSI::WidgetANSI(std::wstring _title, v2d _pos, v2d _size) : Widget::Widget(_title, _pos, _size) {
}
WidgetANSI::WidgetANSI(std::wstring _title) : Widget::Widget(_title) {
}
WidgetANSI::WidgetANSI(std::wstring _title, const char* _ansiFilePath) : Widget::Widget(_title) {
    ansiFile.open(_ansiFilePath);
    if (!ansiFile.is_open())
        std::cout << "Unable to open ANSI file '" << _ansiFilePath << "'" << std::endl; 
}
WidgetANSI::~WidgetANSI() {
    if (ansiFile.is_open())
        ansiFile.close();
}
void WidgetANSI::draw() {
    if (!ansiFile.is_open())
        return;

    //werase(this->win);
    
    std::string _line;
    int x{0}, y{1};
    while (getline(this->ansiFile, _line))
    {
        wmove(this->win, y+2, x+2);
        waddwstr(this->win, L"ansi dbg");
        break;
        if (!cursesANSI::mvwaddnANSIstr(this->win, y, x, _line, this->size.x))
            break;
        y++;
    }


    //wrefresh(this->win);
}
