/**
 * @file CTableControl.cpp
 * @author tejadede
 * @brief Table controller file
 */
#include "CTableControl.h"
CTableControl::CTableControl(): m_row(1), m_col(1), m_rowInitPos(1), m_colInitPos(1){}

CTableControl::~CTableControl() {}

void CTableControl::KeyHandler(int key) {
    switch (key){
        case 'i':
            InsertExpression();
            break;
        case 'g':
            SaveFile();
            break;
        case 'l':
            LoadFile();
            break;
        case KEY_DOWN:
            ChangePosition(m_row+1, m_col);
            break;
        case KEY_UP:
            ChangePosition(m_row-1, m_col);
            break;
        case KEY_RIGHT:
            ChangePosition(m_row, m_col+1);
            break;
        case KEY_LEFT:
            ChangePosition(m_row, m_col-1);
            break;
        default:
            break;
    }
}

void CTableControl::MakeView() {
    clear();
    GetHeader();
    GetTable();
    GetMenu();
    refresh();
}

void CTableControl::GetPosition() const {
    move(0, 6);
    clrtoeol();
    move(1, 18);
    clrtoeol();
    attron(A_BOLD);
    mvprintw(0, 6, "(R%d:C%d):", m_row, m_col);
    attroff(A_BOLD);
    refresh();
}

void CTableControl::GetValue() const {
    move(0, 21);
    clrtoeol();
    mvprintw(0, 14, " %s", m_table.GetValue(m_row, m_col).c_str());
    refresh();
}

void CTableControl::GetExpression() const {
    move(2, 12);
    clrtoeol();
    mvprintw(2, 12, " %s", m_table.GetExpression(m_row, m_col).c_str());
    refresh();
}

void CTableControl::GetHeader() const {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    attron(A_BOLD);
    mvprintw(0, 1, "Cell");
    attroff(A_BOLD);
    GetPosition();
    GetValue();
    attron(A_BOLD);
    mvprintw(2, 1, "Expression:");
    attroff(A_BOLD);
    GetExpression();
    attron(A_BOLD);
    mvprintw(maxY-5, 1, "File name:");
    attroff(A_BOLD);
    refresh();
}

void CTableControl::GetTable() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    int rowNum = (maxY - 13) / 2;
    if ((rowNum + m_rowInitPos) < m_row) {
        m_rowInitPos = m_row - rowNum;
    } else if (m_row < m_rowInitPos) {
        m_rowInitPos = m_row;
    }

    int colNum = (maxX - 6) / 10;
    if ((colNum + m_colInitPos) <= m_col) {
        m_colInitPos = m_col - colNum + 1;
    } else if (m_col < m_colInitPos) {
        m_colInitPos = m_col;
    }

    attrset(COLOR_PAIR(3));
    int offset = 8;
    for (int i = m_rowInitPos; i < (rowNum + m_rowInitPos); ++i) {
        if (m_row == i || m_row == (i + 1)) {
            attrset(COLOR_PAIR(2));
        }
        move(offset, 0);
        hline(0, maxX);
        attroff(COLOR_PAIR(2));
        attrset(COLOR_PAIR(3));
        offset += 2;
    }

    offset = 5;
    for (int i = m_colInitPos; i <= (colNum + m_colInitPos); i++) {
        if (m_col == i || m_col == (i - 1)) {
            attrset(COLOR_PAIR(2));
        }
        move(5, offset);
        vline(0, maxY - 11);
        attroff(COLOR_PAIR(2));
        attrset(COLOR_PAIR(3));
        offset += 10;
    }
    attroff(COLOR_PAIR(3));

    move(4, 0);
    hline(0, maxX);
    move(6, 0);
    hline(0, maxX);

    // Vertical header
    offset = 7;
    for (int i = m_rowInitPos; i <= (rowNum + m_rowInitPos); i++) {
        if (m_row == i) {
            attrset(COLOR_PAIR(2));
        }
        attron(A_BOLD|COLOR_PAIR(1));
        mvprintw(offset, 0, "%4d ", i);
        attroff(A_BOLD|COLOR_PAIR(1));
        offset += 2;
    }

    // Horizontal header
    offset = 6;
    string str;
    int counter;
    for (int i = m_colInitPos; i <= (colNum + m_colInitPos); i++) {
        // Align data
        str = "";
        counter = (int) std::to_string(i).length();
        for (int j = 0; j < ((9 - counter) / 2); j++) {
            str.append(" ");
        }
        str.append(std::to_string(i));
        for (unsigned long int j = (str.length()); j < 9; j++) {
            str.append(" ");
        }

        if (m_col == i) {
            attrset(COLOR_PAIR(2));
        }
        if ((int) str.length() > (maxX - offset)) {
            attron(A_BOLD|COLOR_PAIR(1));
            mvprintw(5, offset, "%.*s", (maxX - offset), str.c_str());
            attroff(A_BOLD|COLOR_PAIR(1));
        } else {
            attron(A_BOLD|COLOR_PAIR(1));
            mvprintw(5, offset, "%s", str.c_str());
            attroff(A_BOLD|COLOR_PAIR(1));
        }
        attroff(COLOR_PAIR(2));
        offset += 10;
    }
    //Get data for the table
    int rowOffset = 7;
    for (int i = m_rowInitPos; i <= (rowNum + m_rowInitPos); i++) {
        int colOffset = 6;
        for (int j = m_colInitPos; j <= (colNum + m_colInitPos); j++) {
            // Align data
            str = "";
            counter = (int) m_table.GetValue(i, j).length();
            for (int k = 0; k < ((9 - counter) / 2); k++) {
                str.append(" ");
            }
            str.append(m_table.GetValue(i, j));
            for (unsigned long int k = (str.length()); k < 9; k++) {
                str.append(" ");
            }

            if (str.length() > 9) {
                str = str.substr(0, 6);
                str.append("...");
            }
            if (m_col == j && m_row == i) {
                attrset(COLOR_PAIR(2));
            }
            if ((int) str.length() > (maxX - colOffset)) {
                mvprintw(rowOffset, colOffset, "%.*s", (maxX - colOffset), str.c_str());
            } else {
                mvprintw(rowOffset, colOffset, "%s", str.c_str());
            }
            attroff(COLOR_PAIR(2));
            colOffset += 10;
        }
        rowOffset += 2;
    }
    refresh();
}

void CTableControl::GetMenu() const {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    attrset(COLOR_PAIR(1));
    mvprintw(maxY - 1, 1, " 'i' (insert) ");
    mvprintw(maxY - 1, 16, " 'h' (help) ");
    mvprintw(maxY - 1, 29, " 'q' (exit) ");
    mvprintw(maxY - 1, 42, " 'g' (save) ");
    mvprintw(maxY - 1, 55, " 'l' (load) ");
    attroff(COLOR_PAIR(1));
    refresh();
}

void CTableControl::ChangePosition(int row, int col) {
    if (row < 1 || col < 1) {
        return;
    } else {
        m_row = row;
        m_col = col;
        GetPosition();
        GetValue();
        GetTable();
        GetExpression();
    }
}

void CTableControl::InsertExpression() {
    echo();
    curs_set(1);
    move(2, 13);
    clrtoeol();
    char str[100];
    move(2, 13);
    string expression = m_table.GetExpression(m_row, m_col);
    for (int i = ((int) expression.length() - 1); i >= 0; --i) {
        ungetch(expression.at(i));
    }
    getnstr(str, 99);
    expression = str;
    m_table.Insert(m_row, m_col, expression);
    noecho();
    curs_set(0);
    GetValue();
    GetTable();
}

void CTableControl::SaveFile(){
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    echo();
    curs_set(1);
    move(maxY-5, 12);
    clrtoeol();
    char file[100];
    move(maxY-5, 12);
    getnstr(file, 99);
    if (!m_table.Save(file)) {
        m_err = "Error while saving file, please press 'g' once again.";
        mvprintw(maxY-5, 12, " %s", m_err.c_str());
        noecho();
        curs_set(0);
        GetTable();
    } else {
        noecho();
        curs_set(0);
        MakeView();
    }
}

void CTableControl::LoadFile() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    echo();
    curs_set(1);
    move(maxY-5, 12);
    clrtoeol();
    char file[100];
    move(maxY-5, 12);
    getnstr(file, 100);
    if (!m_table.Load(file)){
        m_err = "Input file not found, please press 'l' once again and select another file.";
        attron(A_BOLD);
        mvprintw(maxY-5, 12, " %s", m_err.c_str());
        attroff(A_BOLD);
        noecho();
        curs_set(0);
        GetTable();
    } else {
        noecho();
        curs_set(0);
        MakeView();
    }
}