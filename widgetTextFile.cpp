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
    
    /*
    this->bBorder = false;
    txtFile.open(_ansiFilePath);
    if (!txtFile.is_open()) {
        std::cout << "Unable to open ANSI file '" << _ansiFilePath << "'" << std::endl; 
        return;
    }

    std::wstring _line;
    //this->txtFile.seekg(0);

    std::wstringstream wss;
    wss << txtFile.rdbuf();
    //wstr = wss.str();
    txtFile.close();
    */
    

    //std::wstring readFile(const char* filename)
    //{
        //txtFile.open(_ansiFilePath);
        std::wifstream wif(_ansiFilePath);
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        std::wstringstream wss;
        wss << wif.rdbuf();
        //return wss.str();
    //}

    //  usage
    //std::wstring wstr2;
    //wstr2 = readFile("C:\\yourUtf8File.txt");
    //wcout << wstr2;



    std::wstring _line;
    int dd = 0;
    while (getline(wss, _line)) {
        this->contentLines.push_back(_line);
        // dd++;
        // if (dd > 40)
        //     break;
    }
    this->contentLines.push_back(L"ENDDDDDD");
 
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

