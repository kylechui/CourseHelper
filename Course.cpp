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

std::string Course::getDepartment() const { return m_department; }

std::string Course::getDescription() const { return m_description; }

std::string Course::getName() const { return m_department + ' ' + m_ID; }

std::vector<Course *> Course::getPrereqs() const { return m_prereqs; }

std::vector<std::vector<Course *>> Course::getChoices() const {
    return m_choices;
}

std::vector<std::vector<std::vector<Course *>>> Course::getPathways() const {
    return m_pathways;
}

std::tuple<std::vector<Course *>, std::vector<std::vector<Course *>>,
           std::vector<std::vector<std::vector<Course *>>>>
Course::getAllPrereqs() {
    std::unordered_set<Course *> required;
    std::vector<std::vector<Course *>> choices;
    std::vector<std::vector<std::vector<Course *>>> pathways;
    auto dfs = [&required, &choices, &pathways](Course *cur,
                                                auto &&dfs) -> void {
        required.insert(cur);
        for (std::vector<Course *> &choice : cur->getChoices()) {
            choices.push_back(choice);
        }
        for (std::vector<std::vector<Course *>> &pathway : cur->getPathways()) {
            pathways.push_back(pathway);
        }
        for (Course *prereq : cur->getPrereqs()) {
            if (required.find(prereq) == required.end()) {
                dfs(prereq, dfs);
            }
        }
    };
    dfs(this, dfs);
    required.erase(this);
    std::vector<Course *> sorted(required.begin(), required.end());
    sort(begin(sorted), end(sorted));
    return {sorted, choices, pathways};
}

void Course::setDescription(const std::string &description) {
    m_description = description;
}

void Course::addPrereq(Course *prereq) { m_prereqs.push_back(prereq); }

void Course::addChoice(std::vector<Course *> &choice) {
    m_choices.push_back(choice);
}

void Course::addPathway(std::vector<std::vector<Course *>> &pathway) {
    m_pathways.push_back(pathway);
}

// TODO: Format the printing to be at *most* n characters wide, maybe write own
// function in utils to handle print formatting and line wrapping
void Course::printInfo(User *user) {
    std::cout << getName() << std::endl << std::endl;
    std::cout << m_description << std::endl << std::endl;
    auto [required, choices, allPathways] = user->getRemainingPrereqs(this);
    if (user->hasTaken(this)) {
        std::cout << "You have already taken this class." << std::endl;
    } else if (required.empty() && choices.empty() && allPathways.empty()) {
        std::cout << "You can take this class next quarter!" << std::endl;
    } else {
        if (!required.empty()) {
            std::cout << "You still need to take all of the following courses:"
                      << std::endl;
            for (Course *prereq : required) {
                std::cout << "* " << prereq->getName() << std::endl;
            }
        }
        if (!choices.empty()) {
            std::cout << "You must choose at least one class from each of the "
                         "following rows:"
                      << std::endl;
            for (const std::vector<Course *> &choice : choices) {
                std::cout << "* " << joinNames(choice, ", ") << std::endl;
            }
        }
        if (!allPathways.empty()) {
            for (const std::vector<std::vector<Course *>> &pathways :
                 allPathways) {
                std::cout
                    << "You must choose all classes from at least one of the "
                       "following rows:"
                    << std::endl;
                for (const std::vector<Course *> &pathway : pathways) {
                    std::cout << "* " << joinNames(pathway, ", ") << std::endl;
                }
            }
        }
    }
    std::cout << std::endl;
}

void Course::printPrereqs() {
    auto [required, choices, allPathways] = this->getAllPrereqs();
    if (required.empty() && choices.empty() && allPathways.empty()) {
        std::cout << "There are no requirements to take this class."
                  << std::endl;
    } else {
        if (!required.empty()) {
            std::cout << "You need the following:" << std::endl;
            for (Course *course : required) {
                std::cout << "* " << course->getName() << std::endl;
            }
        }
        if (!choices.empty()) {
            std::cout << "You need to take at least one class from each of "
                         "the following rows:"
                      << std::endl;
            for (std::vector<Course *> &choice : choices) {
                std::cout << "* " << joinNames(choice, ", ") << std::endl;
            }
        }
        if (!allPathways.empty()) {
            for (std::vector<std::vector<Course *>> &pathways : allPathways) {
                std::cout
                    << "You need to take all the classes from at least one "
                       "of the following rows:"
                    << std::endl;
                for (std::vector<Course *> &pathway : pathways) {
                    std::cout << "* " << joinNames(pathway, ", ") << std::endl;
                }
            }
        }
    }
}
