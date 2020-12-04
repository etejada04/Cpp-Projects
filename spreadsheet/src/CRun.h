/**
 * @file CRun.h
 * @author tejadede
 * @brief Run file header
 */
#ifndef __CRUN_H__
#define __CRUN_H__
#include <map>
#include <ncurses.h>
#include "CControllers.h"
#include "CStartControl.h"
#include "CHelpControl.h"
#include "CTableControl.h"
#define START 0
#define HELP 1
#define TABLE 2
/**
 * @class CRun
 * @brief This class starts the program and is in charge of handling what control to use and
 * which key is pressed by the user
 */
class CRun {
private:
    int m_view; /**< Current view on the window*/
    std::map <int, CControllers*> m_controllers; /**< Map controllers with their type*/
    CControllers * m_control; /**< Type of control*/
public:
    /**
     * @brief CRun constructor.
     */
    CRun ();
    /**
     * @brief CRun copy constructor.
     * @param src Original CRun object
     */
    CRun (const CRun & src) = delete;
    /**
     * @brief CRun destructor.
     */
    virtual ~CRun ();
    /**
     * @brief This method starts the program, here all the keys are handled.
     */
    void Start ();
    /**
     * @brief This method changes the current view.
     * @param newView Denotes the new view (i.e. START, HELP or TABLE)
     */
    void ChangeWindow (int newView);
};
#endif //__CRUN_H__