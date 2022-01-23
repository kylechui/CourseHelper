#include <bits/stdc++.h>
#include "Course.hpp"

Course::Course(const std::string& name) :
    m_name(name) {
        m_description = "No description provided.";
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

void Course::setDescription(const std::string& description) {
    m_description = description;
}

std::vector<Course*> Course::getPrereqs() {
    return m_prereqs;

}

std::vector<std::set<Course*>> Course::getChoices() {
    return m_choices;
}

void Course::addPrereq(Course* prereq) {
    m_prereqs.push_back(prereq);
}

void Course::addChoice(std::set<Course*>& choice) {
    m_choices.push_back(choice);
}
