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
#include "widgetDialog.hpp"
#include "cursesAnsi.hpp"
#include "utils.hpp"

// = TODO : add position
WidgetDialog::WidgetDialog(std::wstring _title, const char* _ansiFilePath) : WidgetTextFile::WidgetTextFile(_title, _ansiFilePath) {
    dialogTimeStart = Utils::timeInMilliseconds();
    dialog = new ModuleDialog(this->win, this->size.x - 4, this->size.x - 4);
    dialog->initText(contentLines);
}

WidgetDialog::~WidgetDialog() {
    //if (txtFile.is_open())
    //    txtFile.close();
}

void WidgetDialog::handleKey(int _keycode) {
    switch (_keycode)
    {
    case KEY_UP:
    case KEY_DOWN:
    case KEY_PPAGE:
    case KEY_NPAGE:
        //if (this->readSeqIndex)
        break;
    default:
        break;
    }
}

void WidgetDialog::draw() {
    long long _timeCurrent;
    long timeLapsedMs;

    _timeCurrent = Utils::timeInMilliseconds();
    timeLapsedMs = (long) (_timeCurrent - this->dialogTimeStart);
    
    wbkgd(this->win, COLOR_PAIR(this->colorPair));
    wattroff(this->win, COLOR_PAIR(this->colorPair) | A_BOLD | A_REVERSE);   

    dialog->draw(timeLapsedMs);
}

