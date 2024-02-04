#include <string>

#ifdef __unix__
    #include <ncursesw/curses.h>
#else //if //defined(_WIN32)
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif
#include "screen.hpp"
#include "widget.hpp"
#include "widgetDeco.hpp"

WidgetDeco::WidgetDeco(std::wstring _title, v2d _pos, v2d _size) : Widget::Widget(_title, _pos, _size) {
}
WidgetDeco::WidgetDeco(std::wstring _title) : Widget::Widget(_title) {
}
WidgetDeco::~WidgetDeco() {
}

void WidgetDeco::setType(decoType _type) {
    this->type = _type;
}

void WidgetDeco::draw() {
    int _size = 4;
    bool reverse = false;

   switch (this->type) {
    case decoType::FULL_BG:
        // == FULL BG
        for (int _y = 0; _y < size.y; _y++) 
            for (int _x = 0; _x < size.x; _x++) 
                mvwaddwstr(this->win, _y, _x, L"█");
        break;
    case decoType::CHECKBOARD:
        // == CHECKBOARDS
        for (int _y = 0; _y < size.y; _y++) { 
            for (int _x = 0; _x < size.x; _x++) { 
            if (((_x / _size / 2) % 2) != ((_y / _size) % 2)) {
                    wmove(win, _y, _x);
                    waddwstr(win, L"█");
                }
            } 
        }
        break;
    case decoType::STRIPE_REVERSE:
        // !!!!! WARNING no break
        reverse = true;
    case decoType::STRIPE:
        // === STRIPES 
        for (int _y = 0; _y < this->size.y; _y++) { 
            for (int _x = 0; _x < this->size.x; _x++) { 
            if (((_x + (reverse ? (this->size.y-_y) : _y)) / _size / 2) % 2 == 0) {
                    wmove(this->win, _y, _x);
                    waddwstr(this->win, L"█");
                }
            } 
        }
        break;
    default:
        break;
    }    
}
