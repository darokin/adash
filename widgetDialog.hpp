#ifndef ADASH_WIDGETDIALOG_H
#define ADASH_WIDGETDIALOG_H

#include "screen.hpp"
#include "widget.hpp"
#include "widgetTextFile.hpp"
#include "moduleDialog.hpp"

extern v2d termSize;

//struct dialogSequence {
//    int index;
//    std::vector<wchar_t*> lines{}; 
//};

class WidgetDialog : public WidgetTextFile {
private:
    long long dialogTimeStart;
    ModuleDialog* dialog;
public:
    WidgetDialog(std::wstring _title, const char* _ansiFilePath);
    ~WidgetDialog();
    void draw();
    virtual void handleKey(int _keycode);

    void addDialog(std::vector<std::wstring>& _lines);
    void addDialog();
};

#endif
