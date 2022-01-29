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
    for (const Course *p : m_prereqs) {
        delete (p);
    }
}

std::string Course::getName() const { return m_department + ' ' + m_ID; }

std::string Course::getDescription() const { return m_description; }

std::vector<const Course *> Course::getPrereqs() const { return m_prereqs; }

std::vector<std::vector<const Course *>> Course::getChoices() const {
    return m_choices;
}

std::vector<std::vector<const Course *>> Course::getPathways() const {
    return m_pathways;
}

// TODO: change output to a tuple with mandatory, choices, pathways as vectors
std::vector<const Course *> Course::getAllPrereqs() const {
    std::unordered_set<const Course *> processed;
    auto dfs = [&processed](const Course *&cur, auto &&dfs) -> void {
        processed.insert(cur);
        for (const Course *&prereq : cur->getPrereqs()) {
            if (processed.find(prereq) == processed.end()) {
                dfs(prereq, dfs);
            }
        }
    };
    for (const Course *prereq : m_prereqs) {
        dfs(prereq, dfs);
    }
    std::vector<const Course *> sorted(processed.begin(), processed.end());
    sort(begin(sorted), end(sorted));
    return sorted;
}

void Course::setDescription(const std::string &description) {
    m_description = description;
}

void Course::addPrereq(const Course *prereq) { m_prereqs.push_back(prereq); }

void Course::addChoice(std::vector<const Course *> &choice) {
    m_choices.push_back(choice);
}

void Course::addPathway(std::vector<const Course *> &pathway) {
    m_pathways.push_back(pathway);
}

// TODO: Format the printing to be at *most* n characters wide
void Course::printInfo(const User &user) const {
    std::cout << getName() << std::endl << std::endl;
    std::cout << m_description << std::endl << std::endl;
    // TODO: Figure out how to use the this pointer without defining a new
    // pointer
    const Course *tmp = this;
    if (user.hasTaken(tmp)) {
        std::cout << "You have already taken this class." << std::endl;
    } else if (user.hasAllPrereqs(tmp)) {
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
            for (const Course *prereq : m_prereqs) {
                if (!user.hasTaken(prereq)) {
                    std::cout << "* " << prereq->getName() << std::endl;
                }
            }
        }
        if (m_choices.size() != 0) {
            std::cout << "You must choose at least one class from each of the "
                         "following rows:"
                      << std::endl;
            for (const std::vector<const Course *> &choice : m_choices) {
                std::vector<std::string> choiceNames;
                for (const Course *course : choice) {
                    choiceNames.emplace_back(course->getName());
                }
                std::cout << "* " << join(choiceNames, ", ") << std::endl;
            }
        }
        if (m_pathways.size() != 0) {
            std::cout << "You must choose all classes from at least one of the "
                         "following rows:"
                      << std::endl;
            for (const std::vector<const Course *> &pathway : m_pathways) {
                std::vector<std::string> pathwayNames;
                for (const Course *course : pathway) {
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
        for (const Course *course : this->getAllPrereqs()) {
            std::cout << "* " << course->getName() << std::endl;
        }
    }
}
