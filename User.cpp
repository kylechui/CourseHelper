#include <bits/stdc++.h>
#include "User.hpp"

User::User(const std::string& file) : m_file(file) {
    std::ifstream userInfo(m_file, std::ifstream::in);
    std::string course;
    while (getline(userInfo, course)) {
        m_takenCourses.insert(course);
    }
    userInfo.close();
}

void User::addCourse(const std::string &courseID) {
    m_takenCourses.insert(courseID);
    std::ofstream userInfo("user.txt", std::ios::ate|std::ios::app);
    userInfo << courseID << '\n';
    userInfo.close();
}

void User::removeCourse(const std::string &courseID) {
    m_takenCourses.erase(courseID);
    // Remove the course from the file
    std::ifstream userInfo(m_file);
    std::ofstream tmp("tmp.txt");
    std::string line;
    while (getline(userInfo, line)) {
        if (line != courseID) {
            tmp << line << '\n';
        }
    }
    userInfo.close();
    tmp.close();
    remove(m_file.c_str());
    rename("tmp.txt", m_file.c_str());
}

void User::printTakenCourses() {
    for (const std::string& course : m_takenCourses) {
        std::cout << course << std::endl;
    }
}
