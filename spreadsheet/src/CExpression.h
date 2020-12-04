/**
 * @file CExpression.h
 * @author tejadede
 * @brief Expression file header
 */
#ifndef __CEXPRESSION_H__
#define __CEXPRESSION_H__
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <map>
#include <algorithm>
#include "CCell.h"
#include "CParser.h"
#include "CTable.h"
class CTable;
/**
 * @class CExpression
 * @brief This class denotes the expression contained in each cell.
 */
class CExpression : public CCell {
private:
    double m_result; /**< Value of the result*/
    std::string m_message; /**< Error message*/
    bool m_err; /**< Error flag*/
    const CTable * m_table; /**< Table with cells*/
    std::set <std::pair <int, int> > m_relatedTo; /**< Set of related cells*/
    /**
     * @brief This method converts a string into a double.
     * @param str Denotes the input string to be converted
     * @return Double representation of the input string
     */
    double ConvertStrToDouble (const std::string & str);
    /**
     * @brief This method parse the expression and look for functions
     * cell, average or sum and saves the result into the expression.
     * Also it looks if any cycles occurs during the computation.
     * @return A string with the result
     */
    std::string ReplaceCell (std::string expression);
    /**
     * @brief This method evaluates the list of tokens and store the
     * result of the evaluation.
     */
    void Calculate ();
public:
    /**
     * @brief CExpression constructor.
     * @param table Denotes the table of cells
     */
    CExpression (const CTable * table);
    /**
     * @brief Copy constructor.
     * @param cexp Denotes the original expression
     */
    CExpression(const CExpression & cexp);
    /**
     * @brief CExpression destructor.
     */
    virtual ~CExpression();
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
     * @brief This method returns the value of the current cell as a string.
     * @return String value of the cell
     */
    virtual std::string GetValueStr () const;
    /**
     * @brief This method updates the value of the cell and in case the expression
     * we are evaluating depends of other cells (e.g. avg() ) then a set of related
     * cells is created.
     * @param expression Expression to parse
     * @return Set of related cells
     */
    virtual std::set <std::pair <int, int> > Update (const std::string & expression);
};
#endif //__CEXPRESSION_H__
