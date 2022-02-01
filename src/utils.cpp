#include "utils.hpp"

#include <bits/stdc++.h>

#include "Course.hpp"

/**
 * Removes leading and trailing whitespace from a string.
 * @param s the input string.
 * @return the input string without leading or trailing whitespace.
 */
std::string trimWhitespace(const std::string& src) {
    int l = 0, r = src.size() - 1;
    while (l <= r && src[l] == ' ') {
        l++;
    }
    while (l <= r && src[r] == ' ') {
        r--;
    }
    return src.substr(l, r - l + 1);
}

/**
 * Splits a source string up based on a delimiter, and returns the components.
 * @param src the input string.
 * @param delimiter the delimiting character.
 * @return components a list of the split components of src.
 */
std::vector<std::string> split(const std::string& src, const char& delimiter) {
    std::vector<std::string> components;
    std::string component;
    for (const char& c : src) {
        if (c == delimiter) {
            components.emplace_back(component);
            component.clear();
        } else {
            component.push_back(c);
        }
    }
    if (!component.empty()) {
        components.emplace_back(component);
    }
    return components;
}

/**
 * Converts source string to lowercase and returns the result.
 * @param src the input string.
 * @return result the source string converted to lowercase.
 */
std::string tolower(const std::string& src) {
    std::string result(src);
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

/**
 * Returns a string of components joined together by a delimiting string.
 * @param components a list of split components to be joined together.
 * @param delimiter the delimiter to be inserted between components.
 * @return the components joined by delimiter.
 */
std::string join(const std::vector<std::string>& components,
                 const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < components.size() - 1; i++) {
        result += components[i];
        result += delimiter;
    }
    result += components.back();
    return result;
}

std::string joinNames(const std::vector<Course*>& courses,
                      const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < courses.size() - 1; i++) {
        result += courses[i]->getName();
        result += delimiter;
    }
    result += courses.back()->getName();
    return result;
}

std::string formatName(const std::string& name, const std::string& dept) {
    std::string formatted = trimWhitespace(name);
    if (isID(formatted)) {
        return dept + ' ' + formatted;
    }
    return formatted;
}

/**
 * Returns whether the given string is a valid course ID.
 * @param src the source string.
 * @return whether the first whitespace-delimited "word" of src has a digit.
 */
bool isID(const std::string& src) {
    for (size_t i = 0; i < src.size() && src[i] != ' '; i++) {
        if (isdigit(src[i])) {
            return true;
        }
    }
    return false;
}

/**
 * Parses a command string and returns a cmd, args pair.
 * @param src the source string.
 * @return cmd the first "word" in input, delimited by whitespace.
 * @return args a list of strings, delimited by commas (`,`).
 */
std::pair<std::string, std::vector<std::string>> parseInput(
    const std::string& src) {
    int spaceIndex =
        src.find(' ') == std::string::npos ? src.size() : src.find(' ');
    std::string cmd = tolower(src.substr(0, spaceIndex));
    std::vector<std::string> args = split(src.substr(spaceIndex), ',');
    for (std::string& arg : args) {
        arg = trimWhitespace(arg);
        arg = tolower(arg);
    }
    return {cmd, args};
}
