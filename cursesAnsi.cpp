#include <ncursesw/curses.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <locale.h>
#include "cursesAnsi.hpp"

namespace cursesANSI {
    enum class State {
    STATE_NONE,
    STATE_PRINT,
    STATE_ESCAPE,
    STATE_MODIFIER,
    STATE_MODIFIER_UNDO,
    STATE_MODIFIER_FG,
    STATE_MODIFIER_BG    
    };

    struct cursesColorPairs {
        int colorPairIndex;
        int colorFG;
        int colorBG;
    };

    static std::vector<cursesColorPairs> vcolors;
    static State state;// {0};
    static unsigned char prevChar;// {0};
    static int colorFG;// {-1};
    static int colorBG;// {-1};

    static int getColorPair(int fg, int bg) {
        for (auto col : vcolors) {
            if (col.colorFG == fg && col.colorBG == bg)
                return col.colorPairIndex;
        }
        int nID = vcolors.size() + 1;
        init_pair(nID, fg, bg);
        vcolors.push_back(cursesColorPairs{nID, fg, bg});
        return nID;
    }

    unsigned char feedChar(unsigned char c, WINDOW* _win) {
        unsigned char returnChar = 0;

    //    char tt[256]{};
    //    snprintf(tt, 256, "FEED CHAR (%c)[%d] prevChar = '%c'  STATE = '%d'  (mod=4)", c, (int)c, prevChar, state);
    //    mvwaddstr(_win, 40, 2, tt);
            
        switch (state) {
        // =============================================
        case (State::STATE_NONE):
        case (State::STATE_PRINT):
            // # PRINT
            if (c == ASCII_ESC) {
                state = State::STATE_MODIFIER;
                break;
            }
            // OK on retourne le caractère à afficher
            returnChar = c;
            break;
        // =============================================
        case State::STATE_ESCAPE:
            if (c == MOD_START) {
                state = State::STATE_MODIFIER;
            }
            break;
        case State::STATE_MODIFIER_UNDO:
        case State::STATE_MODIFIER_FG:
        case State::STATE_MODIFIER_BG:
        case State::STATE_MODIFIER:
            // # MODIFIER ANALYSIS
            if (c == MOD_END || c == MOD_SPLITTER) {
                // == On applique l'attribut SIMPLE
                if (state == State::STATE_MODIFIER) {
                    switch (prevChar)
                    {
                    case MOD_0_RESET:
                        {
                        wstandend(_win);
                        colorFG = -1;
                        colorBG = -1;
                        wattron(_win, COLOR_PAIR(getColorPair(colorFG, colorBG)));
                        }
                        break;
                    case MOD_1_BOLD:
                        wattron(_win, A_BOLD);
                        break;
                    case MOD_2_DIM:
                        wattron(_win, A_DIM);
                        break;
                    case MOD_3_ITALIC:
                        wattron(_win, A_ITALIC);
                        break;
                    case MOD_4_UNDERLINE:
                        wattron(_win, A_UNDERLINE);
                        break;
                    case MOD_5_BLINK:
                        wattron(_win, A_BLINK);
                        break;
                    case MOD_7_REVERSE:
                        wattron(_win, A_REVERSE);
                        break;
                    case MOD_8_HIDDEN:
                        wattron(_win, A_INVIS);
                        break;
                    case MOD_9_STRIKE:
                        wattron(_win, A_STANDOUT); // not strikethrough at all...
                        break;
                    default:
                        break;
                    }
                } else if (state == State::STATE_MODIFIER_FG) {
                    // Change FG
                    colorFG = ((int)prevChar - ASCII_DIGIT_START);
                    wattron(_win, COLOR_PAIR(getColorPair(colorFG, colorBG)));
                } else if (state == State::STATE_MODIFIER_BG) {
                    // Change BG
                    colorBG = ((int)prevChar - ASCII_DIGIT_START);
                    wattron(_win, COLOR_PAIR(getColorPair(colorFG, colorBG)));
                } else {
                    ;
                }
                
                // == Fin ou split de modifier !
                if (c == MOD_END) {
                    state = State::STATE_PRINT;
                } else { // SPLITTER, on fait comme si on faisait un nouveau modifier
                    c = MOD_START; // on reste dans le state 
                    state = State::STATE_MODIFIER;
                }
            } else {
                // On test si on est pas sur un modifieur complexe
                if (prevChar == MOD_COMPLEX_2_UNDO)
                    state = State::STATE_MODIFIER_UNDO;
                else if (prevChar == MOD_COMPLEX_3_FG)
                    state = State::STATE_MODIFIER_FG;
                else if (prevChar == MOD_COMPLEX_4_BG)
                    state = State::STATE_MODIFIER_BG; 
            }
            break;
        default:
            break;
        }

        prevChar = c;
        return returnChar;
    }

    bool mvwaddnANSIstr(WINDOW* _win, int _startY, int _startX, std::string _line, size_t _size) {
        int _x = _startX;
        unsigned char printChar;
        for (unsigned char c : _line) {
            if (c == ASCII_EOF)
                return false;

            mvwaddwstr(_win, _startY, _x, L"DEBUG ANSI CP437_unicode[(int)printChar - 128]");
            return true;
            
            printChar = feedChar(c, _win);
            if (printChar != 0) {
                // == ASCII char en Unicode ne ressortiront pas donc on les converti en Unicode le plus proche
                if ((int)printChar > 127 && (int)printChar < 256)
                    mvwaddwstr(_win, _startY, _x, CP437_unicode[(int)printChar - 128]);//u8 = CP437_unicode[(int)uc - 128];
                else  
                    mvwaddch(_win, _startY, _x, printChar);
                _x++;
            }
        }
        return true;
    }; // end mvwaddnANSIstr()
}; // end namespace
