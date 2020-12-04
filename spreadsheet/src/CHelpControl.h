/**
 * @file CHelpControl.h
 * @author tejadede
 * @brief Help controller file header
 */
#ifndef __CHELPCONTROL_H__
#define __CHELPCONTROL_H__
#include <ncurses.h>
#include "CControllers.h"
/**
 * @class CHelpControl
 * @brief This class is used to display the help menu and guide the user through the program
 * interface and functions/operator available.
 */
class CHelpControl: public CControllers {
public:
    /**
     * @brief This method display the HELP view.
     */
    virtual void MakeView ();
};
#endif //__CHELPCONTROL_H__