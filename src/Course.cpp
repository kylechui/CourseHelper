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

std::vector<Course *> Course::getRequiredReqs() const { return m_requiredReqs; }

std::vector<std::vector<Course *>> Course::getChoiceReqs() const {
    return m_choiceReqs;
}

std::vector<std::vector<std::vector<Course *>>> Course::getSeriesReqs() const {
    return m_seriesReqs;
}

std::tuple<std::vector<Course *>, std::vector<std::vector<Course *>>,
           std::vector<std::vector<std::vector<Course *>>>>
Course::getAllPrereqs() {
    std::unordered_set<Course *> seen;
    std::vector<std::vector<Course *>> choicesReqs;
    std::vector<std::vector<std::vector<Course *>>> seriesReqs;
    auto dfs = [&seen, &choicesReqs, &seriesReqs](Course *cur,
                                                  auto &&dfs) -> void {
        seen.insert(cur);
        for (std::vector<Course *> &choice : cur->getChoiceReqs()) {
            choicesReqs.push_back(choice);
        }
        for (std::vector<std::vector<Course *>> &seriesReq :
             cur->getSeriesReqs()) {
            seriesReqs.push_back(seriesReq);
        }
        for (Course *prereq : cur->getRequiredReqs()) {
            if (seen.find(prereq) == seen.end()) {
                dfs(prereq, dfs);
            }
        }
    };
    dfs(this, dfs);
    seen.erase(this);
    std::vector<Course *> requiredReqs(seen.begin(), seen.end());
    sort(begin(requiredReqs), end(requiredReqs));
    return {requiredReqs, choicesReqs, seriesReqs};
}

void Course::setDescription(const std::string &description) {
    m_description = description;
}

void Course::addRequiredReq(Course *requiredReq) {
    m_requiredReqs.push_back(requiredReq);
}

void Course::addChoiceReq(std::vector<Course *> &choiceReq) {
    m_choiceReqs.push_back(choiceReq);
}

void Course::addSeriesReq(std::vector<std::vector<Course *>> &seriesReq) {
    m_seriesReqs.push_back(seriesReq);
}

// TODO: Format the printing to be at *most* n characters wide, maybe write own
// function in utils to handle print formatting and line wrapping
void Course::printInfo(User *user) {
    std::cout << getName() << std::endl << std::endl;
    std::cout << m_description << std::endl << std::endl;
    auto [requiredReqs, choiceReqs, seriesReqs] =
        user->getRemainingPrereqs(this);
    if (user->hasTaken(this)) {
        std::cout << "You have already taken this class." << std::endl;
    } else if (requiredReqs.empty() && choiceReqs.empty() &&
               seriesReqs.empty()) {
        std::cout << "You can take this class next quarter!" << std::endl;
    } else {
        if (!requiredReqs.empty()) {
            std::cout << "You still need to take all of the following courses:"
                      << std::endl;
            for (Course *prereq : requiredReqs) {
                std::cout << "* " << prereq->getName() << std::endl;
            }
        }
        if (!choiceReqs.empty()) {
            std::cout << "You must choose at least one class from each of the "
                         "following rows:"
                      << std::endl;
            for (const std::vector<Course *> &choice : choiceReqs) {
                std::cout << "* " << joinNames(choice, ", ") << std::endl;
            }
        }
        if (!seriesReqs.empty()) {
            for (const std::vector<std::vector<Course *>> &seriesReq :
                 seriesReqs) {
                std::cout
                    << "You must choose all classes from at least one of the "
                       "following rows:"
                    << std::endl;
                for (const std::vector<Course *> &series : seriesReq) {
                    std::cout << "* " << joinNames(series, ", ") << std::endl;
                }
            }
        }
    }
    std::cout << std::endl;
}

void Course::printPrereqs() {
    auto [requiredReqs, choiceReqs, seriesReqs] = this->getAllPrereqs();
    if (requiredReqs.empty() && choiceReqs.empty() && seriesReqs.empty()) {
        std::cout << "There are no requirements to take this class."
                  << std::endl;
    } else {
        if (!requiredReqs.empty()) {
            std::cout << "You need the following:" << std::endl;
            for (Course *course : requiredReqs) {
                std::cout << "* " << course->getName() << std::endl;
            }
        }
        if (!choiceReqs.empty()) {
            std::cout << "You need to take at least one class from each of "
                         "the following rows:"
                      << std::endl;
            for (std::vector<Course *> &choice : choiceReqs) {
                std::cout << "* " << joinNames(choice, ", ") << std::endl;
            }
        }
        if (!seriesReqs.empty()) {
            for (std::vector<std::vector<Course *>> &seriesReq : seriesReqs) {
                std::cout
                    << "You need to take all the classes from at least one "
                       "of the following rows:"
                    << std::endl;
                for (std::vector<Course *> &series : seriesReq) {
                    std::cout << "* " << joinNames(series, ", ") << std::endl;
                }
            }
        }
    }
}
