/**
 * @file CStartControl.cpp
 * @author tejadede
 * @brief Start controller file
 */
#include "CStartControl.h"
void CStartControl::MakeView() {
    int maxX, maxY;
    WINDOW * box = newwin(5, 10, 5, 20);
    clear();
    getmaxyx(stdscr, maxY, maxX);
    init_pair(2, COLOR_GREEN, COLOR_WHITE);
    attron(A_BOLD | COLOR_PAIR(2));
    mvprintw(10,15,"   _____                            _____                       _       ");
    mvprintw(11,15,"  / ____|                          |  ___| _    _  ____  ____  | |      ");
    mvprintw(12,15," | |    _ __ ____  ____  ____ _   _| |__  \\ \\__/ // ___|/    \\ | |      ");
    mvprintw(13,15," | |   | '_//    ||    \\|    \\ \\/ /|  __|  \\    / | |   | |_)| | |      ");
    mvprintw(14,15," | |__ | |  | (_||| (_||| (_||\\   /| |___  / /\\ \\ | |__ |  __/ | |      ");
    mvprintw(15,15," \\____||_|  \\__'_|| .__/| .__/ \\ / |_____|/_/  \\_\\\\____|\\____/ |_|      ");
    mvprintw(16,15,"                  | |   | |    / /                                      ");
    mvprintw(17,15,"                  |_|   |_|   /_/ All the best of Excel...(but crappier)");
    mvprintw(18,54,"Created by Eder Tejada (tejadede)");
    attroff(A_BOLD | COLOR_PAIR(2));
    attrset(COLOR_PAIR(1));
    mvprintw(maxY-1, 1, " 's' to start a new spreadsheet ");
    mvprintw(maxY-1, 35, " 'q' to exit the program ");
    attroff(COLOR_PAIR(1));
    wrefresh(box);
    refresh();
}