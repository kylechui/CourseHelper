#include "User.hpp"

#include <bits/stdc++.h>

#include "Course.hpp"
#include "utils.hpp"

User::User() {}

User::User(const std::unordered_map<std::string, Course *> &courseMap,
           const std::string &file)
    : m_file(file) {
    std::ifstream userInfo(m_file, std::ifstream::in);
    std::string name;
    while (getline(userInfo, name)) {
        m_takenCourses.insert(courseMap.at(tolower(name)));
    }
    userInfo.close();
}

std::set<Course *> User::getTakenCourses() const { return m_takenCourses; }

void User::addCourses(
    const std::unordered_map<std::string, Course *> &courseMap,
    const std::vector<std::string> &courseNames) {
    std::ofstream userInfo(m_file, std::ios::ate | std::ios::app);
    for (const std::string &name : courseNames) {
        if (courseMap.find(name) == courseMap.end()) {
            std::cout << "Course \"" + name + "\" not found, try again."
                      << std::endl;
            continue;
        }
        m_takenCourses.insert(courseMap.at(name));
        userInfo << name << '\n';
    }
    userInfo.close();
}

void User::removeCourses(
    const std::unordered_map<std::string, Course *> &courseMap,
    const std::vector<std::string> &courseNames) {
    for (const std::string &name : courseNames) {
        if (courseMap.find(name) == courseMap.end()) {
            std::cout << "Course \"" + name + "\" not found, try again."
                      << std::endl;
            continue;
        }
        m_takenCourses.erase(courseMap.at(name));
    }
    std::ofstream userInfo(m_file, std::ios::ate);
    for (Course *course : m_takenCourses) {
        userInfo << course->getName() << '\n';
    }
    userInfo.close();
}

void User::printTakenCourses() const {
    if (m_file.empty()) {
        std::cout << "You are not logged in. Log in or create a new profile "
                     "via `login [username]'."
                  << std::endl;
    } else if (m_takenCourses.empty()) {
        std::cout << "You have not taken any courses yet." << std::endl;
    } else {
        std::cout << "You have taken:" << std::endl;
        for (Course *course : m_takenCourses) {
            std::cout << course->getName() << std::endl;
        }
    }
}

bool User::hasTaken(Course *course) {
    return m_takenCourses.find(course) != m_takenCourses.end();
}

std::tuple<std::vector<Course *>, std::vector<std::vector<Course *>>,
           std::vector<std::vector<std::vector<Course *>>>>
User::getRemainingPrereqs(Course *course) {
    auto [requiredReqs, choiceReqs, seriesReqs] = course->getAllPrereqs();
    for (Course *taken : m_takenCourses) {
        for (Course *&req : requiredReqs) {
            // Remove the class if it's been found
            if (taken == req) {
                std::swap(req, requiredReqs.back());
                requiredReqs.pop_back();
                break;
            }
        }
        for (std::vector<Course *> &choice : choiceReqs) {
            bool found = false;
            for (Course *&option : choice) {
                if (taken == option) {
                    found = true;
                    break;
                }
            }
            // If the class is one of the choices, remove that choice (since it
            // has been satisfied)
            if (found) {
                std::swap(choice, choiceReqs.back());
                choiceReqs.pop_back();
                break;
            }
        }
        for (std::vector<std::vector<Course *>> &seriesReq : seriesReqs) {
            for (std::vector<Course *> &series : seriesReq) {
                for (Course *&option : series) {
                    if (taken == option) {
                        std::swap(option, series.back());
                        series.pop_back();
                        break;
                    }
                }
                if (series.empty()) {
                    swap(series, seriesReq.back());
                    seriesReq.pop_back();
                }
            }
            if (seriesReq.empty()) {
                swap(seriesReq, seriesReqs.back());
                seriesReqs.pop_back();
            }
        }
    }
    return {requiredReqs, choiceReqs, seriesReqs};
}

/* bool User::hasAllPrereqs(Course *course) {
    auto [required, choices, allPathways] = course->getAllPrereqs();
    for (Course *prereq : required) {
        if (m_takenCourses.find(prereq) == m_takenCourses.end()) {
            return false;
        }
    }
    for (std::vector<Course *> &choice : choices) {
        bool has = false;
        for (Course *option : choice) {
            if (m_takenCourses.find(option) != m_takenCourses.end()) {
                has = true;
                break;
            }
        }
        if (!has) {
            return false;
        }
    }
    for (std::vector<std::vector<Course *>> &pathways : allPathways) {
        bool hasPathway = false;
        for (std::vector<Course *> &pathway : pathways) {
            bool hasAll = true;
            for (Course *option : pathway) {
                if (m_takenCourses.find(option) == m_takenCourses.end()) {
                    hasAll = false;
                    break;
                }
            }
            if (hasAll) {
                hasPathway = true;
                break;
            }
        }
        if (!hasPathway) {
            return false;
        }
    }
    return true;
} */
