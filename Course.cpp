#include "Course.hpp"

#include <bits/stdc++.h>

#include "User.hpp"
#include "utils.hpp"

Course::Course(const std::string &name) {
    int i = name.size() - 1;
    while (name[i] != ' ') {
        i--;
    }
    m_department = name.substr(0, i);
    m_ID = name.substr(i + 1);
    m_description = "No description provided.";
}

Course::~Course() {
    for (Course *p : m_prereqs) {
        delete (p);
    }
}

std::string Course::getName() const { return m_department + ' ' + m_ID; }

std::string Course::getDescription() const { return m_description; }

std::vector<Course *> Course::getPrereqs() const { return m_prereqs; }

std::vector<std::vector<Course *>> Course::getChoices() const {
    return m_choices;
}

std::vector<std::vector<Course *>> Course::getPathways() const {
    return m_pathways;
}

// TODO: change output to a tuple with mandatory, choices, pathways as vectors
std::vector<Course *> Course::getAllPrereqs() const {
    std::unordered_set<Course *> processed;
    auto dfs = [&processed](Course *&cur, auto &&dfs) -> void {
        processed.insert(cur);
        for (Course *&prereq : cur->getPrereqs()) {
            if (processed.find(prereq) == processed.end()) {
                dfs(prereq, dfs);
            }
        }
    };
    for (Course *prereq : m_prereqs) {
        dfs(prereq, dfs);
    }
    std::vector<Course *> sorted(processed.begin(), processed.end());
    sort(begin(sorted), end(sorted));
    return sorted;
}

void Course::setDescription(const std::string &description) {
    m_description = description;
}

void Course::addPrereq(Course *prereq) { m_prereqs.push_back(prereq); }

void Course::addChoice(std::vector<Course *> &choice) {
    m_choices.push_back(choice);
}

void Course::addPathway(std::vector<Course *> &pathway) {
    m_pathways.push_back(pathway);
}

// TODO: Format the printing to be at *most* n characters wide
void Course::printInfo(User &user) {
    std::cout << getName() << std::endl << std::endl;
    std::cout << m_description << std::endl << std::endl;
    if (user.hasTaken(this)) {
        std::cout << "You have already taken this class." << std::endl;
    } else if (user.hasAllPrereqs(this)) {
        std::cout << "You can take this class next quarter!" << std::endl;
    } else {
        if (m_prereqs.size() == 0 && m_choices.size() == 0 &&
            m_pathways.size() == 0) {
            std::cout << "There are no requirements for this class."
                      << std::endl;
        }
        if (m_prereqs.size() != 0) {
            std::cout << "You still need to take all of the following courses:"
                      << std::endl;
            for (Course *prereq : m_prereqs) {
                if (!user.hasTaken(prereq)) {
                    std::cout << "* " << prereq->getName() << std::endl;
                }
            }
        }
        if (m_choices.size() != 0) {
            std::cout << "You must choose at least one class from each of the "
                         "following rows:"
                      << std::endl;
            for (const std::vector<Course *> &choice : m_choices) {
                std::vector<std::string> choiceNames;
                for (Course *course : choice) {
                    choiceNames.emplace_back(course->getName());
                }
                std::cout << "* " << join(choiceNames, ", ") << std::endl;
            }
        }
        if (m_pathways.size() != 0) {
            std::cout << "You must choose all classes from at least one of the "
                         "following rows:"
                      << std::endl;
            for (const std::vector<Course *> &pathway : m_pathways) {
                std::vector<std::string> pathwayNames;
                for (Course *course : pathway) {
                    pathwayNames.emplace_back(course->getName());
                }
                std::cout << "* " << join(pathwayNames, ", ") << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

void Course::printPrereqs() const {
    if (this->getAllPrereqs().size() == 0) {
        std::cout << "There are no requirements to take this class."
                  << std::endl;
    } else {
        std::cout << "You need the following:" << std::endl;
        for (Course *course : this->getAllPrereqs()) {
            std::cout << "* " << course->getName() << std::endl;
        }
    }
}
