#include <bits/stdc++.h>
#include "Course.hpp"
#include "User.hpp"

// TODO: Move these into utils
std::string trim(const std::string& s) {
    int l = 0, r = s.size() - 1;
    while (l <= r && s[l] == ' ') {
        l++;
    }
    while (l <= r && s[r] == ' ') {
        r--;
    }
    return s.substr(l, r - l + 1);
}

// TODO: Move these into utils
bool isPrefix(const std::string& s, const std::string& prefix) {
    if (prefix.size() > s.size()) {
        return false;
    }
    for (size_t i = 0; i < prefix.size(); i++) {
        if (prefix[i] != s[i]) {
            return false;
        }
    }
    return true;
}

std::pair<std::string, std::vector<std::string>> parseInput(const std::string& input) {
    std::string cmd;
    std::vector<std::string> args;
    size_t i;
    // Read up until the first whitespace character as the command
    for (i = 0; i < input.size(); i++) {
        if (input[i] == ' ') {
            break;
        }
        cmd.push_back(input[i]);
    }
    // The remaining characters are comma-separated arguments
    std::string curArg;
    for (; i < input.size(); i++) {
        if (input[i] == ',') {
            args.push_back(trim(curArg));
            curArg.clear();
        }
        else {
            curArg.push_back(input[i]);
        }
    }
    if (!curArg.empty()) {
        args.push_back(trim(curArg));
    }
    return { cmd, args };
}

void printHelpMessage() {
    const std::vector<std::pair<std::string, std::string>> helpMessage = {
        { "help", "Shows this list of all commands" },
        { "exit", "Exits the program" },
        { "", "" },
        { "list", "Shows list of courses that have already been taken" },
        { "add [course]", "Adds course to the courses database" },
        { "take [course]", "Adds course to the list of taken courses" },
        { "info [course]", "Shows information about a given course" },
        { "prereq [course]", "Lists all prereqs needed to take a given course" },
        { "available [department]", "Shows list of courses that are currently available" },
        { "update [department]", "Updates course information for a given department" },
    };
    for (auto& [cmd, desc] : helpMessage) {
        std::cout << std::left << std::setw(25) << cmd << ' ' << desc << std::endl;
    }
}

Course* getCourse(const std::string& name, std::unordered_map<std::string, Course*>& courseMap) {
    if (courseMap.find(name) != courseMap.end()) {
        return courseMap[name];
    }
    return new Course(name);
}

// TODO: Make this a member function of the Course class (pass user as param)
void printInfo(Course* const& course, User& user) {
    std::vector<Course*> prereqs = course->getPrereqs();
    std::vector<std::set<Course*>> choices = course->getChoices();
    std::cout << course->getName() << std::endl << std::endl;
    std::cout << course->getDescription() << std::endl << std::endl;
    // Print prereq information
    if (user.hasTaken(course)) {
        std::cout << "You have already taken this class." << std::endl;
    }
    else if (user.hasAllPrereqs(course)) {
        std::cout << "You can take this class next quarter!" << std::endl;
    }
    else {
        if (prereqs.size() == 0 && choices.size() == 0) {
            std::cout << "There are no requirements for this class." << std::endl;
        }
        if (prereqs.size() != 0) {
            std::cout << "You still need to take the following courses:" << std::endl;
            for (Course* prereq : prereqs) {
                if (!user.hasTaken(prereq)) {
                    std::cout << "* " << prereq->getName() << std::endl;
                }
            }
        }
        if (choices.size() != 0) {
            for (std::set<Course*>& choice : choices) {
                std::cout << "You may choose at least one from the following:" << std::endl;
                for (Course* prereq : choice) {
                    std::cout << "- " << prereq->getName() << std::endl;
                }
            }
        }
    }
    std::cout << "========================================" << std::endl;
}

// TODO: Make this a member function of the Course class (no params)
std::vector<Course*> getAllPrereqs(Course*& course) {
    std::unordered_set<Course*> processed;
    auto dfs = [&processed](Course*& cur, auto&& dfs) -> void {
        processed.insert(cur);
        for (Course*& prereq : cur->getPrereqs()) {
            if (processed.find(prereq) == processed.end()) {
                dfs(prereq, dfs);
            }
        }
    };
    dfs(course, dfs);
    processed.erase(course);
    std::vector<Course*> sorted(processed.begin(), processed.end());
    sort(begin(sorted), end(sorted));
    return sorted;
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
        size_t i = 0;
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
                size_t j = 0;
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
        PREREQ,
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
        { "prereq", PREREQ },
        { "remove", REMOVE },
        { "take", TAKE },
        { "update", UPDATE },
    };
    // Load in prerequisite info from local text files
    std::unordered_map<std::string, Course*> courseMap;
    std::unordered_set<std::string> departments;
    for (auto& entry : std::filesystem::directory_iterator("./Courses")) {
        std::string filename = std::string(entry.path());
        if (filename.substr(filename.size() - 8) == "_IDs.txt") {
            loadFile(filename.substr(0, filename.size() - 8), courseMap);
            std::string dept = filename.substr(10, filename.size() - 18);
            for (char& c : dept) {
                if (c == '_') {
                    c = ' ';
                }
            }
            departments.insert(dept);
        }
    }
    // Load in the user info 
    User user("user.txt");

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
                if (!args.empty() && departments.find(args[0]) != departments.end()) {
                    std::vector<Course*> availableCourses;
                    for (const auto& [name, course] : courseMap) {
                        if (!isPrefix(name, args[0])) {
                            continue;
                        }
                        if (!user.hasTaken(course) && user.hasAllPrereqs(course)) {
                            availableCourses.push_back(course);
                        }
                    }
                    sort(begin(availableCourses), end(availableCourses));
                    std::cout << "Here's a list of courses you can take in the " + args[0] + " department:" << std::endl;
                    for (Course*& c : availableCourses) {
                        std::cout << "* " << c->getName() << std::endl;
                    }
                }
                else {
                    std::cout << "Department not found. Please try again." << std::endl;
                }
                break;
            case EXIT:
                return 0;
            case HELP:
                printHelpMessage();
                break;
            case INFO:
                if (!args.empty() && courseMap.find(args[0]) != courseMap.end()) {
                    printInfo(courseMap[args[0]], user);
                }
                else {
                    std::cout << "Course not found. Please try again." << std::endl;
                }
                break;
            case LIST:
                user.printTakenCourses();
                break;
            case PREREQ:
                if (!args.empty() && courseMap.find(args[0]) != courseMap.end()) {
                    std::cout << "You need the following:" << std::endl;
                    for (Course*& course : getAllPrereqs(courseMap[args[0]])) {
                        std::cout << "* " << course->getName() << std::endl;
                    }
                }
                else {
                    std::cout << "Course not found. Please try again." << std::endl;
                }
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
                std::ignore = std::system(("python3 ./scraper.py " + args[0]).c_str());
                break;
        }
    }
}
