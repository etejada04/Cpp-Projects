/**
 * @file CParser.h
 * @author tejadede
 * @brief Parser file header
 */
#ifndef __CPARSER_H__
#define __CPARSER_H__
#include <iomanip>
#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include "CCell.h"
#include "CTable.h"
#define PI 3.1415927
#define E 2.71828182846
const std::string mychars[] = { "+", "-", "*" , "/", "%", "(", ")", ";"};/**< Set of characters*/
/**
 * @class CParser
 * @brief This class is handling the parsing and evaluation of expressions, also is able to
 * detect if a function is  being used and compute it accordingly.
 */
class CParser {
private:
    std::string m_inputExp; /**< Value of the input ME*/
    /**
     * @brief This method replace all instances of input string.
     * @param str Denotes the input string
     * @param from Denotes from where we start to replace
     * @param to Denotes until where we finish the replace
     */
    void Replace (std::string & str, const std::string & from, const std::string & to);
    /**
     * @brief This method split the expression into tokens which are later used
     * for the Reverse Polish Notation to evaluate them.
     * @param tokens Denotes the list of tokens
     * @param delim Denotes the breaking point for the parsing
     */
    void Token (std::list <std::string> & tokens, const std::string & delim);
public:
    /**
     * @brief CParser constructor.
     * @param str Denotes the string to parse
     */
    CParser (const std::string & str);
    /**
     * @brief This method evaluate the expression that is passed by the RPN.
     * @param rpn Denotes the strings converted into RPN
     * @param res Denotes the result string
     * @return True if no errors occurred during the evaluation, false
     * otherwise
     */
    bool Evaluate (const std::vector <std::string> & rpn, std::string & res);
    /**
     * @brief This method converts infix notation into RPN.
     * @param vec Denotes the tokens to be processed
     * @return True if no errors occurred during the conversion, false
     * otherwise
     */
    bool InfixToRPN (std::vector <std::string> & vec);
    /**
     * @brief This method check if there exist matching parentheses.
     * @return True if there is a match, false otherwise
     */
    bool ParenthesesFlag ();
};
/**
 * @brief This function check if argument is an operator.
 * @param str Denotes the operator
 * @return True if argument is a valid operator, false otherwise
 */
bool OperatorFlag (const std::string & str);
/**
 * @brief This function check if argument is a function (i.e. sin, cos, etc.).
 * @param str Denotes the operator
 * @return True if argument is a valid operator, false otherwise
 */
bool FunctionFlag (const std::string & str);
/**
 * @brief This function check if argument is a separator (i.e. ';').
 * @param str Denotes the function
 * @return True if argument is a valid operator, false otherwise
 */
bool SeparatorFlag (const std::string & str);
/**
 * @brief This function check the precedence of the operator.
 * @param str Denotes the operator
 * @return Precedence related to the operator
 */
int Precedence (const std::string & str);
/**
 * @brief This function check if argument is pi (i.e. 3.1415927).
 * @param str Denotes the argument
 * @return True if argument is pi, false otherwise
 */
bool PiFlag (const std::string & str);
/**
 * @brief This function check if argument is e (i.e. 2.71828182846).
 * @param str Denotes the argument
 * @return True if argument is e, false otherwise
 */
bool EFlag (const std::string & str);
/**
* @brief This function check if argument is a float.
* @param str Denotes the argument
* @return True if argument is a float, false otherwise
*/
bool FloatFlag (const std::string & str);
/**
* @brief This function check if argument is a number.
* @param str Denotes the argument
* @return True if argument is a number, false otherwise
*/
bool NumberFlag (const std::string & str);
/**
 * @brief This function check the number of arguments in the function.
 * @param str Denotes the input expression
 * @return Number of arguments in the expression
 */
unsigned int NumberOfArgs (const std::string & str);
/**
 * @brief This function checks if operator is left or right associative.
 * @param str Denotes the operator
 * @return True is operator is left associative, false if it is right
 * associative
 */
bool LeftAssociative (const std::string & str);
/**
 * @brief This function does the modulus operation.
 * @param n Denotes the number to which we apply the mod
 * @param m Denotes the number of the mod
 */
int Mod (int n, int m);
#endif //__CPARSER_H__
