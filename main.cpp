#include <bits/stdc++.h>
#include "Course.hpp"
#include "User.hpp"

void trim(std::string& s) {
    int i = 0;
    while (i < s.size() && s[i] == ' ') {
        i++;
    }
    int diff = i;
    while (i < s.size()) {
        s[i - diff] = s[i];
        s[i++] = ' ';
    }
    while (!s.empty() && s.back() == ' ') {
        s.pop_back();
    }
}

std::pair<std::string, std::vector<std::string>> parseInput(const std::string& input) {
    std::string cmd;
    std::vector<std::string> args;
    size_t i;
    for (i = 0; i < input.size(); i++) {
        if (input[i] == ' ') {
            break;
        }
        cmd.push_back(input[i]);
    }
    std::string curArg;
    for (; i < input.size(); i++) {
        if (input[i] == ',') {
            trim(curArg);
            args.push_back(curArg);
            curArg.clear();
        }
        else {
            curArg.push_back(input[i]);
        }
    }
    if (!curArg.empty()) {
        trim(curArg);
        args.push_back(curArg);
    }
    return { cmd, args };
}

void printHelpMessage() {
    const std::vector<std::pair<std::string, std::string>> helpMessage = {
        { "help", "Shows this list of all commands" },
        { "exit", "Exits the program" },
        { "", "" },
        { "available", "Shows list of courses that are currently available" },
        { "list", "Shows list of courses that have already been taken" },
        { "add [course]", "Adds course to the courses database" },
        { "take [course]", "Adds course to the list of taken courses" },
        { "info [ID]", "Shows information about a given course ID" },
        { "update [department]", "Updates course information for a given department" },
    };
    for (auto& [cmd, desc] : helpMessage) {
        std::cout << std::left << std::setw(15) << cmd << ' ' << desc << std::endl;
    }
}

int main() {
    // Define input codes to denote various actions
    enum inputCode {
        AVAILABLE,
        EXIT,
        HELP,
        INFO,
        LIST,
        REMOVE,
        TAKE,
        UPDATE,
    };
    // Input table that will validate inputs from the user, returns input codes
    const std::unordered_map<std::string, int> validInputs = {
        { "available", AVAILABLE },
        { "exit", EXIT },
        { "help", HELP },
        { "info", INFO },
        { "list", LIST },
        { "remove", REMOVE },
        { "take", TAKE },
        { "update", UPDATE },
    };
    // Load in the user info 
    User user("user.txt");
    // TODO: Load in courses info from *all* database files
    // Format database files [DEPT]_[IDs|descriptions].txt

    std::string input;
    while (getline(std::cin, input)) {
        // Parse the input command
        auto [cmd, args] = parseInput(input);
        // Check if the input command is valid or not
        if (validInputs.find(cmd) == validInputs.end()) {
            std::cout << "Invalid input. Type `help' for a list of commands." << std::endl;
            continue;
        }
        // Handle valid commands
        switch (validInputs.at(cmd)) {
            case AVAILABLE:
                // TODO: Add code to list all available classes based on DAG
                break;
            case EXIT:
                return 0;
            case HELP:
                printHelpMessage();
                break;
            case INFO:
                // TODO: Add code that returns basic information about a given class,
                // including missing pre-reqs (if any)
                break;
            case LIST:
                user.printTakenCourses();
                break;
            case TAKE:
                for (const std::string& course : args) {
                    user.addCourse(course);
                }
                break;
            case REMOVE:
                for (const std::string& course : args) {
                    user.removeCourse(course);
                }
                break;
            case UPDATE:
                std::system(("python3 ./scraper.py " + args[0]).c_str());
                break;
        }
    }
}
