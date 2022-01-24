#ifndef UTILS_H_
#define UTILS_H_

#include <bits/stdc++.h>

std::string trimWhitespace(const std::string& src);
std::vector<std::string> split(const std::string& src, const char& delimiter);
std::string join(const std::vector<std::string>& components, const std::string& delimiter);
bool isPrefix(const std::string& src, const std::string& prefix);
bool isID(const std::string& src);
std::pair<std::string, std::vector<std::string>> parseInput(const std::string& src);

#endif
