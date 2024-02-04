#include <stdexcept>
#include "widgetManager.hpp"
#include "widget.hpp"

extern wchar_t debugText[256];

WidgetManager* WidgetManager::widgetmgr = nullptr;
WidgetManager* WidgetManager::getInstance() {
    if (widgetmgr == nullptr) {
        widgetmgr = new WidgetManager();
    }
    return widgetmgr;
}

void WidgetManager::drawWidgets() {
    for (auto wRow : widgets) {
        for (auto w : wRow) {
            w->mainDraw();
        }
    }
}

bool WidgetManager::addWidget(unsigned short _row, Widget* _widget, wSizeMode _mh, wSizeMode _mv, unsigned int _h, unsigned int _v) {
    std::vector<Widget*> rowOfWidget;
    
    // TODO : faire plus propre...
    _widget->modeH = _mh;
    _widget->modeV = _mv;
    _widget->fixedH = _h;
    _widget->fixedV = _v;

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

void WidgetManager::refreshWidgetsSizes() {
    int startX {};
    int startY {};

    //swprintf(debugText, 256, L"\0"); 

    int termCols{};
    int termLines{};
    getmaxyx(stdscr, termLines, termCols);   
    termLines-=1;//-; // pour la ligne de statut en bas
    
    v2d tmpSize{}, tmpPos{};
    for (auto wRow : widgets) {
        // Pour chaque ligne on calcule la hauteur
        // D'abord si on a une ligne avant on prend en compte le décalage  
        startX = 0;  
        for (auto w : wRow) {
            tmpSize = {};
            // En fonction du mode on va mettre les valeurs de position et de taille à jour

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
/*
            // On calcule la largeur
            switch (w->modeV)
            {
            case wSizeMode::MODE_FIX:
                tmpSize.y = w->fixedV;
                break;
            case wSizeMode::MODE_2:
                tmpSize.y = termLines * 0.2;
                break;
            case wSizeMode::MODE_8:
                tmpSize.y = termLines * 0.2;
                break;
            case wSizeMode::MODE_33:
                tmpSize.y = termLines / 3.0;
                break;
            case wSizeMode::MODE_FULL:
                tmpSize.y = termLines - startY;
                break;
            default:
                break;
            }
*/
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
            //w->
            //swprintf()

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
