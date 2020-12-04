/**
 * @file CTable.h
 * @author tejadede
 * @brief Table file header
 */
#ifndef __CTABLE_H__
#define __CTABLE_H__
#include <string>
#include <map>
#include <iostream>
#include <stdexcept>
#include "CCell.h"
#include "CExpression.h"
#include "CString.h"
/**
 * @class CTable
 * @brief This class represents the whole table with CCell's.
 */
class CTable {
private:
    std::map < std::pair <int, int>
            , std::set < std::pair <int, int> > > m_relations; /**< Map of related cells*/
    std::map < std::pair <int, int>, CCell*> m_table; /**< Map of table with CCell's*/
    unsigned int m_id; /**< ID of the cell*/
public:
    /**
     * @brief CTable constructor.
     */
    CTable ();
    /**
     * @brief CTable copy constructor.
     * @param table Denotes the original table
     */
    CTable (const CTable & table);
    /**
     * @brief CTable destructor.
     */
    virtual ~CTable ();
    /**
     * @brief This method access a cell in an specific position.
     * @param x Denotes the row number
     * @param y Denotes the column number
     * @return Pointer to a CCell
     */
    virtual CCell * GetCell (int row, int col) const;
    /**
     * @brief This method insert a string into the cell into at an specified
     * row and column.
     * @param x Denotes the row number
     * @param y Denotes the column number
     * @param expression Denotes the value to be inserted into the cell
     */
    virtual void Insert (int row, int col, const std::string & value);
    /**
     * @brief This method is used in the case when the modification of the value
     * of another cell has an effect in the current cell.
     * @param row Denotes the row number
     * @param col Denotes the column number
     */
    virtual void Refresh (int row, int col);
    /**
     * @brief This method returns the mathematical expression of an specific cell to parse it.
     * @param x Denotes the row number
     * @param y Denotes the column number
     * @return Expression of the cell
     */
    virtual std::string GetExpression (int row, int col) const;
    /**
     * @brief This method returns the value of an specific cell.
     * @param x Denotes the row number
     * @param y Denotes the column number
     * @return Value of the cell
     */
    virtual std::string GetValue (int row, int col) const;
    /**
     * @brief This method writes a file extract parameters row, column and expression and
     * saves them into a file.
     * @param fname Denotes the name of the output file
     * @return True if no errors occurred while saving the file, false otherwise
     */
    virtual bool Save (const std::string & fname);
    /**
     * @brief This method load the parameters from the input file and creates a new table
     * with them.
     * @param fname Denotes the source input file
     * @return True if file was found, false otherwise
     */
    virtual bool Load (const std::string & fname);
};
#endif //__CTABLE_H__