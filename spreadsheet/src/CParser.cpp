/**
 * @file CParser.cpp
 * @author tejadede
 * @brief Parser file
 */
#include "CParser.h"
void CParser::Replace(std::string & str, const std::string & from, const std::string & to) {
    size_t index = 0;
    while ((index = str.find(from, index)) != std::string::npos){
        str.replace(index, from.length(), to);
        index+=to.length();
    }
}

void CParser::Token(std::list <std::string> & tokens, const std::string & delim) {
    typedef std::list <std::string>::iterator myIter;
    std::string firstTk = "", prevTk = "", nextTk = "";
    size_t nextIndex = 0, initIndex = 0;
    size_t size = sizeof (mychars) / sizeof (std::string);
    for (int i = 0; i < static_cast<int> (size); ++i) {
        std::string str = mychars[i];
        Replace(m_inputExp, str, " " + str + " ");
    }
    initIndex = m_inputExp.find_first_not_of(delim, nextIndex);
    while (initIndex != std::string::npos && nextIndex != std::string::npos){
        nextIndex = m_inputExp.find(delim, initIndex);
        std::string tk = m_inputExp.substr(initIndex, nextIndex-initIndex);
        tokens.push_back(tk);
        initIndex = m_inputExp.find_first_not_of(delim, nextIndex);
    }
    firstTk = tokens.front();
    if (firstTk == "-"){
        std::list <std::string>::iterator it = tokens.begin();
        it++;

        if (it == tokens.end()) {
            return;
        }

        nextTk = *(it);

        if (NumberFlag(nextTk) || FloatFlag(nextTk)) {
            tokens.pop_front();
            tokens.front() = firstTk + nextTk;
        } else if (nextTk == "(" || FunctionFlag(nextTk)) {
            tokens.front() = firstTk + "1";
            tokens.insert(it, "*");
        } else if (nextTk == "-" && firstTk == "-") {
            std::list <std::string>::iterator newIt = it;
            std::advance(newIt, -1);
            tokens.erase(it);
            tokens.erase(newIt);
        }
    }
    for (myIter it = tokens.begin(); it != std::prev(tokens.end()); ++it) {
        std::string tk = *it;
        std::list <std::string>::iterator newIt = it;
        std::advance(newIt, 1);

        if (newIt == tokens.end()) {
            break;
        }
        std::string newTk = *newIt;

        if (tk == "-" && prevTk == "(") {
            if(NumberFlag(newTk) || FloatFlag(newTk)) {
                tokens.erase(newIt);
                *it = "-" + newTk;
                tk = *it;
            }
        } else if (tk == "-" && (OperatorFlag(prevTk) || prevTk == "%")) {
            if (tk == "-" && prevTk == "-") {
                std::list <std::string>::iterator it1 = it;
                std::list <std::string>::iterator it2 = it1;
                it2++;
                std::advance(it1, -1);
                tokens.erase(it);
                *it1 = "+";
                std::list <std::string>::iterator it3 = it1;
                std::advance(it3, -1);

                if (OperatorFlag(*it3) || *it3 == "(") {
                    tokens.erase(it1);
                }

                tk = *it2;
                it = it2;
                if (it == std::prev(tokens.end())) {
                    break;
                }
            } else if (NumberFlag(newTk) || FloatFlag(newTk) || FunctionFlag(newTk)) {
                bool exit = false;
                if (newIt == std::prev(tokens.end())) {
                    exit = true;
                }

                tokens.erase(newIt);
                *it = "-" + newTk;
                tk = *it;

                if (exit) {
                    break;
                }
            } else if (newTk == "(") {
                *it = "-1";
                tk = *it;
                tokens.insert(newIt, "*");
            }
        }
        prevTk = tk;
    }
    prevTk = "";
    myIter prevIt;
    for (myIter it = tokens.begin(); it != tokens.end(); ++it) {
        std::string tk = *it;
        if(tk == "(" && prevTk == "-"){
            tokens.insert(it, "1");
            tokens.insert(it, "*");
        }
        prevTk = tk;
        prevIt = it;
    }
}

CParser::CParser(const std::string &str): m_inputExp(str) {}

bool CParser::Evaluate(const std::vector <std::string> &  rpn, std::string & res) {
    std::stack <std::string> stack;
    typedef std::vector <std::string>::const_iterator rpn_iter;
    for(rpn_iter it = rpn.begin(); it != rpn.end(); ++it) {
        std::string tk = *it;
        if(NumberFlag(tk) || EFlag(tk) || PiFlag(tk) || FloatFlag(tk)){
            if(PiFlag(tk)) {
                std::stringstream ss;
                ss << PI;
                tk = ss.str();
            } else if(EFlag(tk)) {
                std::stringstream ss;
                ss << E;
                tk = ss.str();
            }
            stack.push(tk);
        } else if (OperatorFlag(tk) || FunctionFlag(tk)){
            double result = 0.0;
            unsigned int argNum = NumberOfArgs(tk);
            unsigned long argStack = stack.size();
            bool unaryFlag = false;
            std::vector <double> args;
            std::string value;

            if (argStack < argNum){
                if (argStack == 1 && argNum == 2 && (tk == "+" || tk == "-")) {
                    value = stack.top();
                    result = strtod(value.c_str(), NULL);
                    stack.pop();
                    unaryFlag = true;
                } else {
                    return  false;
                }
            } else {
                while (argNum > 0) {
                    value = stack.top();
                    double n = strtod(value.c_str(), NULL);
                    args.push_back(n);
                    stack.pop();
                    argNum--;
                }
            }

            if (OperatorFlag(tk) && !unaryFlag) {
                double d1 = args[0], d2 = args[1];

                if (tk == "+"){
                    result = d2 + d1;
                } else if (tk == "-") {
                    result = d2 - d1;
                } else if (tk == "*") {
                    result = d2 * d1;
                } else if (tk =="/") {
                    result = d2 / d1;
                } else if (tk == "%"){
                    int i1 = (int) args[0], i2 = (int) args[1];
                    result = Mod(i2, i1);
                }
            } else if (FunctionFlag(tk)) {
                double d0 = args [0];
                //In case sin is negative then multiply by -1
                double m = tk.find("-") != std::string::npos ? -1 : 1;
                if (tk.find("sin") != std::string::npos){
                    result = sin(d0);
                } else if (tk.find("cos") != std::string::npos){
                    result = cos(d0);
                } else if (tk.find("tan") != std::string::npos){
                    result = tan(d0);
                } else if (tk.find("log") != std::string::npos){
                    result = log10(d0);
                } else if (tk.find("ln") != std::string::npos){
                    result = log(d0);
                } else if (tk.find("pow") != std::string::npos){
                    double p = args[0], b =  args[1];
                    result = pow(b, p);
                } else if (tk.find("sqrt") != std::string::npos){
                    result = sqrt(d0);
                } else if (tk.find("abs") != std::string::npos){
                    result = abs(d0);
                } else if (tk.find("exp") != std::string::npos){
                    result = exp(d0);
                }
                result *= m;
            }
            if (result == (long) result) {
                result = (long) result;
            }
            std::stringstream ss;
            ss << result;
            stack.push(ss.str());
        }
    }

    if (stack.size() == 1) {
        //Final result
        double fres;
        res = stack.top();
        fres = strtod(res.c_str(), NULL);
        if( fres == (long) fres) {
            long lres = (long) fres;
            std::stringstream ss;
            ss << lres;
            res = ss.str();
        }
        return true;
    }
    return false;
}

bool CParser::InfixToRPN(std::vector <std::string> & vec) {
    typedef std::list <std::string>::const_iterator tokenIt;
    std::string stackTk;
    std::stack <std::string> stack;
    std::queue <std::string> output;
    bool done = true;
    std::list <std::string> infix;
    Token(infix, " ");

    for (tokenIt it = infix.begin(); it != infix.end(); ++it) {
        std::string tk = *it;
        if (NumberFlag(tk) || FloatFlag(tk) || PiFlag(tk) || EFlag(tk)) {
            output.push(tk);
        } else if (FunctionFlag(tk)) {
            stack.push(tk);
        } else if (SeparatorFlag(tk)) {
            stackTk = stack.top();
            while (stackTk != "(") {
                output.push(stackTk);
                stack.pop();
                stackTk = stack.top();
            }
            if (stackTk == "(") {
                done = true;
            } else {
                done = false;
            }
        } else if (OperatorFlag(tk)) {
            while (!stack.empty() && OperatorFlag(stack.top())
                    && ((LeftAssociative(tk) && Precedence(tk) == Precedence(stack.top()))
                        || (Precedence(tk) < Precedence(stack.top())))) {
                stackTk = stack.top();
                stack.pop();
                output.push(stackTk);
            }
            stack.push(tk);
        } else if (tk == "(") {
            stack.push(tk);
        } else if (tk == ")") {
            while  (!stack.empty() && stack.top() != "("){
                output.push(stack.top());
                stack.pop();
            }

            if (!stack.empty()) {
                stackTk = stack.top();
                if(stackTk != "(") {
                    done = false;
                }
            } else {
                return false;
            }
            stack.pop();
            if(!stack.empty()) {
                stackTk = stack.top();
                if(FunctionFlag(stackTk)) {
                    output.push(stackTk);
                    stack.pop();
                }
            }
        }
    }
    while (!stack.empty()) {
        output.push(stack.top());
        stack.pop();
    }

    while (!output.empty()){
        std::string tk = "" ;
        tk = output.front();
        vec.push_back(tk);
        output.pop();
    }
    return done;

}

bool CParser::ParenthesesFlag() {
    std::vector <std::string> input;
    long leftside = std::count(m_inputExp.begin(), m_inputExp.end(), '(');
    long rightside = std::count(m_inputExp.begin(), m_inputExp.end(), ')');
    return leftside == rightside && !m_inputExp.empty();
}

bool OperatorFlag (const std::string & str){
    return (str == "+" || str == "-" || str == "*" || str == "/" || str == "%");
}

bool FunctionFlag (const std::string & str){
    return (str.find("sin") != std::string::npos || str.find("cos") != std::string::npos
            || str.find("tan") != std::string::npos || str.find("log") != std::string::npos
            || str.find("ln") != std::string::npos || str.find("pow") != std::string::npos
            || str.find("exp") != std::string::npos || str.find("abs") != std::string::npos
            || str.find("sqrt") != std::string::npos );
}

bool SeparatorFlag (const std::string & str){
    return str == ";";
}

int Precedence (const std::string & str){
    int pre = 1;
    if (str == "+" || str == "-" ) {
        pre = 1;
    } else if (str == "*" || str == "/" || str == "%") {
        pre = 2;
    }
    return pre;
}

bool PiFlag (const std::string & str){
    return str == "pi";
}

bool EFlag (const std::string & str){
    return str == "e";
}

bool FloatFlag (const std::string & str){
    float number;
    std::istringstream iss (str);
    iss >> std::noskipws >> number;
    return iss.eof() && !iss.fail();

}

bool NumberFlag (const std::string & str){
    std::string::const_iterator it = str.begin();
    while(it != str.end() && std::isdigit(*it, std::locale())){
        ++it;
    }
    return !str.empty() && it == str.end();
}

unsigned int NumberOfArgs (const std::string & str){
    unsigned int argCnt = 1;
    if(str == "+" || str == "-" || str == "*" || str == "/"
       || str == "%" || str == "pow"){
        argCnt = 2;
    } 
    return argCnt;
}

bool LeftAssociative (const std::string & str){
    return (str == "+" || str == "-" || str == "*" || str == "/" || str == "%");
}

int Mod (int n, int m){
    int res = n % m;
    return (n >= 0 || res == 0) ? res : m + n;
}
