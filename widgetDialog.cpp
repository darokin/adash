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

WidgetDialog::WidgetDialog(std::wstring _title, const char* _ansiFilePath) : WidgetTextFile::WidgetTextFile(_title, _ansiFilePath) {
    /*
    std::wifstream wif(_ansiFilePath);
    wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
 

    std::wstring _line;
    int dd = 0;
    while (getline(wss, _line)) {
        this->contentLines.push_back(_line);
    }
    */
    
    this->readSeqIndex = 0;
    int nbColumns = 42;
    
    this->dialogSequences.push_back(std::vector<wchar_t*> {});
    
    // == We fill a vector of sequence starter with a pair of START / LENGTH
    std::wstring seqSeparator {L"-"};
    unsigned short lineCount {0};
    unsigned short charCount {0};
    wchar_t* newLine;
    for (int i = 0; i < this->contentLines.size(); i++) {
        newLine = new wchar_t[nbColumns]; //(wchar_t*)malloc(sizeof(wchar_t) * nbColumns);
        // == Parcours de la ligne
        for (auto wc : this->contentLines[i]) {
            // == Copy char by char
            newLine[charCount] = wc;
            charCount++;
            // == Test saut de ligne 
            // == Pour dépassement du nombre de colornne
            // == Ou pour le cas d'une fin de ligne normale (dernière caractère de la ligne dans le fichier)
            if (charCount >= nbColumns || wc == this->contentLines[i].back()) {
                // == On push la ligne terminée et on en commence une nouvelle
                this->dialogSequences.back().push_back(newLine);
                newLine = new wchar_t[nbColumns];
                lineCount++;
                charCount = 0;
            }
            //this->dialogSequences[this->dialogSequences.size()].push_back(newLine);

        }
        
        
        //if ((this->contentLines[0] == seqSeparator) || (i == this->contentLines.size() - 1)) {
            //this->seqStartsInContent.push_back({seqStart, seqLenght});
            //if (seqLenght > maxLength)
            //    maxLength = seqLenght;
            //seqLenght = 0;
            //seqStart = i;
        //} 
    }
    /*
    //this->seqStartsInContent.push_back({0, 0});
    unsigned short seqLenght {};
    unsigned short seqStart  {};
    unsigned short maxLength {};
    for (int i = 0; i < this->contentLines.size(); i++) {
        seqLenght++;
        if ((this->contentLines[0] == seqSeparator) || (i == this->contentLines.size() - 1)) {
            this->seqStartsInContent.push_back({seqStart, seqLenght});
            if (seqLenght > maxLength)
                maxLength = seqLenght;
            seqLenght = 0;
            seqStart = i;
        }
    }
    */
    // == From the max seqLenght we calculate optimum max Y and change position
    //this->setPos({})

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
    wbkgd(this->win, COLOR_PAIR(this->colorPair));
    wattroff(this->win, COLOR_PAIR(this->colorPair) | A_BOLD | A_REVERSE);   

    mvwaddnwstr(this->win, 1, 1, L"WIDGET DIALOG", 42);
    /*
    std::wstring *content;
    for (unsigned short nLine = this->cursorY; nLine < this->contentLines.size(); nLine++) {
        content = &this->contentLines[nLine];
        mvwaddnwstr(this->win, nLine - this->cursorY + 1, 2, content->c_str(), this->size.x - 4);
        if (nLine - this->cursorY + 3 >= this->size.y)
            break;
    }
    */
}

