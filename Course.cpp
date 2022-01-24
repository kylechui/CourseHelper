#include <bits/stdc++.h>
#include "Course.hpp"
#include "User.hpp"

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

std::vector<Course*> Course::getPrereqs() {
    return m_prereqs;
}

std::vector<std::set<Course*>> Course::getChoices() {
    return m_choices;
}

std::vector<Course*> Course::getAllPrereqs() {
    std::unordered_set<Course*> processed;
    auto dfs = [&processed](Course*& cur, auto&& dfs) -> void {
        processed.insert(cur);
        for (Course*& prereq : cur->getPrereqs()) {
            if (processed.find(prereq) == processed.end()) {
                dfs(prereq, dfs);
            }
        }
    };
    for (Course* prereq : m_prereqs) {
        dfs(prereq, dfs);
    }
    std::vector<Course*> sorted(processed.begin(), processed.end());
    sort(begin(sorted), end(sorted));
    return sorted;
}

void Course::setDescription(const std::string& description) {
    m_description = description;
}

void Course::addPrereq(Course* prereq) {
    m_prereqs.push_back(prereq);
}

void Course::addChoice(std::set<Course*>& choice) {
    m_choices.push_back(choice);
}

// TODO: Format the printing to be at *most* n characters wide
void Course::printInfo(User& user) {
    std::cout << m_name << std::endl << std::endl;
    std::cout << m_description << std::endl << std::endl;
    // Print prereq information
    if (user.hasTaken(this)) {
        std::cout << "You have already taken this class." << std::endl;
    }
    else if (user.hasAllPrereqs(this)) {
        std::cout << "You can take this class next quarter!" << std::endl;
    }
    else {
        if (m_prereqs.size() == 0 && m_choices.size() == 0) {
            std::cout << "There are no requirements for this class." << std::endl;
        }
        if (m_prereqs.size() != 0) {
            std::cout << "You still need to take the following courses:" << std::endl;
            for (Course* prereq : m_prereqs) {
                if (!user.hasTaken(prereq)) {
                    std::cout << "* " << prereq->getName() << std::endl;
                }
            }
        }
        if (m_choices.size() != 0) {
            for (std::set<Course*>& choice : m_choices) {
                std::cout << "You may choose at least one from the following:" << std::endl;
                for (Course* prereq : choice) {
                    std::cout << "- " << prereq->getName() << std::endl;
                }
            }
        }
    }
    std::cout << "========================================" << std::endl;
}
