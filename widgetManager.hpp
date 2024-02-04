#ifndef ADASH_WIDGETMANAGER_H
#define ADASH_WIDGETMANAGER_H

#include <vector>
#include "widget.hpp"

extern v2d termSize;

class WidgetManager {
protected:
    WidgetManager(); 
    static WidgetManager* widgetmgr;
    // ==================================
    //Rows of widget. One row is a vector of widgets.
    std::vector<std::vector<Widget*>> widgets;
    Widget* selectedWidget;
    int indSelected;
    std::vector<Widget*> widgetsFlat; //  pour parcours séquentiels lors sélection par exemple
public:
    static WidgetManager* getInstance();
    // ==================================
    void drawWidgets();
    bool addWidget(unsigned short _row, Widget* _widget, wSizeMode _mh, wSizeMode _mv, unsigned int _h, unsigned int _w);
    void refreshWidgetsSizes(unsigned short _termCols, unsigned short _termLines);
    void freeWidgets();
    // ==================================
    void tabSelection(); // TAB received
    void deselection();

    void handleKey(int _keycode);
};

#endif