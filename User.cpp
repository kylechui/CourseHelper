#include <bits/stdc++.h>
#include "User.hpp"
#include "Course.hpp"

User::User(const std::unordered_map<std::string, Course*>& courseMap, const std::string& file) : m_file(file) {
    std::ifstream userInfo(m_file, std::ifstream::in);
    std::string name;
    while (getline(userInfo, name)) {
        m_takenCourses.insert(courseMap.at(name));
    }
    userInfo.close();
}

std::set<Course*> User::getTakenCourses() {
    return m_takenCourses;
}

void User::addCourses(const std::unordered_map<std::string, Course*>& courseMap, const std::vector<std::string>& courseNames) {
    std::ofstream userInfo(m_file, std::ios::ate|std::ios::app);
    for (const std::string& name : courseNames) {
        if (courseMap.find(name) == courseMap.end()) {
            std::cout << "Course \"" + name + "\" not found, try again." << std::endl;
            continue;
        }
        m_takenCourses.insert(courseMap.at(name));
        userInfo << name << '\n';
    }
    userInfo.close();
}

void User::removeCourses(const std::unordered_map<std::string, Course*>& courseMap, const std::vector<std::string>& courseNames) {
    for (const std::string& name : courseNames) {
        if (courseMap.find(name) == courseMap.end()) {
            std::cout << "Course \"" + name + "\" not found, try again." << std::endl;
            continue;
        }
        m_takenCourses.erase(courseMap.at(name));
    }
    std::ofstream userInfo(m_file, std::ios::ate);
    for (Course* course : m_takenCourses) {
        userInfo << course->getName() << '\n';
    }
    userInfo.close();
}

void User::printTakenCourses() {
    std::cout << "You have taken:" << std::endl;
    for (Course* course : m_takenCourses) {
        std::cout << course->getName() << std::endl;
    }
}

bool User::hasTaken(Course* const& course) {
    return m_takenCourses.find(course) != m_takenCourses.end();
}

bool User::hasAllPrereqs(Course* const& course) {
    for (Course* const& prereq : course->getPrereqs()) {
        if (m_takenCourses.find(prereq) == m_takenCourses.end()) {
            return false;
        }
    }
    for (const std::set<Course*>& choice : course->getChoices()) {
        bool has = false;
        for (Course* const& option : choice) {
            if (m_takenCourses.find(option) != m_takenCourses.end()) {
                has = true;
                break;
            }
        }
        if (!has) {
            return false;
        }
    }
    return true;
}
