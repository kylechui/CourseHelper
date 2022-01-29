#include <bits/stdc++.h>

#include "Course.hpp"
#include "User.hpp"
#include "utils.hpp"

void printHelpMessage() {
    const std::vector<std::pair<std::string, std::string>> helpMessage = {
        {"help", "Shows this list of all commands"},
        {"exit", "Exits the program"},
        {"", ""},
        {"list", "Shows list of courses that have already been taken"},

        {"add [course]", "Adds course to the courses database"},
        {"take [course]", "Adds course to the list of taken courses"},
        {"info [course]", "Shows information about a given course"},
        {"prereq [course]", "Lists all prereqs needed to take a given course"},
        {"available [department]",
         "Shows list of courses that are currently available"},
        {"update [department]",
         "Updates course information for a given department"},
    };
    for (auto &[cmd, desc] : helpMessage) {
        std::cout << std::left << std::setw(25) << cmd << ' ' << desc
                  << std::endl;
    }
}

void printAvailableCourses(
    const std::unordered_map<std::string, Course *> &courseMap,
    const User &user, const std::string &dept) {
    std::vector<const Course *> availableCourses;
    for (const auto &[name, course] : courseMap) {
        if (!isPrefix(name, dept)) {
            continue;
        }

        // TODO: Figure out how to avoid creating a new pointer here
        const Course *tmp = course;
        if (!user.hasTaken(tmp) && user.hasAllPrereqs(tmp)) {
            availableCourses.push_back(course);
        }
    }
    sort(begin(availableCourses), end(availableCourses));
    std::cout << "Here's a list of courses you can take in the " + dept +
                     " department:"
              << std::endl;
    for (const Course *c : availableCourses) {
        std::cout << "* " << c->getName() << std::endl;
    }
}

/**
 * Returns a pointer to the Course object corresponding to a given name, creates
 * one otherwise.
 * @param courseMap a map from names to Course pointers.
 * @param name the name of the Course.
 * @return a pointer to a Course object with name name.
 */
Course *getCourse(std::unordered_map<std::string, Course *> &courseMap,
                  const std::string &name) {
    if (courseMap.find(name) == courseMap.end()) {
        courseMap[name] = new Course(name);
    }
    return courseMap[name];
}

void loadFile(const std::string &pathPrefix,
              std::unordered_map<std::string, Course *> &courseMap) {
    const std::string prefix = "./Courses/";
    // Get the department name
    std::string dept(pathPrefix.begin() + prefix.size(), pathPrefix.end());
    replace(dept.begin(), dept.end(), '_', ' ');
    // Read in all input files simultaneously for the given department
    std::ifstream IDs(pathPrefix + "_IDs.txt", std::ifstream::in);
    std::ifstream descriptions(pathPrefix + "_descriptions.txt",
                               std::ifstream::in);
    std::ifstream prereqs(pathPrefix + "_prereqs.txt", std::ifstream::in);
    std::string ID, prereq, desc;
    while (getline(IDs, ID) && getline(prereqs, prereq) &&
           getline(descriptions, desc)) {
        std::string name = dept + ' ' + ID;
        Course *curCourse = getCourse(courseMap, name);
        curCourse->setDescription(desc);
        // If there are no prereqs, go to the next Course
        if (prereq.empty()) {
            continue;
        }
        // Parse prereq and add the corresponding prereqs to the current Course
        // First split by comma (but only if not inside parens)
        std::vector<std::string> components;
        std::string component;
        int depth = 0;
        for (const char &c : prereq) {
            if (c == ',' && depth == 0) {
                components.emplace_back(component);
                component.clear();
            } else if (c == '(') {
                depth++;
            } else if (c == ')') {
                depth--;

            } else {
                component.push_back(c);
            }
        }
        if (!component.empty()) {
            components.emplace_back(component);
        }
        // Find whether each component is:
        // * A standalone course name
        // * A choice (choose one course from the list)
        // * A pathway (choose one sublist from a list of lists)
        // Then add it to the current Course using the corresponding method
        for (std::string &component : components) {
            bool isChoice = false, isPathway = false;
            for (const char &c : component) {
                if (c == '|') {
                    isChoice = true;
                    break;
                }
                if (c == ',') {
                    isPathway = true;
                    break;
                }
            }
            if (isChoice) {
                std::vector<std::string> choiceNames = split(component, '|');
                std::vector<const Course *> choices;
                for (std::string &choiceName : choiceNames) {
                    choiceName = trimWhitespace(choiceName);
                    if (isID(choiceName)) {
                        choiceName = dept + ' ' + choiceName;
                    }
                    choices.emplace_back(getCourse(courseMap, choiceName));
                }
                curCourse->addChoice(choices);
            } else if (isPathway) {
                std::vector<std::string> pathwayNames = split(component, ',');
                std::vector<const Course *> pathways;
                for (std::string &pathwayName : pathwayNames) {
                    pathwayName = trimWhitespace(pathwayName);
                    if (isID(pathwayName)) {
                        pathwayName = dept + ' ' + pathwayName;
                    }
                    pathways.emplace_back(
                        getCourse(courseMap, trimWhitespace(pathwayName)));
                }
                curCourse->addPathway(pathways);
            } else {
                component = trimWhitespace(component);
                if (isID(component)) {
                    component = dept + ' ' + component;
                }
                curCourse->addPrereq(getCourse(courseMap, component));
            }
        }
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
        {"available", AVAILABLE}, {"exit", EXIT}, {"help", HELP},
        {"info", INFO},           {"list", LIST}, {"prereq", PREREQ},
        {"remove", REMOVE},       {"take", TAKE}, {"update", UPDATE},
    };
    // Load in prerequisite info from local text files
    std::unordered_map<std::string, Course *> courseMap;
    std::unordered_set<std::string> departments;
    for (auto &entry : std::filesystem::directory_iterator("./Courses")) {
        std::string filename = std::string(entry.path());
        const std::string prefix = "./Courses/", suffix = "_prereqs.txt";
        size_t psz = prefix.size(), ssz = suffix.size();
        if (filename.substr(filename.size() - ssz) == suffix) {
            loadFile(filename.substr(0, filename.size() - ssz), courseMap);
            std::string dept =
                filename.substr(psz, filename.size() - psz - ssz);
            replace(dept.begin(), dept.end(), '_', ' ');
            departments.insert(dept);
        }
    }
    // Load in the user info
    User user(courseMap, "user.txt");
    std::string input;
    while (getline(std::cin, input)) {
        const auto [cmd, args] = parseInput(input);
        // Check if the input command is valid or not
        if (validInputs.find(cmd) == validInputs.end()) {
            std::cout << "Invalid input. Type `help' for a list of commands."
                      << std::endl;
            continue;
        }
        // Handle valid commands
        switch (validInputs.at(cmd)) {
            case AVAILABLE:
                // TODO: Add code to list all available classes based on DAG
                if (!args.empty() &&
                    departments.find(args[0]) != departments.end()) {
                    printAvailableCourses(courseMap, user, args[0]);
                } else {
                    std::cout << "Department not found. Please try again."
                              << std::endl;
                }
                break;
            case EXIT:
                return 0;
            case HELP:
                printHelpMessage();
                break;
            case INFO:
                if (!args.empty() &&
                    courseMap.find(args[0]) != courseMap.end()) {
                    courseMap[args[0]]->printInfo(user);
                } else {
                    std::cout << "Course not found. Please try again."
                              << std::endl;
                }
                break;
            case LIST:
                user.printTakenCourses();
                break;
            case PREREQ:
                if (!args.empty() &&
                    courseMap.find(args[0]) != courseMap.end()) {
                    courseMap[args[0]]->printPrereqs();
                } else {
                    std::cout << "Course not found. Please try again."
                              << std::endl;
                }
                break;
            case TAKE:
                user.addCourses(courseMap, args);
                break;
            case REMOVE:
                user.removeCourses(courseMap, args);
                break;
            case UPDATE:
                std::ignore =
                    std::system(("python3 ./scraper.py " + args[0]).c_str());
                break;
        }
    }
}
