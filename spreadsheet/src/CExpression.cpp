/**
 * @file CExpression.cpp
 * @author tejadede
 * @brief Expression file
 */
#include "CExpression.h"
CExpression::CExpression(const CTable * table) {
    m_table = table;
    m_err = false;
}

CExpression::CExpression(const CExpression & cexp): CCell(cexp) {
    m_value = cexp.m_value;
    m_err = false;
}

CExpression::~CExpression() {}

double CExpression::ConvertStrToDouble(const std::string & str) {
    double res = 0.0;
    std::istringstream iss (str);
    return !(iss >> res) ? 0 : res;
}

void CExpression::Refresh() {
    m_err = false;
    Calculate();
}

std::set <std::pair <int, int> > CExpression::Update(const std::string & expression) {
    m_err = false;
    m_relatedTo.clear();
    m_value = expression;
    Calculate();
    return m_relatedTo;
}

double CExpression::GetValue() const {
    return (m_err) ? throw std::logic_error(m_message) : m_result;
}

std::string CExpression::GetValueStr() const {
    if (m_err) {
        return m_message;
    } else {
        std::ostringstream oss;
        oss << std::setprecision(3) << m_result;
        return oss.str();
    }
}

std::string CExpression::ReplaceCell(std::string expression) {
    std::string keyword, subExpr;
    //Remove spaces
    expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());
    m_value = expression;
    // Replace '=' from the beginning with space
    expression.replace(expression.find("="), 1, " ");
    expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

    keyword = "cell";
    size_t index = expression.find(keyword, 0);
    while (index != std::string::npos) {
        char ch;
        int row = 0, col = 0;
        double value;
        CCell * cell = nullptr;
        std::stringstream ss;
        size_t closeTag = expression.find(")", index);
        subExpr = expression.substr(index+4, closeTag-index+1);
        ss << subExpr;
        ss >> ch;
        if (ch != '('){
            m_err = true;
            m_message = "Invalid function cell format.";
            return expression;
        }
        ss >> row;
        ss >> ch;
        if (ch != ';'){
            m_err = true;
            m_message = "Invalid function cell format.";
            return expression;
        }
        ss >> col;
        ss >> ch;
        if (ch != ')'){
            m_err = true;
            m_message = "Invalid function cell format.";
            return expression;
        }
        if (col == 0 || row == 0){
            m_err = true;
            m_message = "Invalid function cell format.";
            return expression;
        }
        m_relatedTo.insert(std::make_pair(row, col));
        try {
            cell = m_table->GetCell(row, col);
            if (cell->GetId() == m_cellId){
                m_err = true;
                m_message = "ERROR: Cycle detected.";
                return expression;
            } else {
                value = cell->GetValue();
            }
        } catch (std::logic_error ex) {
            m_err = true;
            m_message = "ERROR: Input cell contains a string or is empty.";
            return expression;
        }
        expression.replace(index, closeTag-index+1, std::to_string(value));
        index = expression.find(keyword, index+1);
    }

    keyword = "sum";
    index = expression.find(keyword, 0);
    while (index != std::string::npos) {
        char ch;
        int rowStart = 0, colStart = 0, rowEnd = 0, colEnd = 0;
        double value = 0.0;
        CCell * cell = nullptr;
        std::stringstream ss;
        size_t closeTag = expression.find(")", index);
        subExpr = expression.substr(index+3, closeTag-index+1);
        ss << subExpr;
        ss >> ch;
        if (ch != '('){
            m_err = true;
            m_message = "Invalid function sum format.";
            return expression;
        }
        ss >> rowStart;
        ss >> ch;
        if (ch != ';'){
            m_err = true;
            m_message = "Invalid function sum format.";
            return expression;
        }
        ss >> colStart;
        ss >> ch;
        if (ch != ':'){
            m_err = true;
            m_message = "Invalid function sum format.";
            return expression;
        }
        ss >> rowEnd;
        ss >> ch;
        if (ch != ';'){
            m_err = true;
            m_message = "Invalid function sum format.";
            return expression;
        }
        ss >> colEnd;
        ss >> ch;
        if (ch != ')'){
            m_err = true;
            m_message = "Invalid function sum format.";
            return expression;
        }

        if (colStart == 0 || rowStart == 0 || rowEnd == 0 || colEnd == 0){
            m_err = true;
            m_message = "Invalid function sum format.";
            return expression;
        }

        if (rowStart > rowEnd){
            int tmp = rowEnd;
            rowEnd = rowStart;
            rowStart = tmp;
        }

        if (colStart > colEnd) {
            int tmp = colEnd;
            colEnd = colStart;
            colStart = tmp;
        }

        for (int i = rowStart; i <= rowEnd; ++i) {
            for (int j = colStart; j <= colEnd; ++j) {
                m_relatedTo.insert(std::make_pair(i, j));
                try {
                    cell = m_table->GetCell(i, j);
                    if (cell->GetId() == m_cellId){
                        m_err = true;
                        m_message = "ERROR: Cycle detected.";
                    } else {
                        value += cell->GetValue();
                    }
                } catch (std::logic_error ex) {
                    m_err = true;
                    m_message = "ERROR: Input cell contains a string or is empty.";
                }
            }
        }
        if (m_err) {
            return expression;
        }
        expression.replace(index, closeTag-index+1, std::to_string(value));
        index = expression.find(keyword, index+1);
    }
    keyword = "avg";
    index = expression.find(keyword, 0);
    while (index != std::string::npos) {
        char ch;
        int rowStart = 0, colStart = 0, rowEnd = 0, colEnd = 0, rcounter = 0, ccounter = 0;
        double value = 0.0;
        CCell * cell = nullptr;
        std::stringstream ss;
        size_t closeTag = expression.find(")", index);
        subExpr = expression.substr(index+3, closeTag-index+1);
        ss << subExpr;
        ss >> ch;
        if (ch != '('){
            m_err = true;
            m_message = "Invalid function avg format.";
            return expression;
        }
        ss >> rowStart;
        ss >> ch;
        if (ch != ';'){
            m_err = true;
            m_message = "Invalid function avg format.";
            return expression;
        }
        ss >> colStart;
        ss >> ch;
        if (ch != ':'){
            m_err = true;
            m_message = "Invalid function avg format.";
            return expression;
        }
        ss >> rowEnd;
        ss >> ch;
        if (ch != ';'){
            m_err = true;
            m_message = "Invalid function avg format.";
            return expression;
        }
        ss >> colEnd;
        ss >> ch;
        if (ch != ')'){
            m_err = true;
            m_message = "Invalid function avg format.";
            return expression;
        }

        if (colStart == 0 || rowStart == 0 || rowEnd == 0 || colEnd == 0){
            m_err = true;
            m_message = "Invalid function avg format.";
            return expression;
        }

        if (rowStart > rowEnd){
            int tmp = rowEnd;
            rowEnd = rowStart;
            rowStart = tmp;
        }

        if (colStart > colEnd) {
            int tmp = colEnd;
            colEnd = colStart;
            colStart = tmp;
        }

        for (int i = rowStart; i <= rowEnd; ++i) {
            for (int j = colStart; j <= colEnd; ++j) {
                m_relatedTo.insert(std::make_pair(i, j));
                try {
                    cell = m_table->GetCell(i, j);
                    if (cell->GetId() == m_cellId){
                        m_err = true;
                        m_message = "ERROR: Cycle detected.";
                    } else {
                        value += cell->GetValue();
                    }
                } catch (std::logic_error ex) {
                    m_err = true;
                    m_message = "ERROR: Input cell contains a string or is empty.";
                }
            }
        }
        if (m_err) {
            return expression;
        }
        rcounter = rowEnd - rowStart + 1;
        ccounter = colEnd - colStart + 1;
        expression.replace(index, closeTag-index+1, std::to_string(value/(rcounter*ccounter)));
        index = expression.find(keyword, index+1);
    }
    return expression;
}

void CExpression::Calculate() {
    std::string exp = ReplaceCell(m_value);
    if (!m_err) {
        CParser parser (exp);
        if (!parser.ParenthesesFlag()){
            m_err = true;
            m_message = "ERROR: No matching parentheses.";
        } else {
            std::vector <std::string> rpn;
            if(parser.InfixToRPN(rpn)){
                std::string res;
                if (parser.Evaluate(rpn, res)){
                    m_result = ConvertStrToDouble(res);
                }
            } else {
                m_err = true;
                m_message = "ERROR: No matching parentheses.";
            }
        }
    }
}