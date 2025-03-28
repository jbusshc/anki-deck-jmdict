#pragma once

#include <string>

// Retorna true si existe substr en str
bool inSubstr(const std::string& str, const std::string& substr);

bool isEmpty(const std::string str);
void replaceWhiteSpaces(std::string& text);
void cleanText(std::string& text);
bool isCommon(const std::string&);