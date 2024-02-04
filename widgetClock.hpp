#ifndef ADASH_WIDGETCLOCK_H
#define ADASH_WIDGETCLOCK_H

#include "widget.hpp"

class WidgetClock : public Widget {
private:
    bool isBig {true};

public:
    WidgetClock(std::wstring _title, v2d _pos, v2d _size);
    WidgetClock(std::wstring _title);
    ~WidgetClock();
    virtual void draw();
    virtual void handleKey(int _keycode);
};

#endif
