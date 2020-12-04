/**
 * @file CString.cpp
 * @author tejadede
 * @brief String file
 */
#include "CString.h"
CString::CString() {}

CString::CString(const CString & cstr): CCell(cstr) {
	m_value = cstr.m_value;
}

CString::~CString() {}