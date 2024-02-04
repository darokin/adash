#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <sys/time.h> // gettimeofday()

#ifdef _WIN32
	//#include <windows.h>
	#define NCURSES_WIDECHAR 1
    #include <ncursesw/ncurses.h>
#else // __unix__
	#include <unistd.h> // usleep()
    #include <ncursesw/curses.h>
#endif

#include "screen.hpp"
#include "widgetManager.hpp"
#include "widget.hpp"
#include "widgetClock.hpp"
#include "widgetDeco.hpp"
#include "widgetANSI.hpp"
#include "utils.hpp"

#define STATUS_TEXT_MAXLENGTH	2561

WINDOW *wmain, *wstatus;

#define DEBUG 1
#ifdef DEBUG
	WINDOW *wdDebug;
	wchar_t debugText[256];
#endif

WidgetManager* wmgr;

//std::vector<Widget*> widgets;
wchar_t statusText[STATUS_TEXT_MAXLENGTH];
v2d termSize;
static int statusTextPosx {1}; 

long long timeInMilliseconds() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

//namespace ScreenWidget {
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
	if (!wmgr->addWidget(1, widgetHeader, wSizeMode::MODE_FULL, wSizeMode::MODE_FIX, 0, 8)) {
		std::cout << "ERROR adding widget header" << std::endl;
	}
	// ================================================
	// == ROW 2 : Pattern FIX + Clock FLOAT               
	// COLOR
	/*      
	WidgetDeco* widgetColor = new WidgetDeco(L"Color");
	widgetColor->setType(decoType::FULL_BG);
	widgetColor->setColorPair(colorPairs::YELLOW_ON_BLACK);
	if (!wmgr->addWidget(2, widgetColor, wSizeMode::MODE_4, wSizeMode::MODE_FIX, 18, 11)) {
		std::cout << "ERROR adding widget color" << std::endl;
	}
	*/
	WidgetANSI* widgetAnsi = new WidgetANSI(L"ANSI widget", "./adash_out01.ans");
	if (!wmgr->addWidget(2, widgetAnsi, wSizeMode::MODE_4, wSizeMode::MODE_FIX, 18, 11)) {
		std::cout << "ERROR adding widget ANSI" << std::endl;
	}
	
	WidgetClock* widgetClock = new WidgetClock(L"Clock");
	widgetClock->setColorPair(colorPairs::YELLOW_ON_BLACK);
	if (!wmgr->addWidget(2, widgetClock, wSizeMode::MODE_6, wSizeMode::MODE_FIX, 0, 11)) {
		std::cout << "ERROR adding widget clock" << std::endl;
	}
	/*
	WidgetDeco* widgetColor2 = new WidgetDeco(L"Color");
	widgetColor2->setType(decoType::FULL_BG);
	widgetColor2->setColorPair(colorPairs::YELLOW_ON_BLACK);
	if (!wmgr->addWidget(2, widgetColor2, wSizeMode::MODE_FULL, wSizeMode::MODE_FIX, 18, 11)) {
		std::cout << "ERROR adding widget color2" << std::endl;
	}
	*/
	// ================================================
	// == ROW 2 : Third     with 20$ heigh
	// COLOR      
	/*
	WidgetDeco* widgetThird1 = new WidgetDeco(L"widgetThird1");
	widgetThird1->setColorPair(colorPairs::BLACK_ON_BLUE);
	if (!wmgr->addWidget(3, widgetThird1, widgetModeHorizontal::MODE_33, widgetModeVertical::MODE_FIX, 0, 4)) {
		std::cout << "ERROR adding widget widgetThird1" << std::endl;
	}
	Widget* widgetThird2 = new Widget(L"widgetThird2");
	if (!wmgr->addWidget(3, widgetThird2, widgetModeHorizontal::MODE_33, widgetModeVertical::MODE_FIX, 0, 4)) {
		std::cout << "ERROR adding widget widgetThird2" << std::endl;
	}
	Widget* widgetThird3 = new Widget(L"widgetThird3");
	if (!wmgr->addWidget(3, widgetThird3, widgetModeHorizontal::MODE_FULL, widgetModeVertical::MODE_FIX, 0, 4)) {
		std::cout << "ERROR adding widget widgetThird3" << std::endl;
	}
	*/

	// ================================================
	// == ROW 4 : Big text float / pattern FULL FLOAT TO BOTTOM
	WidgetDeco* widgetBigText = new WidgetDeco(L"Big text");
	widgetBigText->setType(decoType::CHECKBOARD);
	widgetBigText->setType(decoType::STRIPE);
	widgetBigText->setColorPair(colorPairs::PINK_ON_BLACK);
	widgetBigText->setColorPair(colorPairs::YELLOW_ON_BLACK);
	if (!wmgr->addWidget(3, widgetBigText, wSizeMode::MODE_8, wSizeMode::MODE_FULL, 0, 0)) {
		std::cout << "ERROR adding widget big text" << std::endl;
	}
	Widget* widgetLong = new Widget(L"Long vert pattern");
	widgetLong->setColorPair(colorPairs::YELLOW_ON_BLACK);
	if (!wmgr->addWidget(3, widgetLong, wSizeMode::MODE_FULL, wSizeMode::MODE_FULL, 0, 0)) {
		std::cout << "ERROR adding widget long" << std::endl;
	}
	

	wmgr->refreshWidgetsSizes();
	wrefresh(stdscr);
	//widgets.push_back(new Widget(L"F ♞ F", {1, 1}, {8, 6}));
	//widgets.push_back(new Widget(L"Test 中自有黄 #1", {11, 1}, {20, 6}));
	//widgets.push_back(new Widget(L"Second 🙏🙏 wmain \u265E", {1, 8}, {30, 8}));
	//widgets.push_back(new Widget(L"..ONLY..", {2, 4}, {10, 5}));
}

void drawWidgets() {
	wmgr->drawWidgets();
}
//}

void drawStatusBar() {
	werase(wstatus); // reset position
	//wbkgd(wstatus, COLOR_PAIR(colorPairs::YELLOW_ON_BLACK));
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
		start_color();
	};
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
	
	/*
	wtest = newwin(3, 20, 10, 10);
	wattron(wtest, COLOR_PAIR(colorPairs::BLUE_ON_WHITE));
	wmove(wtest, 0, 3);
	//waddwstr(wtest, L"Ok test test");
	wprintw(wtest, "%ls", L"Ok test test");
	wrefresh(wtest);
	
	wtest2 = newwin(5, 12, 18, 2);
	wattron(wtest2, COLOR_PAIR(colorPairs::BLUE_ON_WHITE));
	wmove(wtest2, 0, 3);
	//waddwstr(wtest, L"Ok test test");
	wprintw(wtest2, "%ls", L"TEST 2  TEST  2    TEST 2 ");
	wrefresh(wtest2);
	*/	
	
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
			//case 27: break;
			case KEY_RESIZE:
				//swprintf(debugText, 256, L"Resize triggered"); 
				screenResize();
				// TODO : mouve debug win
				wmgr->refreshWidgetsSizes();
				wrefresh(stdscr);
				// Refresh la status bar ou toust le stdscr une fois ?
				// refaire les calculs avec le bon calcul du reste etc.
				break;
			default:
				;//snprintf(str, STATUS_MESSAGE_SIZE, "Key : '%d' / '%c'", keycode, (char)keycode);
		}

	} while(keycode != 27);
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

	//statusText = L("A-DASH v0.1 [" << termSize.x << ", " << termSize.y << "]");
	//std::cout << "RESIZE [" << termSize.x << ", " << termSize.y << "]" << std::endl;

	std::ostringstream logMsg;
	logMsg << "RESIZE [" << termSize.x << ", " << termSize.y << "]" << std::endl;
	Utils::Log(logMsg.str());
	//swprintf(statusText, STATUS_TEXT_MAXLENGTH, L"EFadgasdfgsa"); 
	//swprintf ( statusText, 100, L"The half of %d is %d", 80, 80/2 );
	//snprintf(sstatusr, STATUS_RIGHT_MESSAGE_SIZE, "LINES = %d   CHARS = %d   [%dx%d]\n", fc->nbLines, fc->nbChars, termw, termh);

	//printf("resize signal new size = [%d , %d]\n", w, h);
	//sprintf(global.debug, "[%d , %d]", w, h);


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
