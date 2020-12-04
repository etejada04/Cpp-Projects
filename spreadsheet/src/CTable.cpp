/**
 * @file CTable.cpp
 * @author tejadede
 * @brief Table file
 */
#include <fstream>
#include "CTable.h"
CTable::CTable() : m_id(0) {}

CTable::CTable(const CTable & table) {
    m_table = table.m_table;
}

CTable::~CTable() {
    for (auto it = m_table.begin(); it != m_table.end(); ++it){
        delete it->second;
    }
}

CCell * CTable::GetCell(int row, int col) const {
    auto search = m_table.find(std::make_pair(row,col));
    if (search != m_table.end()){
        return search->second;
    } else {
        throw std::logic_error("ERROR: Cell not found.");
    }
}

void CTable::Refresh(int row, int col) {
    auto search = m_table.find(std::make_pair(row,col));
    if(search != m_table.end()){
        search->second->Refresh();
    }
}

void CTable::Insert(int row, int col, const std::string & value) {
    std::set < std::pair <int, int> > relatedTo;
    auto search = m_table.find(std::make_pair(row,col));
    if (search == m_table.end()) {
        CCell * cell;
        if (value.length() > 0 && value.at(0) == '='){
            cell = new CExpression (this);
        } else {
            cell = new CString();
        }
        cell->SetId(m_id++);
        m_table.insert(std::make_pair(std::make_pair(row, col), cell));
        relatedTo = cell->Update(value);
    } else {
        delete search->second;
        CCell * cell;
        if (value.length() > 0 && value.at(0) == '='){
            cell = new CExpression (this);
        } else {
            cell =  new CString();
        }
        cell->SetId(m_id++);
        m_table[std::make_pair(row, col)] = cell;
        relatedTo = cell->Update(value);
    }

    //Create relationship
    if (relatedTo.size() > 0){
        for (auto it = relatedTo.begin(); it != relatedTo.end(); ++it) {
            auto relation = m_relations.find(*it);
            if (relation == m_relations.end()) {
                //Create a new one
                std::set <std::pair <int, int> > tmpRel;
                tmpRel.insert(std::make_pair(row,col));
                m_relations.insert(std::make_pair(std::make_pair(it->first, it->second), tmpRel));
            } else {
                relation->second.insert(std::make_pair(row, col));
            }
        }
    }
    auto recompute = m_relations.find(std::make_pair(row,col));
    if (recompute != m_relations.end()) {
        for(auto it = recompute->second.begin(); it != recompute->second.end(); ++it){
            Refresh((*it).first, (*it).second);
        }
    }
}

std::string CTable::GetExpression(int row, int col) const {
    std::string res;
    auto search = m_table.find(std::make_pair(row,col));
    if(search != m_table.end()){
        res = search->second->GetExpression();
    }
    return res;
}

std::string CTable::GetValue(int row, int col) const {
    std::string res;
    auto search = m_table.find(std::make_pair(row,col));
    if(search!=m_table.end()){
        res = search->second->GetValueStr();
    }
    return res;
}

bool CTable::Save(const std::string & fname) {
    std::string homepath = getenv("HOME");
    std::ofstream out((homepath + "/tejadede/examples/" + fname).c_str());
    if (!out) {
        return false;
    }
    for (auto it = m_table.begin(); it != m_table.end(); ++it) {
        out << it->first.first << "," << it->first.second << "," << it->second->GetExpression() << std::endl;
    }
    return true;
}

bool CTable::Load(const std::string & fname) {
    std::string homepath = getenv("HOME");
    std::ifstream in ((homepath + "/tejadede/examples/" + fname).c_str());
    std::string line, rowStr, colStr, expression;
    CTable newTable;
    int row, col;
    m_table.clear();
    if(in) {
        while (in >> line) {
            std::istringstream iss (line);
            getline(iss, rowStr, ',');
            getline(iss, colStr, ',');
            getline(iss, expression);
            row = atoi(rowStr.c_str());
            col = atoi(colStr.c_str());
            Insert(row, col, expression);
        }
    } else {
        return false;
    }
    return true;
}