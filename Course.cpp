#include <bits/stdc++.h>
#include "utils.hpp"
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

std::vector<std::set<Course*>> Course::getPathways() {
    return m_pathways;
}

// TODO: change output to a tuple with mandatory, choices, pathways as vectors
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

void Course::addPathway(std::set<Course*>& pathway) {
    m_pathways.push_back(pathway);
}

// TODO: Format the printing to be at *most* n characters wide
void Course::printInfo(User& user) {
    std::cout << m_name << std::endl << std::endl;
    std::cout << m_description << std::endl << std::endl;
    if (user.hasTaken(this)) {
        std::cout << "You have already taken this class." << std::endl;
    }
    else if (user.hasAllPrereqs(this)) {
        std::cout << "You can take this class next quarter!" << std::endl;
    }
    else {
        if (m_prereqs.size() == 0 && m_choices.size() == 0 && m_pathways.size() == 0) {
            std::cout << "There are no requirements for this class." << std::endl;
        }
        if (m_prereqs.size() != 0) {
            std::cout << "You still need to take all of the following courses:" << std::endl;
            for (Course* prereq : m_prereqs) {
                if (!user.hasTaken(prereq)) {
                    std::cout << "* " << prereq->getName() << std::endl;
                }
            }
        }
        if (m_choices.size() != 0) {
            std::cout << "You must choose at least one class from each of the following rows:" << std::endl;
            for (std::set<Course*>& choice : m_choices) {
                std::vector<std::string> choiceNames;
                for (Course* course : choice) {
                    choiceNames.emplace_back(course->getName());
                }
                std::cout << "* " << join(choiceNames, ", ") << std::endl;
            }
        }
        if (m_pathways.size() != 0) {
            std::cout << "You must choose all classes from at least one of the following rows:" << std::endl;
            for (std::set<Course*>& pathway : m_pathways) {
                std::vector<std::string> pathwayNames;
                for (Course* course : pathway) {
                    pathwayNames.emplace_back(course->getName());
                }
                std::cout << "* " << join(pathwayNames, ", ") << std::endl;
            }
        }
    }
    std::cout << std::endl;
}
