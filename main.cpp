#include <algorithm>
#include <bits/stdc++.h>
#include "Course.hpp"
#include "User.hpp"

void trim(std::string& s) {
    int i = 0, sz = s.size();
    while (i < sz && s[i] == ' ') {
        i++;
    }
    int diff = i;
    while (i < sz) {
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

Course* getCourse(const std::string& name, std::unordered_map<std::string, Course*>& courseMap) {
    if (courseMap.find(name) != courseMap.end()) {
        return courseMap[name];
    }
    return new Course(name);
}

void printInfo(Course* const course) {
    std::vector<Course*> prereqs = course->getPrereqs();
    std::vector<std::set<Course*>> choices = course->getChoices();
    std::cout << course->getName() << std::endl << std::endl;
    std::cout << course->getDescription() << std::endl << std::endl;
    // Print prereq information
    if (prereqs.size() == 0 && choices.size() == 0) {
        std::cout << "There are no requirements for this class." << std::endl;
    }
    if (prereqs.size() != 0) {
        std::cout << "You must take the following courses:" << std::endl;
        for (Course* prereq : prereqs) {
            std::cout << "* " << prereq->getName() << std::endl;
        }
    }
    if (choices.size() != 0) {
        // TODO: Come up with better var name (maybe)
        for (std::set<Course*>& st : choices) {
            std::cout << "You may choose at least one from the following:" << std::endl;
            for (Course* prereq : st) {
                std::cout << "- " << prereq->getName() << std::endl;
            }
        }
    }
    std::cout << "========================================" << std::endl;
}

// TODO: Fix parsing when ( and ) are involved
void loadFile(const std::string& prefix, std::unordered_map<std::string, Course*>& courseMap) {
    // Get the department name
    std::string dept(prefix.begin() + 10, prefix.end());
    for (char& c : dept) {
        if (c == '_') {
            c = ' ';
        }
    }
    // Read in all input files simultaneously for the given department
    std::ifstream IDs(prefix + "_IDs.txt", std::ifstream::in);
    std::ifstream descriptions(prefix + "_descriptions.txt", std::ifstream::in);
    std::ifstream prereqs(prefix + "_prereqs.txt", std::ifstream::in);
    std::string ID;
    while (getline(IDs, ID)) {
        std::string prereq, desc, name = dept + ' ' + ID;
        getline(prereqs, prereq);
        getline(descriptions, desc);
        Course* curCourse = getCourse(name, courseMap);
        if (!desc.empty()) {
            curCourse->setDescription(desc);
        }
        // If there are no prereqs, go to the next Course
        if (prereq.empty()) {
            courseMap[name] = curCourse;
            continue;
        }
        // Parse prereq and add the corresponding prereqs to the current Course
        prereq.push_back(',');
        int i = 0;
        std::string prereqName;
        while (i < prereq.size()) {
            bool isChoice = false;
            while (prereq[i] != ',') {
                if (prereq[i] == '|')
                    isChoice = true;
                prereqName.push_back(prereq[i++]);
            }
            if (isChoice) {
                prereqName += " |";
                std::set<Course*> choices;
                std::string curPrereqName;
                int j = 0;
                while (j < prereqName.size()) {
                    if (prereqName[j] == '|') {
                        // Trim trailing whitespace
                        while (!curPrereqName.empty() && curPrereqName.back() == ' ') {
                            curPrereqName.pop_back();
                        }
                        if (isdigit(curPrereqName[0])) {
                            curPrereqName = dept + ' ' + curPrereqName;
                        }
                        choices.insert(getCourse(curPrereqName, courseMap));
                        curPrereqName.clear();
                        j++;
                        // Trim leading whitespace
                        while (j < prereqName.size() && prereqName[j] == ' ') {
                            j++;
                        }
                    }
                    else {
                        curPrereqName.push_back(prereqName[j++]);
                    }
                }
                curCourse->addChoice(choices);
            }
            else {
                if (isdigit(prereqName[0])) {
                    prereqName = dept + ' ' + prereqName;
                }
                curCourse->addPrereq(getCourse(prereqName, courseMap));
                prereqName.clear();
            }
            i++;
            while (i < prereq.size() && prereq[i] == ' ') {
                i++;
            }
        }
        courseMap[name] = curCourse;
    }
    IDs.close();
    prereqs.close();
    descriptions.close();
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
    // Load in prerequisite info from local text files
    std::unordered_map<std::string, Course*> courseMap;
    for (auto& entry : std::filesystem::directory_iterator("./Courses")) {
        std::string filename = std::string(entry.path());
        if (filename.substr(filename.size() - 8) == "_IDs.txt") {
            loadFile(filename.substr(0, filename.size() - 8), courseMap);
        }
    }

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
                if (courseMap.find(args[0]) != courseMap.end()) {
                    printInfo(courseMap[args[0]]);
                }
                else {
                    std::cout << "Course not found. Please try again." << std::endl;
                }
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
