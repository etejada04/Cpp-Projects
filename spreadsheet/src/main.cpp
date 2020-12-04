/**
 * @file main.cpp
 * @author tejadede
 * @brief Main file of the spreadsheet
 * @mainpage Crappy Excel
 * @section title Simple Spreadsheet editor
 * This project was created by Eder Jair Tejada Ortigoza (tejadede@fit.cvut.cz)
 * for the course Programming and Algorithms 2.
 * @subsection desc Description
 * The program consist in the implementation of a simple spreadsheet
 * editor using ncurses. It supports the following operators:  +, -, *, /, mod.
 * Despite basic operation it is also possible to use mathematical functions such
 * as: sum, average, trigonometric functions, absolute value, square root or
 * logarithm. Another feauture is file serialization, which makes possible to read and load
 * files, all files all stored under the directory "/examples".
 * @subsection ctrl Controls:
 * 's' to start a new spreadsheet\n
 * 'h' to display the help menu\n
 * 'q' to quit the program\n
 * 'i' to insert a value in a cell\n
 * 'g' to save the current file\n
 * 'l' to load a file into the spreadsheet\n
 * UP/DOWN/LEFT/RIGHT KEY to navigate around the spreadsheet\n
 */
#include "CRun.h"
int main() {
    CRun crappyExcel;
    crappyExcel.Start();
    return 0;
}