#include <bits/stdc++.h>
#include "Course.hpp"

Course::Course(const std::string& name, const std::string& description) :
    m_name(name), m_description(description) {
}

Course::~Course() {
    for (Course* p : m_prereqs) {
        delete(p);
    }
}

std::string Course::getName() {
    return m_name;
}

std::string Course::getDescription() {
    return m_description;
}

std::vector<Course*> Course::getPrereqs() {
    return m_prereqs;

}

std::vector<std::set<Course*>> Course::getChoices() {
    return m_choices;
}

void Course::addPrereq(Course* other) {
    m_prereqs.push_back(other);
}

void Course::addPrereqs(std::set<Course*>& other) {
    m_choices.push_back(other);
}
