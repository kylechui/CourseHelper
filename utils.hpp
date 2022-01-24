#ifndef UTILS_H_
#define UTILS_H_

#include <bits/stdc++.h>

std::string trimWhitespace(const std::string& src);
std::vector<std::string> split(const std::string& src, const char& delimiter);
bool isPrefix(const std::string& src, const std::string& prefix);
std::pair<std::string, std::vector<std::string>> parseInput(const std::string& src);

#endif
