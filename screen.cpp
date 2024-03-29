#include <iostream>
#include <sstream>
#include <vector>

#ifdef _WIN32
	//#include <windows.h>
	#define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#else // __unix__
	#include <unistd.h> // usleep()
    #include <ncursesw/curses.h>
	#include <ncursesw/menu.h>
#endif

#include "screen.hpp"
#include "widgetManager.hpp"
#include "widget.hpp"
#include "widgetClock.hpp"
#include "widgetDeco.hpp"
#include "widgetANSI.hpp"
#include "widgetTextFile.hpp"
#include "widgetDialog.hpp"
#include "utils.hpp"

#define STATUS_TEXT_MAXLENGTH	256

#define DEBUG 1
#ifdef DEBUG
	WINDOW *wdDebug;
	wchar_t debugText[256];
#endif

enum class ScreenState {
	NORMAL,
	TABSELECTION,
	WIDGETACTIVE
};

WINDOW *wmain, *wstatus;
WidgetManager* wmgr;
wchar_t statusText[STATUS_TEXT_MAXLENGTH];
static int statusTextPosx {1}; 
v2d termSize;
static ScreenState screenState {ScreenState::NORMAL};





//namespace ScreenWidget {

/*
void initWidgets() {
	
	wmgr = WidgetManager::getInstance();
	
	// TODO : faire une fonction de cleanup après les affectations.
	// pour laisser les user mettre 33% puis 66% sur une ligne
	// mais parourir pour cleaner tous les vecteurs de widget et forcer le dernier en mode full 
	// pour que le calcul se fasse bien.
	
	// ================================================
	// == HEADER ROW 1
	WidgetDeco* widgetHeader = new WidgetDeco(L"HEADER ♞");
	widgetHeader->setType(decoType::STRIPE);
	widgetHeader->setColorPair(colorPairs::YELLOW_ON_BLACK);
	if (!wmgr->addWidget(1, widgetHeader, wSizeMode::MODE_FULL, wSizeMode::MODE_FIX, 0, 4)) {
		std::cout << "ERROR adding widget header" << std::endl;
	}
	// ================================================
	// == ROW 2 : Pattern FIX + Clock FLOAT               
	// COLOR    
	// WidgetDeco* widgetColor = new WidgetDeco(L"Color");
	// widgetColor->setType(decoType::FULL_BG);
	// widgetColor->setColorPair(colorPairs::YELLOW_ON_BLACK);
	// if (!wmgr->addWidget(2, widgetColor, wSizeMode::MODE_4, wSizeMode::MODE_FIX, 18, 11)) {
	// 	std::cout << "ERROR adding widget color" << std::endl;
	// }
	
	WidgetANSI* widgetAnsi = new WidgetANSI(L"ANSI widget", "./adash_out01.ans");
	if (!wmgr->addWidget(2, widgetAnsi, wSizeMode::MODE_4, wSizeMode::MODE_FIX, 18, 17)) {
		std::cout << "ERROR adding widget ANSI" << std::endl;
	}
	
	WidgetClock* widgetClock = new WidgetClock(L"Clock");
	widgetClock->setColorPair(colorPairs::YELLOW_ON_BLACK);
	widgetClock->setBorder(false);
	if (!wmgr->addWidget(2, widgetClock, wSizeMode::MODE_FULL, wSizeMode::MODE_FIX, 0, 17)) {
		std::cout << "ERROR adding widget clock" << std::endl;
	}
	
	// WidgetDeco* widgetColor2 = new WidgetDeco(L"Color");
	// widgetColor2->setType(decoType::FULL_BG);
	// widgetColor2->setColorPair(colorPairs::YELLOW_ON_BLACK);
	// if (!wmgr->addWidget(2, widgetColor2, wSizeMode::MODE_FULL, wSizeMode::MODE_FIX, 18, 11)) {
	// 	std::cout << "ERROR adding widget color2" << std::endl;
	// }
	
	// ================================================
	// == ROW 2 : Third     with 20$ heigh
	// COLOR      
	
	// WidgetDeco* widgetThird1 = new WidgetDeco(L"widgetThird1");
	// widgetThird1->setColorPair(colorPairs::BLACK_ON_BLUE);
	// if (!wmgr->addWidget(3, widgetThird1, widgetModeHorizontal::MODE_33, widgetModeVertical::MODE_FIX, 0, 4)) {
	// 	std::cout << "ERROR adding widget widgetThird1" << std::endl;
	// }
	// Widget* widgetThird2 = new Widget(L"widgetThird2");
	// if (!wmgr->addWidget(3, widgetThird2, widgetModeHorizontal::MODE_33, widgetModeVertical::MODE_FIX, 0, 4)) {
	// 	std::cout << "ERROR adding widget widgetThird2" << std::endl;
	// }
	// Widget* widgetThird3 = new Widget(L"widgetThird3");
	// if (!wmgr->addWidget(3, widgetThird3, widgetModeHorizontal::MODE_FULL, widgetModeVertical::MODE_FIX, 0, 4)) {
	// 	std::cout << "ERROR adding widget widgetThird3" << std::endl;
	// }
	

	// ================================================
	// == ROW 4 : Big text float / pattern FULL FLOAT TO BOTTOM
	WidgetTextFile* widgetBigText = new WidgetTextFile(L"Big text", "./emoji-data.txt");
	widgetBigText->setColorPair(colorPairs::BLUE_ON_WHITE);
	if (!wmgr->addWidget(3, widgetBigText, wSizeMode::MODE_8, wSizeMode::MODE_FULL, 0, 0)) {
		std::cout << "ERROR adding widget big text" << std::endl;
	}
	Widget* widgetLong = new Widget(L"Long vert pattern");
	widgetLong->setColorPair(colorPairs::YELLOW_ON_BLACK);
	if (!wmgr->addWidget(3, widgetLong, wSizeMode::MODE_FULL, wSizeMode::MODE_FULL, 0, 0)) {
		std::cout << "ERROR adding widget long" << std::endl;
	}
	

	wmgr->refreshWidgetsSizes(termSize.x, termSize.y);
	wrefresh(stdscr);
	// ♞ F中自有黄 ond 🙏🙏 wmain \u265E"
}
*/


void initWidgets() {
	
	wmgr = WidgetManager::getInstance();
	
	// ================================================
	// == HEADER ROW 1
	WidgetDeco* wHeader = new WidgetDeco(L"HEADER ♞");
	WidgetClock* wClock = new WidgetClock(L"CLOCK");
	//WidgetDialog* wContent = new WidgetDialog(L"Content 🙏", "makefile"); //Widget(L"Content 🙏");

	
	WidgetDialog* wDialog = new WidgetDialog(L"INTRO", "test.txt");
	int dialSizeX = 44;
	int dialSizeY = 12;
	wDialog->setPos({2, 2}); //(termSize.x / 2) - (dialSizeX / 2), (termSize.y / 2) - (dialSizeY / 2)});
	wDialog->setSize({dialSizeX, dialSizeY});

	wHeader->setType(decoType::STRIPE);
	wHeader->setColorPair(colorPairs::YELLOW_ON_BLACK);

	wmgr->addWidget(1, wHeader, wSizeMode::MODE_FIX, wSizeMode::MODE_FIX, 84, 4);

	wmgr->addWidget(2, wClock, wSizeMode::MODE_FIX, wSizeMode::MODE_FIX, 84, 9);

	WidgetANSI* widgetAnsi = new WidgetANSI(L"ANSI widget", "./adash_out01.ans");
	wmgr->addWidget(3, widgetAnsi, wSizeMode::MODE_FIX, wSizeMode::MODE_FIX, 84, 42);//24 - 13);
	//wmgr->addWidget(3, wContent, wSizeMode::MODE_FIX, wSizeMode::MODE_FIX, 80, 24 - 13);

	//wmgr->addWidget(0, wDialog, wSizeMode::MODE_FIX, wSizeMode::MODE_FIX, dialSizeX, dialSizeY);

	wmgr->refreshWidgetsSizes(termSize.x, termSize.y);

	//wContent->addDialog();
	wDialog->addDialog();

	wrefresh(stdscr);
}


void drawWidgets() {
	wmgr->drawWidgets();
}

void drawStatusBar() {
	werase(wstatus); // reset position
	wbkgd(wstatus, COLOR_PAIR(colorPairs::YELLOW_ON_BLACK));
	wbkgd(wstatus, COLOR_PAIR(colorPairs::BLACK_ON_WHITE) | A_REVERSE);
	//wattron(wstatus, COLOR_PAIR(colorPairs::YELLOW_ON_BLACK));
	wmove(wstatus, 0, 2);//statusTextPosx);
	wprintw(wstatus, "%ls", statusText);
	//waddstr(wstatus, "gsfdgsfg");
	wrefresh(wstatus);
}

void drawDebug() {
	werase(wdDebug);
	wbkgd(wdDebug, COLOR_PAIR(colorPairs::YELLOW_ON_WHITE));
	box(wdDebug, 0, 0);
	wmove(wdDebug, 1, 1);
	wprintw(wdDebug, "%ls", debugText);
	wrefresh(wdDebug);
}

void screenInit() {
    std::cout << "InitScreen()" << std::endl;

	// == Init screen
	initscr();
	wclear(stdscr);
	cbreak(); 				// one char at a time (do not buffer till enter)
	curs_set(FALSE);		// hide cursor
	noecho(); 				// suppress echoing of keys input
	keypad(stdscr, TRUE); 	// allow getch() to listen to arrows and other special keys
	nodelay(stdscr, TRUE);	// non blocking getch()
    nonl();                 // no new line / catch enter key

	// == Init color pairs
	if (has_colors()) {     // is it necessary ?
		use_default_colors();
		start_color();
	};
	init_pair(colorPairs::DEFAULT, -1, -1);
	init_pair(colorPairs::BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);
	init_pair(colorPairs::WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair(colorPairs::YELLOW_ON_WHITE, COLOR_YELLOW, COLOR_WHITE);
	init_pair(colorPairs::YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
	init_pair(colorPairs::BLACK_ON_YELLOW, COLOR_BLACK, COLOR_YELLOW);
	init_pair(colorPairs::PINK_ON_WHITE, COLOR_MAGENTA, COLOR_WHITE);	
	init_pair(colorPairs::PINK_ON_BLACK, COLOR_MAGENTA, COLOR_BLACK);	
	init_pair(colorPairs::BLACK_ON_PINK, COLOR_BLACK, COLOR_MAGENTA);	
	init_pair(colorPairs::BLUE_ON_WHITE, COLOR_BLUE, COLOR_WHITE);
	init_pair(colorPairs::BLUE_ON_BLACK, COLOR_BLUE, COLOR_BLACK);
	init_pair(colorPairs::BLACK_ON_BLUE, COLOR_BLACK, COLOR_BLUE);
    
	// == Init term size by faking a resize
	screenResize();

	// == Create main wmaindow and initialize default style
	//wMain = newwmain(viewSize.y, viewSize.x, 0, 0);
	wstatus = newwin(1, termSize.x, termSize.y - 1, 0);
	//drawStatusBar(); // draw the first time
	#ifdef DEBUG
		wdDebug = newwin(8, 60, termSize.y - (8 + 2), termSize.x - (60 + 2));
	#endif
	
    initWidgets();
}

void screenLoop() {
    int keycode;
	
	
	/*
	long long timeStart, timeSeqStart, timeCurrent;
	long timeLapsedMs;

	timeStart = timeInMilliseconds();	// TODO : check if useful...
	timeSeqStart = timeStart;
	
	//auto startTime = std::chrono::steady_clock::now();
	//auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
    //napms(WAIT_TIME - diff.count());
	*/
	bool bQuit = false;

    do {
		msleep(80);
	
		//timeCurrent = timeInMilliseconds();
		//timeLapsedMs = (long) (timeCurrent - timeSeqStart);
        
		// =================================================
		drawWidgets();
		drawStatusBar();
		//wrefresh(stdscr);
		#ifdef DEBUG
//			drawDebug();
		#endif
		// ============================================
		// == KEY LISTENING
		keycode = wgetch(stdscr);
		if (keycode == -1)
			continue;

		wmgr->handleKey(keycode);

		switch (keycode) {
			case KEY_LEFT: 
				// == PREVIOUS
				//action = ACTION_PREVIOUS;
				if (statusTextPosx > 0)
					statusTextPosx--;
				drawStatusBar();
				break;
			case KEY_RIGHT: 
				// == NEXT
				//action = ACTION_NEXT;
				statusTextPosx++;
				drawStatusBar();
				break;
			case 9: 
				// TABULATION
				if (screenState != ScreenState::WIDGETACTIVE)
					screenState = ScreenState::TABSELECTION;
			case KEY_RESIZE:
				//swprintf(debugText, 256, L"Resize triggered"); 
				screenResize();
				// TODO : mouve debug win
				wmgr->refreshWidgetsSizes(termSize.x, termSize.y);
				wrefresh(stdscr);
				// Refresh la status bar ou toust le stdscr une fois ?
				// refaire les calculs avec le bon calcul du reste etc.
				break;
			case 27: // ESCAPE
				if (screenState != ScreenState::NORMAL)
					screenState = ScreenState::NORMAL;
				else
					bQuit = true;
				break;
			default:
				swprintf(statusText, STATUS_TEXT_MAXLENGTH, L"Key '%d' '0x%x' [%c]", keycode, keycode, (char)keycode);
				wrefresh(wstatus);
		}

	} while(!bQuit);//keycode != 27);
}

/*
void screenRefresh() {
    wclear(wmain);

    wbkgd(wmain, COLOR_PAIR(1));
	wattron(wmain, COLOR_PAIR(1) | A_BOLD);
	wmove(wmain, 0, 4);
	wprintw(wmain, "ADASH");//, sfilepath);
	wrefresh(wmain);

    //drawWidgets();
    
    wrefresh(stdscr);
}
*/

void screenResize() {
	int w, h;
	getmaxyx(stdscr, h, w);
	termSize.x = w;
	termSize.y = h;
	swprintf(statusText, STATUS_TEXT_MAXLENGTH, L" \u265E A-DASH ♞ v0.1 w = [%d, %d]", termSize.x, termSize.y); 
/*
	std::ostringstream logMsg;
	logMsg << "RESIZE [" << termSize.x << ", " << termSize.y << "]" << std::endl;
	Utils::Log(logMsg.str());
*/
}

void screenExit() {
	//wclear(wmain);
	wclear(stdscr); 
	endwin();
	echo();
	nodelay(stdscr, FALSE);
	curs_set(true);
	refresh();
}
