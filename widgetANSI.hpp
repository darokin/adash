#ifndef ADASH_WIDGETANSI_H
#define ADASH_WIDGETANSI_H

#include "widget.hpp"

class WidgetANSI : public Widget {
private:
    std::ifstream ansiFile;
public:
    WidgetANSI(std::wstring _title, v2d _pos, v2d _size);
    WidgetANSI(std::wstring _title);
    WidgetANSI(std::wstring _title, const char* _ansiFilePath);
    ~WidgetANSI();
    void draw();
};

#endif
