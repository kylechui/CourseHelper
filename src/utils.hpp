#ifndef UTILS_H_
#define UTILS_H_

#include <bits/stdc++.h>
class Course;

std::string trimWhitespace(const std::string& src);
std::vector<std::string> split(const std::string& src, const char& delimiter);
std::string tolower(const std::string& src);
std::string join(const std::vector<std::string>& components,
                 const std::string& delimiter);
std::string joinNames(const std::vector<Course*>& courses,
                      const std::string& delimiter);
std::string formatName(const std::string& name, const std::string& dept);
bool isID(const std::string& src);
std::pair<std::string, std::vector<std::string>> parseInput(
    const std::string& src);

#endif
