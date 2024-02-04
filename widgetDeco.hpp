#ifndef ADASH_WIDGETDECO_H
#define ADASH_WIDGETDECO_H

#include "widget.hpp"

enum class decoType {
    FULL_BG,
    CHECKBOARD,
    STRIPE,
    STRIPE_REVERSE
};

class WidgetDeco : public Widget {
private:
    decoType type;
public:
    WidgetDeco(std::wstring _title, v2d _pos, v2d _size);
    WidgetDeco(std::wstring _title);
    ~WidgetDeco();
    virtual void draw();
    void setType(decoType _type);
};

#endif