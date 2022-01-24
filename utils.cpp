#include <bits/stdc++.h>
#include "utils.hpp"

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
        }
        else {
            component.push_back(c);
        }
    }
    if (!component.empty()) {
        components.emplace_back(component);
    }
    return components;
}

/**
 * Returns whether a given string is a prefix of another string.
 * @param src the source string.
 * @param prefix the potential prefix of src.
 * @return whether prefix is a prefix of src.
 */
bool isPrefix(const std::string& src, const std::string& prefix) {
    if (prefix.size() > src.size()) {
        return false;
    }
    for (size_t i = 0; i < prefix.size(); i++) {
        if (prefix[i] != src[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Parses a command string and returns a cmd, args pair.
 * @param src the source string.
 * @return cmd the first "word" in input, delimited by whitespace.
 * @return args a list of strings, delimited by commas (`,`).
 */
std::pair<std::string, std::vector<std::string>> parseInput(const std::string& src) {
    int spaceIndex = src.find(' ') == std::string::npos ? src.size() : src.find(' ');
    std::string cmd = src.substr(0, spaceIndex);
    std::vector<std::string> args = split(src.substr(spaceIndex), ',');
    for (std::string& arg : args) {
        arg = trimWhitespace(arg);
    }
    return { cmd, args };
}