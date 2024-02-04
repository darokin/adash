#ifndef ADASH_WIDGETMANAGER_H
#define ADASH_WIDGETMANAGER_H

#include <vector>
#include "widget.hpp"

class WidgetManager {
protected:
    WidgetManager() { }
    static WidgetManager* widgetmgr;
    // ==================================
    //Rows of widget. One row is a vector of widgets.
    std::vector<std::vector<Widget*>> widgets;
public:
    static WidgetManager* getInstance();
    // ==================================
    void drawWidgets();
    bool addWidget(unsigned short _row, Widget* _widget, wSizeMode _mh, wSizeMode _mv, unsigned int _h, unsigned int _w);
    void freeWidgets();
    void refreshWidgetsSizes();
};

#endif