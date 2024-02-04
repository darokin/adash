#include <stdexcept>
#include "screen.hpp"
#include "widgetManager.hpp"
#include "widget.hpp"

extern wchar_t debugText[256];

WidgetManager* WidgetManager::widgetmgr = nullptr;

WidgetManager::WidgetManager() {
    this->selectedWidget = nullptr;
    this->indSelected = 0;
}
WidgetManager* WidgetManager::getInstance() {
    if (widgetmgr == nullptr) {
        widgetmgr = new WidgetManager();
    }
    return widgetmgr;
}

void WidgetManager::drawWidgets() {
    for (auto wRow : widgets) {
        for (auto w : wRow) {
            w->mainDraw((w == this->selectedWidget));
        }
    }
}

void WidgetManager::handleKey(int _keycode) {
    if (_keycode == 9) { // TAB
    	this->tabSelection();
        return;
    } else if (_keycode == 27) {
        this->deselection();
        return;
    }
    
    if (!indSelected) 
        return;
    if (selectedWidget == nullptr)
        return;

    selectedWidget->handleKey(_keycode);
}

void WidgetManager::deselection() {
    if (this->selectedWidget != nullptr)
        this->selectedWidget->mainDraw(false);   
    this->indSelected = 0;
    this->selectedWidget = nullptr;
}

void WidgetManager::tabSelection() {
    if (this->selectedWidget == nullptr) {
        if (this->widgetsFlat.size() < 1)
            return;
        
        this->indSelected = 0;
    } else {
        if (this->indSelected + 1 >= this->widgetsFlat.size())
            this->indSelected = 0;
        else
            this->indSelected++;
    }
    try {
        this->selectedWidget = this->widgetsFlat.at(this->indSelected);
    } catch (std::out_of_range const& exc) {
        //std::cout << exc.what() << '\n';
        this->deselection();
    }
}

bool WidgetManager::addWidget(unsigned short _row, Widget* _widget, wSizeMode _mh, wSizeMode _mv, unsigned int _h, unsigned int _v) {
    std::vector<Widget*> rowOfWidget;
    
    // TODO : faire plus propre...
    _widget->modeH = _mh;
    _widget->modeV = _mv;
    _widget->fixedH = _h;
    _widget->fixedV = _v;

    this->widgetsFlat.push_back(_widget);

    if (_row > this->widgets.size() + 1) {
        // missing rows !
        return false;
    }
    if (_row == this->widgets.size() + 1) {
        // new row
        std::vector<Widget*> newRow {};
        newRow.push_back(_widget);
        this->widgets.push_back(newRow);
        return true;
    }
    
    // == Normal add widget to existing row
    try {
        this->widgets.at(_row-1).push_back(_widget);
    } catch (std::out_of_range const& exc) {
        //std::cout << exc.what() << '\n';
        return false;
    }
    
    return true;
}

void WidgetManager::refreshWidgetsSizes(unsigned short _termCols, unsigned short _termLines) {
    int termCols{_termCols}; 
    int termLines{_termLines - 1}; // -1 pour la ligne de statut en bas   
    int startX, startY {};
    v2d tmpSize{}, tmpPos{};

    for (auto wRow : widgets) {
        startX = 0;  
        for (auto w : wRow) {
            tmpSize = {};
            if (w->modeH == wSizeMode::MODE_FIX)
                tmpSize.x = w->fixedH;
            else if (w->modeH == wSizeMode::MODE_FULL)
                tmpSize.x = termCols - startX;
            else
                tmpSize.x = termCols * widgetSizeRatio[(int)w->modeH];

            if (w->modeV == wSizeMode::MODE_FIX)
                tmpSize.y = w->fixedV;
            else if (w->modeV == wSizeMode::MODE_FULL)
                tmpSize.y = termLines - startY;
            else
                tmpSize.y = termLines * widgetSizeRatio[(int)w->modeV];

            // On clean pour les arrondis éventuels
            if (tmpSize.x + tmpPos.x > termCols)
                tmpSize.x = termCols - tmpPos.x;
            if (tmpSize.y + tmpPos.y > termLines)
                tmpSize.y = termLines - tmpPos.y;

            // On a la position en cours
            w->setPos({startX, startY});

            // On enregistre la taille du widget
            w->setSize(tmpSize);

            // Update windows pos and sizes in curses
            w->refreshPosAndSize();

            //swprintf(debugText, 256, L"%ls%ls [%d, %d] (%dx%d)\n", debugText, w->title.c_str(), startX, startY, tmpSize.x, tmpSize.y); 

            // On décale le tout pour le prochain widget
            startX += tmpSize.x;
        }
        startY += tmpSize.y;
    }
}

void WidgetManager::freeWidgets() {
    for (auto wRow : widgets) {
        for (auto w : wRow) {
            delete w;
        }
    }
}

// =====================================
