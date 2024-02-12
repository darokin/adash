#ifndef ADASH_WIDGETDIALOG_H
#define ADASH_WIDGETDIALOG_H

#include "screen.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"

extern v2d termSize;

//struct dialogSequence {
//    int index;
//    std::vector<wchar_t*> lines{}; 
//};

class WidgetDialog : public WidgetTextFile {
private:
    //std::wifstream txtFile;
    //std::vector<std::wstring> contentLines{};
    //int cursorY {0};
    std::vector<unsigned short[2]> seqStartsInContent{};
    int readSeqIndex; // Reading sequence number

    //std::vector<dialogSequence> dialogSequences{};
    std::vector<std::vector<wchar_t*>> dialogSequences{};
public:
    WidgetDialog(std::wstring _title, const char* _ansiFilePath);
    ~WidgetDialog();
    void draw();
    virtual void handleKey(int _keycode);
};

#endif
