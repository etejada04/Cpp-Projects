/**
 * @file CControllers.h
 * @author tejadede
 * @brief Controllers file header
 */
#ifndef __CCONTROLLERS_H__
#define __CCONTROLLERS_H__
#include <ncurses.h>
/**
 * @class CControllers
 * @brief Class This class handles the controllers for each view of the application,
 * each class inherited from CControllers provide its own keys and view.
 */
class CControllers {
public:
    /**
     * @brief CControllers destructor.
     */
    virtual ~CControllers ();
    /**
     * @brief This method handles the keys required by each view.
     * @param key Denotes the key value
     */
    virtual void KeyHandler (int key);
    /**
     * @brief This method display the view that is required by the class.
     */
    virtual void MakeView ();

};
#endif //__CCONTROLLERS_H__
