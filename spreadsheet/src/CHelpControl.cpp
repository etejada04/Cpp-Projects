/**
 * @file CHelpControl.cpp
 * @author tejadede
 * @brief Help controller file
 */
#include "CHelpControl.h"
void CHelpControl::MakeView() {
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    attron(A_BOLD | A_UNDERLINE);
    mvprintw(0, 1, "CONTROLS:");
    attroff(A_BOLD | A_UNDERLINE);
    mvprintw(1, 6, "UP KEY (Move up)");
    mvprintw(2, 6, "DOWN KEY (Move down)");
    mvprintw(3, 6, "RIGHT KEY (Move right)");
    mvprintw(4, 6, "LEFT KEY (Move left)");
    mvprintw(5, 6, "'i' (Insert)");
    attron(A_BOLD | A_UNDERLINE);
    mvprintw(7, 1, "SUPPORTED MATHEMATICAL FUNCTIONS:");
    attroff(A_BOLD | A_UNDERLINE);
    mvprintw(8, 6, "Remember to use '=' before any mathematical expression.");
    mvprintw(9, 6, "Otherwise, it will be considered just as a string with no meaningful value.");
    mvprintw(10, 6, "An ERROR exception will be thrown to warn the user about those cases.");
    mvprintw(11, 6, "For example: '= 150' is a number but '150' is a string.");
    mvprintw(12, 6, "The program supports the following functions to evaluate expressions:");
    mvprintw(13, 8, "sum(x): sum (Format: =sum(R;C:R;C), where R and C are the row and column number)");
    mvprintw(14, 8, "avg(x): average (Format: =avg(R;C:R;C), where R and C are the row and column number)");
    mvprintw(15, 8, "Trigonometric functions: sin(x): sine | cos(x): cosine | tan(x): tangent");
    mvprintw(16, 8, "abs(x): absolute value");
    mvprintw(17, 8, "exp(x): exponential function");
    mvprintw(18, 8, "log(x): logarithm base 10");
    mvprintw(19, 8, "ln(x): natural logarithm");
    mvprintw(20, 8, "pow(x, y): x^y");
    mvprintw(21, 8, "cell(x, y): cell value (Format: =cell(R;C), where R and C are the row and column number)");
    mvprintw(22, 8, "sqrt(x): square root");
    attron(A_BOLD | A_UNDERLINE);
    mvprintw(24, 1, "SUPPORTED OPERATORS:");
    attroff(A_BOLD | A_UNDERLINE);
    mvprintw(25, 6, "The program supports the following operators: +, -, *, /, mod (%)");
    attron(A_BOLD | A_UNDERLINE);
    mvprintw(27, 1, "SAVE/LOAD FILES:");
    attroff(A_BOLD | A_UNDERLINE);
    mvprintw(28, 6, "It is also possible to save and load files. All files are stored in the \"/example\" directory.");mvprintw(28, 6, "It is also possible to save and load files. All files are stored in the \"/example\" directory.");
    mvprintw(29, 6, "Files are stored in the following format: Row, Column, Expression.");
    mvprintw(30, 6, "Controls:");
    mvprintw(31, 8, "'g' (Save file)");
    mvprintw(32, 8, "'l' (Load file)");
    attrset(COLOR_PAIR(1));
    mvprintw(maxY - 1, 1, " 's' to go to the spreadsheet ");
    mvprintw(maxY - 1, 32, " 'q' to exit the program ");
    attroff(COLOR_PAIR(1));
    refresh();
}