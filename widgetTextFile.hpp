#ifndef ADASH_WIDGETTEXTFILE_H
#define ADASH_WIDGETTEXTFILE_H

#include "widget.hpp"

class WidgetTextFile : public Widget {
private:
    std::wifstream txtFile;
    std::vector<std::wstring> contentLines{};
    int cursorY {0};
public:
    WidgetTextFile(std::wstring _title, const char* _ansiFilePath);
    ~WidgetTextFile();
    void draw();
    virtual void handleKey(int _keycode);
};

#endif
