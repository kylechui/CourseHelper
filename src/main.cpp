#include <bits/stdc++.h>

#include <algorithm>
#include <cctype>

#include "Course.hpp"
#include "User.hpp"
#include "utils.hpp"

void cleanup(std::unordered_map<std::string, Course *> &courseMap, User *user) {
    delete (user);
    for (auto &[name, course] : courseMap) {
        delete (course);
    }
}

void printHelpMessage() {
    const std::vector<std::pair<std::string, std::string>> helpMessage = {
        {"help", "Shows this list of all commands"},
        {"exit", "Exits the program"},
        {"", ""},
        {"list", "Shows list of courses that have already been taken"},
        {"take [course]", "Adds course to the list of taken courses"},
        {"info [course]", "Shows information about a given course"},
        {"prereq [course]", "Lists all prereqs needed to take a given course"},
        {"available [department]",
         "Shows list of courses that are currently available"},
        {"update [department]",
         "Updates course information for a given department"},
    };
    for (const auto &[cmd, desc] : helpMessage) {
        std::cout << std::left << std::setw(25) << cmd << ' ' << desc
                  << std::endl;
    }
}

void printAvailableCourses(
    const std::unordered_map<std::string, Course *> &courseMap, User *user,
    const std::string &dept) {
    std::vector<const Course *> availableCourses;

    for (const auto &[name, course] : courseMap) {
        auto [requiredReqs, choiceReqs, seriesReqs] =
            user->getRemainingPrereqs(course);
        if (course->getDepartment() != dept) {
            continue;
        }

        if (!user->hasTaken(course) && requiredReqs.empty() &&
            choiceReqs.empty() && seriesReqs.empty()) {
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
    std::string lowercaseName = tolower(name);
    if (courseMap.find(lowercaseName) == courseMap.end()) {
        courseMap[lowercaseName] = new Course(name);
    }
    return courseMap[lowercaseName];
}

void loadFile(const std::string &pathPrefix,
              std::unordered_map<std::string, Course *> &courseMap) {
    const std::string prefix = "./Courses/";
    // Get the department name
    std::string dept(pathPrefix.begin() + prefix.size(), pathPrefix.end());
    replace(dept.begin(), dept.end(), '_', ' ');
    // Read in all input files simultaneously for the given department
    std::ifstream IDFile(pathPrefix + "_IDs.txt", std::ifstream::in);
    std::ifstream descFile(pathPrefix + "_descriptions.txt", std::ifstream::in);
    std::ifstream prereqFile(pathPrefix + "_prereqs.txt", std::ifstream::in);
    std::string IDLine, prereqLine, descLine;
    while (getline(IDFile, IDLine) && getline(prereqFile, prereqLine) &&
           getline(descFile, descLine)) {
        std::string name = dept + ' ' + IDLine;
        Course *curCourse = getCourse(courseMap, name);
        curCourse->setDescription(descLine);
        // If there are no prereqs, go to the next Course
        if (prereqLine.empty()) {
            continue;
        }
        // Parse prereq and add the corresponding prereqs to the current Course
        // First split by comma (but only if not inside parens)
        std::vector<std::string> components;
        std::string component;
        int depth = 0;
        for (const char &c : prereqLine) {
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
        // Process each comma-separated "component" of the input string
        for (std::string &component : components) {
            // Determine whether the current component is a part of a choice,
            // series, or neither
            bool isChoice = false, isSeries = false;
            for (const char &c : component) {
                if (c == '|') {
                    isChoice = true;
                    break;
                }
                if (c == ',') {
                    isSeries = true;
                    break;
                }
            }
            if (isChoice) {
                // Handle choice case
                std::vector<std::string> choiceNames = split(component, '|');
                std::vector<Course *> choices;
                for (std::string &choiceName : choiceNames) {
                    choices.emplace_back(
                        getCourse(courseMap, formatName(choiceName, dept)));
                }
                curCourse->addChoiceReq(choices);
            } else if (isSeries) {
                // Handle series case
                std::vector<std::string> seriesNames = split(component, '|');
                std::vector<std::vector<Course *>> seriesReq;
                for (std::string &seriesName : seriesNames) {
                    std::vector<Course *> series;
                    std::vector<std::string> seriesComponent =
                        split(seriesName, ',');
                    for (std::string &tmp : seriesComponent) {
                        series.emplace_back(
                            getCourse(courseMap, formatName(tmp, dept)));
                    }
                    seriesReq.emplace_back(series);
                }
                curCourse->addSeriesReq(seriesReq);
            } else {
                // If neither of the above cases, just treat the component as a
                // Course name and add it to courseMap
                curCourse->addRequiredReq(
                    getCourse(courseMap, formatName(component, dept)));
            }
        }
    }
    IDFile.close();
    prereqFile.close();
    descFile.close();
}

int main() {
    // Define input codes to denote various actions
    enum inputCode {
        AVAILABLE,
        EXIT,
        HELP,
        INFO,
        LIST,
        LOGIN,
        LOGOUT,
        PREREQ,
        REMOVE,
        TAKE,
        UPDATE,
    };
    // Input table that will validate inputs from the user, returns input codes
    const std::unordered_map<std::string, int> validInputs = {
        {"available", AVAILABLE}, {"exit", EXIT},     {"help", HELP},
        {"info", INFO},           {"list", LIST},     {"login", LOGIN},
        {"logout", LOGOUT},       {"prereq", PREREQ}, {"remove", REMOVE},
        {"take", TAKE},           {"update", UPDATE},
    };
    // Load in prerequisite info from local text files
    std::unordered_map<std::string, Course *> courseMap;
    std::vector<std::string> departments;
    for (auto &entry : std::filesystem::directory_iterator("./Courses")) {
        std::string filename = std::string(entry.path());
        const std::string prefix = "./Courses/", suffix = "_prereqs.txt";
        const size_t psz = prefix.size(), ssz = suffix.size();
        if (filename.substr(filename.size() - ssz) == suffix) {
            loadFile(filename.substr(0, filename.size() - ssz), courseMap);
            std::string dept =
                filename.substr(psz, filename.size() - psz - ssz);
            replace(dept.begin(), dept.end(), '_', ' ');
            departments.push_back(dept);
        }
    }
    // Initialize user (anonymous)
    User *user = new User();
    std::string input;
    while (getline(std::cin, input)) {
        auto [cmd, args] = parseInput(input);
        // Check if the input command is valid or not
        if (validInputs.find(cmd) == validInputs.end()) {
            std::cout << "Invalid input. Type `help' for a list of commands."
                      << std::endl;
            continue;
        }
        // Handle valid commands
        switch (validInputs.at(cmd)) {
            case AVAILABLE: {
                bool found = false;
                for (const std::string &department : departments) {
                    if (args[0] == tolower(department)) {
                        printAvailableCourses(courseMap, user, department);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Department not found. Please try again."
                              << std::endl;
                }
                break;
            }
            case EXIT: {
                cleanup(courseMap, user);
                return 0;
            }
            case HELP: {
                printHelpMessage();
                break;
            }
            case INFO: {
                if (!args.empty() &&
                    courseMap.find(args[0]) != courseMap.end()) {
                    courseMap[args[0]]->printInfo(user);
                } else {
                    std::cout << "Course not found. Please try again."
                              << std::endl;
                }
                break;
            }
            case LIST: {
                user->printTakenCourses();
                break;
            }
            case LOGIN: {
                std::string username(args[0]);
                if (username.empty()) {
                    std::cout << "No username detected. Please try again."
                              << std::endl;
                }
                // Format the user-passed argument into a valid path
                std::replace(args[0].begin(), args[0].end(), ' ', '_');
                std::string userString = "./Users/" + args[0] + ".txt";
                std::filesystem::path userPath(userString);
                if (!std::filesystem::exists(userPath)) {
                    std::cout << "User `" + username +
                                     "' not found. Would you like to create a "
                                     "new profile? [Y/n] "
                              << std::endl;
                    std::string response;
                    getline(std::cin, response);
                    if (tolower(response[0]) != 'y') {
                        break;
                    }
                }
                delete (user);
                user = new User(courseMap, userPath);
                break;
            }
            case LOGOUT: {
                delete (user);
                user = new User();
                break;
            }
            case PREREQ: {
                if (!args.empty() &&
                    courseMap.find(args[0]) != courseMap.end()) {
                    courseMap[args[0]]->printPrereqs();
                } else {
                    std::cout << "Course not found. Please try again."
                              << std::endl;
                }
                break;
            }
            case TAKE: {
                user->addCourses(courseMap, args);
                break;
            }
            case REMOVE: {
                user->removeCourses(courseMap, args);
                break;
            }
            case UPDATE: {
                std::ignore =
                    std::system(("python3 ./scraper.py " + args[0]).c_str());
                break;
            }
            default: {
                std::cout << "Invalid command. Type `help' to get a list "
                             "of commands."
                          << std::endl;
                break;
            }
        }
    }
    cleanup(courseMap, user);
}
