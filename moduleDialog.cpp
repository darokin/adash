#include "moduleDialog.hpp"

int endWordInHowManyChar(std::wstring wstr, size_t wordCountStart) {
    size_t _strLenght = wstr.length();
    if (wordCountStart >= _strLenght)
        return 0;
    for (size_t i = wordCountStart; i < _strLenght; i++) {
        if (wstr[i] == L' ' || wstr[i] == L'\n')
            return i - wordCountStart;
    }
    return _strLenght - wordCountStart;
}

ModuleDialog::ModuleDialog(WINDOW* _win, unsigned short _nbColumns, unsigned short _lineMaxForced) 
    : win {_win}, nbColumns {_nbColumns}, lineMaxForced {_lineMaxForced} {}

void ModuleDialog::draw(long long _timeCurrent) {
    //const wchar_t* cursBlock = L"█";
    unsigned short nbCharsToShow {0};
    unsigned short x {0};
    unsigned short y {(unsigned short)this->startPos.y};
    unsigned short xdec {0}; // dec if centered
    unsigned short slen {0}; // current string length
    nbCharsToShow = (_timeCurrent - this->seqTimeStart) / this->msPerChar;
    if (nbCharsToShow > this->sequences[this->seqIndex]->nbChars)
        nbCharsToShow = this->sequences[this->seqIndex]->nbChars;
    for (auto s : this->sequences[this->seqIndex]->lines) {
        slen = wcslen(s);
        xdec = (this->centered ? (this->nbColumns - slen) / 2 : 0);
        if (wcslen(s) > nbCharsToShow) {
            mvwaddnwstr(win, y, this->startPos.x + xdec, s, nbCharsToShow);
            x = nbCharsToShow + this->startPos.x + xdec;
            y++;
            break;    
        }
        mvwaddwstr(win, y, this->startPos.x + xdec, s);
        nbCharsToShow -= slen;
        x = this->startPos.x + xdec + slen - 1; 
        y++;
    } 
    if (this->blink)
        mvwaddwstr(win, y - 1, x, ((int(_timeCurrent / this->msPerChar) % 2) ? cursBlock : L"_"));
    /*
    std::wstringstream wss;
    wss << L"sequences["<<this->seqIndex<<"]->nbChars = ["<< this->sequences[this->seqIndex]->nbChars << "]";
    wss << L"StartTime = '" << this->seqTimeStart << "' nbCharsToShow [" << nbCharsToShow << "] ; _timeCurrent = '" << _timeCurrent << "'";
    mvwaddnwstr(this->win, 3, 1, wss.str().c_str(), 129);
    */
}

void ModuleDialog::dialogPrev(long long _timeCurrent) {
    this->seqTimeStart = _timeCurrent;
    if (this->seqIndex > 0)
        this->seqIndex--;
}

void ModuleDialog::dialogNext(long long _timeCurrent) {
    unsigned short nbCharsToShow;
    
    nbCharsToShow = (_timeCurrent - this->seqTimeStart) / this->msPerChar;
    if (nbCharsToShow < this->sequences[this->seqIndex]->nbChars) {
        this->seqTimeStart -= this->sequences[this->seqIndex]->nbChars * this->msPerChar;
        return;
    }

    if (this->seqIndex >= this->sequences.size() - 1)
        return;
     
    this->seqTimeStart = _timeCurrent;
    this->seqIndex++;
}

void ModuleDialog::initText(std::vector<std::wstring>& _lines) {
    dialogSequence* newDialogSeq;
    wchar_t* newLine;
    unsigned short lineCount {0};
    unsigned short charCount {0};
    unsigned short endWord;
    wchar_t wc;
    const wchar_t seqCharSeparator {L'-'};
    
    newDialogSeq = new dialogSequence{ {}, 0, 0 };
    this->sequences.push_back(newDialogSeq);

    newLine = new wchar_t[this->nbColumns+1];
    for (size_t i = 0; i < _lines.size(); i++) {
        // == Test if on a dialog separator 
        if (_lines[i].length() < 3 && _lines[i][0] == seqCharSeparator) { 
            sequences.back()->nbLines = lineCount;
            if (lineCount > lineMax)
              lineMax = lineCount;
            lineCount = 0;
            newDialogSeq = new dialogSequence{ {}, 0, 0 };
            sequences.push_back(newDialogSeq);
            continue;
        }
        // == Empty line
        if (_lines[i].length() <= 0) {
            newLine[0] = L'\0';
            sequences.back()->lines.push_back(newLine);
            sequences.back()->nbChars++;
            newLine = new wchar_t[nbColumns+1];
            lineCount++;
        }
        for (size_t ichar = 0; ichar < _lines[i].length(); ichar++) {
            wc = _lines[i][ichar];
            // == Skip unlucky space at the beginning of the line jump
            if (charCount == 0 && ichar > 0 && wc == L' ' && _lines[i].length() > 1)
              continue;
            // == Copy char by char
            newLine[charCount] = wc;
            charCount++;
            // == Test saut de ligne 
            endWord = endWordInHowManyChar(_lines[i], ichar+1);//charCount);
            if ((endWord + charCount > nbColumns && wc == L' ') || charCount >= nbColumns || ichar == _lines[i].length()-1) {
                // == Closing line, pushing it and starting a new one
                newLine[charCount] = L'\0';
                sequences.back()->lines.push_back(newLine);
                sequences.back()->nbChars += charCount;
                newLine = new wchar_t[nbColumns+1];
                lineCount++;
                charCount = 0;
            }
        }
        // == Test si on dépasse le nb de line max autorisée
        if (this->lineMaxForced > 0 && sequences.back()->lines.size() > 0 && sequences.back()->lines.size() % this->lineMaxForced == 0) {
            lineCount = 0;
            newDialogSeq = new dialogSequence{ {}, 0, 0 };
            sequences.push_back(newDialogSeq);
        }
    }

    // == Fin de la dernière dialogSequence 
    this->sequences.back()->nbLines = lineCount;
    if (lineCount > lineMax)
        this->lineMax = lineCount;
}

