/**
 * @file CCell.cpp
 * @author tejadede
 * @brief Cell file
 */
#include "CCell.h"
CCell::CCell() {}

CCell::CCell(const CCell & cell): m_value(cell.m_value) {}

CCell::~CCell() {}

void CCell::Refresh() {}

std::string CCell::GetExpression() const {
    return m_value;
}

double CCell::GetValue() const {
    throw std::logic_error("ERROR: Cell contains a string.");
}

std::string CCell::GetValueStr() const {
    return m_value;
}

std::set<std::pair<int, int>> CCell::Update(const std::string & expression) {
    std::set < std::pair <int, int> > relatedTo;
    m_value = expression;
    return relatedTo;
}

unsigned int CCell::GetId() const {
    return m_cellId;
}

void CCell::SetId(unsigned int id) {
    m_cellId = id;
}