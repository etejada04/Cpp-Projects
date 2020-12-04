/**
 * @file CString.h
 * @author tejadede
 * @brief String file header
 */
#ifndef __CSTRING_H__
#define __CSTRING_H__
#include <string>
#include "CCell.h"
/**
 * @class CString
 * @brief This class represents cells that must be treated as string since
 * their expression is missing the '=' sign at the beginning.
 */
class CString : public CCell {
public:
    /**
     * @brief CString constructor.
     */
    CString ();
    /**
     * @brief CString copy constructor.
     * @param cstr Denotes the original CString object
     */
    CString (const CString & cstr);
    /**
     * @brief CString destructor.
     */
    virtual ~CString ();
};
#endif //__CSTRING_H__
