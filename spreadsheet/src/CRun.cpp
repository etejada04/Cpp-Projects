/**
 * @file CRun.cpp
 * @author tejadede
 * @brief Run file
 */
#include "CRun.h"
CRun::CRun(): m_view(-10){
    m_control = NULL;
    initscr();
    keypad(stdscr, TRUE);
    if (has_colors()) {
        start_color();
    }
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    m_controllers.insert(std::make_pair((int) START, new CStartControl()));
    m_controllers.insert(std::make_pair((int) HELP, new CTableControl()));
    m_controllers.insert(std::make_pair((int) TABLE, new CHelpControl()));
    try {
        ChangeWindow(START);
    } catch (const std::invalid_argument & ia) {
        throw ia;
    }
}

CRun::~CRun() {
    for (auto it = m_controllers.begin(); it != m_controllers.end(); ++it){
        delete it->second;
    }
    endwin();
}

void CRun::Start() {
    int ch;
    noecho();
    curs_set(0);
    while(true){
        ch = getch();

        switch (ch) {
            case 'q':
                return;
            case 's':
                ChangeWindow(TABLE);
            case 'h':
                if(m_view == TABLE){
                    try {
                        ChangeWindow(HELP);
                    } catch (const std::invalid_argument & ia) {
                        throw ia;
                    }
                } else {
                    try {
                        ChangeWindow(TABLE);
                    } catch (const std::invalid_argument & ia) {
                        throw ia;
                    }
                }
            default:
                m_control->KeyHandler(ch);
                break;
        }
    }
}

void CRun::ChangeWindow(int newView) {
    auto search = m_controllers.find(newView);
    if (search == m_controllers.end()){
        throw std::invalid_argument("Window not found");
    }
    m_view = newView;
    m_control = m_controllers[newView];
    m_control->MakeView();
}