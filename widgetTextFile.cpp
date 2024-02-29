#include <time.h>
//#include <iostream>
#ifdef __unix__
    #include <ncursesw/curses.h>
#else //if //defined(_WIN32)
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif
#include <locale>
#include <sstream>
#include <fstream>
#include <codecvt>

#include "screen.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "cursesAnsi.hpp"

WidgetTextFile::WidgetTextFile(std::wstring _title, const char* _ansiFilePath) : Widget::Widget(_title) {
    // TODO : tester ouverture et rename file
    std::wifstream wif(_ansiFilePath);
    wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();

    std::wstring _line;
    while (getline(wss, _line))
        this->contentLines.push_back(_line);
}

WidgetTextFile::~WidgetTextFile() {
    if (txtFile.is_open())
        txtFile.close();
}

void WidgetTextFile::handleKey(int _keycode) {
    switch (_keycode)
    {
    case KEY_UP:
        if (this->cursorY > 0)
            this->cursorY--;
        break;
    case KEY_DOWN:
        if (this->cursorY < this->contentLines.size() - this->size.y + 2)
            this->cursorY++;
        break;
    case KEY_PPAGE:
        this->cursorY -= this->size.y - 2;
        if (this->cursorY < 0)
            this->cursorY = 0;
        break;
    case KEY_NPAGE:
        this->cursorY += this->size.y - 2;
        if (this->cursorY > this->contentLines.size() - this->size.y + 2)
            this->cursorY = this->contentLines.size() - this->size.y + 2;
        break;
    default:
        break;
    }
}

void WidgetTextFile::draw() {
    wbkgd(this->win, COLOR_PAIR(this->colorPair));
    wattroff(this->win, COLOR_PAIR(this->colorPair) | A_BOLD | A_REVERSE);   

    std::wstring *content;
    for (unsigned short nLine = this->cursorY; nLine < this->contentLines.size(); nLine++) {
        content = &this->contentLines[nLine];
        mvwaddnwstr(this->win, nLine - this->cursorY + 1, 2, content->c_str(), this->size.x - 4);
        if (nLine - this->cursorY + 3 >= this->size.y)
            break;
    }
}

