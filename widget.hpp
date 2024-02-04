#ifndef ADASH_WIDGET_H
#define ADASH_WIDGET_H

#ifdef __unix__
    #include <ncursesw/curses.h>
#elif defined(_WIN32)
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif
#include <string>
#include <vector>

#include "utils.hpp"

enum class wSizeMode {
    MODE_FULL,     // Full width / float to max
    MODE_FIX,      // Fixed width provided by parameter
    MODE_1,
    MODE_2,        // 20% floating
    MODE_3,
    MODE_4,        // 40% floating
    MODE_5,
    MODE_6,        // 20% floating
    MODE_7,
    MODE_8,        // 40% floating
    MODE_9,
    MODE_33,        // etc.
    MODE_66,
    MODE_25,
    MODE_50,
    MODE_75,
    MODE_COUNT
};

static const float widgetSizeRatio[(int)wSizeMode::MODE_COUNT] {
    1.0,        // MODE_FULL,  
    2.0,        // MODE_FIX,    
    0.1,        // MODE_1,
    0.2,        // MODE_2,      
    0.3,        // MODE_3,
    0.4,        // MODE_4,      
    0.5,        // MODE_5,
    0.6,        // MODE_6,      
    0.7,        // MODE_7,
    0.8,        // MODE_8,      
    0.9,        // MODE_9,
    (1/3.0),    // MODE_33,     
    (1/1.5),    // MODE_66,
    0.25,       // MODE_25,
    0.50,       // MODE_50,
    0.75        // MODE_75,
    //MODE_COUNT
};

class Widget {
protected:
    WINDOW*     win;
    uint16_t    titlePosX;
    
public:
    std::wstring title{};
    v2d         pos{0, 0};
    v2d         size{80, 25};
    int         colorPair;
    std::vector<wchar_t[512]> contentLines{};

    wSizeMode modeH;
    wSizeMode modeV;
    unsigned int fixedH;
    unsigned int fixedV;
    Widget(std::wstring _title, v2d _pos, v2d _size);
    Widget(std::wstring _title);
    virtual ~Widget(); // so derived class automatically call widget destructor

    virtual void draw();
    void mainDraw();

    void refreshPosAndSize();
    void setPos(v2d _pos);
    void setColorPair(int _colorPair);
    void setSize(v2d _size);
};

#endif