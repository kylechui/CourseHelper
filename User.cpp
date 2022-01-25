#include "User.hpp"

#include <bits/stdc++.h>

#include "Course.hpp"

User::User(const std::unordered_map<std::string, Course*>& courseMap,
           const std::string& file)
    : m_file(file) {
    std::ifstream userInfo(m_file, std::ifstream::in);
    std::string name;
    while (getline(userInfo, name)) {
        m_takenCourses.insert(courseMap.at(name));
    }
    userInfo.close();
}

std::set<const Course*> User::getTakenCourses() const { return m_takenCourses; }

void User::addCourses(const std::unordered_map<std::string, Course*>& courseMap,
                      const std::vector<std::string>& courseNames) {
    std::ofstream userInfo(m_file, std::ios::ate | std::ios::app);
    for (const std::string& name : courseNames) {
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
    const std::unordered_map<std::string, Course*>& courseMap,
    const std::vector<std::string>& courseNames) {
    for (const std::string& name : courseNames) {
        if (courseMap.find(name) == courseMap.end()) {
            std::cout << "Course \"" + name + "\" not found, try again."
                      << std::endl;
            continue;
        }
        m_takenCourses.erase(courseMap.at(name));
    }
    std::ofstream userInfo(m_file, std::ios::ate);
    for (const Course* course : m_takenCourses) {
        userInfo << course->getName() << '\n';
    }
    userInfo.close();
}

void User::printTakenCourses() const {
    std::cout << "You have taken:" << std::endl;
    for (const Course* course : m_takenCourses) {
        std::cout << course->getName() << std::endl;
    }
}

bool User::hasTaken(const Course*& course) const {
    return m_takenCourses.find(course) != m_takenCourses.end();
}

bool User::hasAllPrereqs(const Course*& course) const {
    for (const Course* prereq : course->getPrereqs()) {
        if (m_takenCourses.find(prereq) == m_takenCourses.end()) {
            return false;
        }
    }
    for (const std::set<const Course*>& choice : course->getChoices()) {
        bool has = false;
        for (const Course* option : choice) {
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
