/**
 * @file CStartControl.h
 * @author tejadede
 * @brief Start controller file header
 */
#ifndef __CSTARTCONTROL_H__
#define __CSTARTCONTROL_H__
#include <ncurses.h>
#include "CControllers.h"
/**
 * @class CStartControl
 * @brief This class is used at the beginning of the program to display a welcome message
 * and let the user start the program or quit it.
 */
class CStartControl: public CControllers {
public:
    /**
     * @brief This method displays the START view.
     */
    virtual void MakeView ();
};
#endif //__CSTARTCONTROL_H__