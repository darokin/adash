#ifndef ADASH_MODULEDIALOG_H
#define ADASH_MODULEDIALOG_H

#define CURS_BLOCK  L"█"
static const wchar_t* cursBlock = L"█";

#include <vector>
#include <sstream>

#ifdef __unix__
    #include <cursesw.h>
#elif defined _WIN32
    #define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#endif

struct dialogSequence {
    std::vector<wchar_t*> lines;
    int nbLines;
    int nbChars;
};

class ModuleDialog {
private:
    WINDOW *win;    
    std::vector<dialogSequence*> sequences{};
    size_t seqIndex {0};
    long long seqTimeStart {0};
    unsigned short nbColumns;
    unsigned short lineMax {0};
    unsigned short lineMaxForced {0};
    float msPerChar {100.f};
    bool blink {false};
    bool centered {false};
public:
    ModuleDialog(WINDOW* _win, unsigned short _nbColumns, unsigned short _lineMaxForced);
    ~ModuleDialog() {};
    void initText(std::vector<std::wstring>& _lines);
    void draw(long long _timeCurrent);
    void dialogPrev(long long _timeCurrent);
    void dialogNext(long long _timeCurrent);
};

#endif
