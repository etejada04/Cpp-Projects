/**
 * @file CCell.h
 * @author tejadede
 * @brief Cell file header
 */
#ifndef __CCELL_H__
#define __CCELL_H__
#include <set>
#include <string>
#include <stdexcept>
#include <sstream>
#include <exception>
/**
 * @class CCell
 * @brief This class denotes each cell contained in our table. Each cell is identified by
 * an ID and contains some expression which will be used for the parsing.
 */
class CCell {
protected:
    std::string m_value; /**< Value contained in the cell*/
    unsigned int m_cellId; /**< Cell identifier*/
public:
    /**
     * @brief CCell constructor.
     */
    CCell ();
    /**
     * @brief CCell copy constructor.
     * @param cell Denotes the original cell
     */
    CCell(const CCell & cell);
    /**
     * @brief CCell destructor
     */
    virtual ~CCell();
    /**
     * @brief This method is used in the case when the modification of the value
     * of another cell has an effect in the current cell.
     */
    virtual void Refresh ();
    /**
     * @brief This method return the value of the cell as a double.
     * @return Double value of the cell
     */
    virtual double GetValue () const;
    /**
     * @brief This method return the mathematical expression of current cell.
     * @return Expression as a string
     */
    virtual std::string GetExpression () const;
    /**
     * @brief This method returns the value of the current cell as a string.
     * @return String value of the cell
     */
    virtual std::string GetValueStr () const;
    /**
     * @brief This method updates the value of the cell and in case the expression
     * we are evaluating depends of other cells (e.g. avg() ) then a set of related
     * cells is created.
     * @param expression Denotes the mathematical expression
     * @return Set of related cells
     */
    virtual std::set <std::pair <int, int> > Update (const std::string & expression);
    /**
     * @brief This method returns the id of the cell.
     * @return Id of cell
     */
    unsigned int GetId () const;
    /**
     * @brief This method modifies the id of the cell.
     * @param id Denotes the new id
     */
    void SetId (unsigned int id);
};
#endif //__CCELL_H__