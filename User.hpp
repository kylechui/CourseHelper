#ifndef USER_H_
#define USER_H_

#include <bits/stdc++.h>
#include "Course.hpp"

class User {
public:
    User(const std::string& file);
    void addCourse(const std::string& courseID);
    void removeCourse(const std::string& courseID);
    std::set<std::string> getTakenCourses();
    void printTakenCourses();
    bool hasTaken(Course* const& course);
    bool hasAllPrereqs(Course* const& course);
private:
    std::set<std::string> m_takenCourses;
    const std::string m_file;
};

#endif
