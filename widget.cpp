#include <iostream>
#include <string>
#ifdef __unix__
    #include <ncursesw/curses.h>
#else //if //defined(_WIN32)
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif

#include "screen.hpp"
#include "widget.hpp"

Widget::Widget(std::wstring _title, v2d _pos, v2d _size) : title { _title }, pos { _pos }, size { _size } {
    //std::wcout << "Constructor widget '" << this->title << "'" << std::endl;
    this->win = newwin(this->size.y, this->size.x, this->pos.y, this->pos.x);
    this->titlePosX = (this->size.x / 2) - (this->title.length() / 2);
    this->colorPair = colorPairs::BLUE_ON_WHITE;
}
Widget::Widget(std::wstring _title) : title { _title } {
    //std::wcout << "Constructor widget '" << this->title << "'" << std::endl;
    this->pos = {0, 0};
    this->size = {1, 1};
    this->win = newwin(this->size.y, this->size.x, this->pos.y, this->pos.x);
    this->titlePosX = (this->size.x / 2) - (this->title.length() / 2);
}
Widget::~Widget() {
    //std::wcout << "Destructor widget '" << this->title << "'" << std::endl;
    delwin(this->win);
}

void Widget::setPos(v2d _pos) {
    this->pos = _pos;
}
void Widget::setSize(v2d _size) {
    this->size = _size;
    this->titlePosX = (this->size.x / 2) - (this->title.length() / 2);
}
void Widget::setColorPair(int _colorPair) {
    this->colorPair = _colorPair;
}
void Widget::refreshPosAndSize() {
    mvwin(this->win, this->pos.y, this->pos.x);
    wresize(this->win, this->size.y, this->size.x);
}

void Widget::draw() {
    mvwaddwstr(this->win, 0, this->titlePosX, this->title.c_str());
    //wprintw(this->win, this->title.c_str())
    
    mvwaddwstr(this->win, 4, 4, L"PROUT");
}

void Widget::mainDraw() {
    werase(this->win);
    wbkgd(this->win, COLOR_PAIR(this->colorPair));
    wattron(this->win, COLOR_PAIR(this->colorPair) | A_BOLD);

    int _py = 0;
    int _px = 0;
    for (int _tx = _px; _tx < this->size.x; _tx++) {
        mvwaddwstr(this->win, 0, _tx, L"▀");
        mvwaddwstr(this->win, this->size.y - 1, _tx, L"▄");
    }
    for (int _ty = _py; _ty < this->size.y; _ty++) {
        mvwaddwstr(this->win, _ty, 0, L"█");
        mvwaddwstr(this->win, _ty, _px + this->size.x - 1, L"█");
    }
    draw();

    /*
    unsigned short nLine = 1;
    for (auto content : this->contentLines) {
        mvwaddwstr(this->win, nLine, 0, content);
        nLine++;
        if (nLine >= this->size.y)
            break;
    }
    */

    //if (bDoRefresh)
    wrefresh(this->win);
}