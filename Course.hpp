#ifndef COURSE_H_
#define COURSE_H_

#include <bits/stdc++.h>
class User;

class Course {
public:
    Course(const std::string& name);
    ~Course();
    std::string getName();
    std::string getDescription();
    std::vector<Course*> getPrereqs();
    std::vector<std::set<Course*>> getChoices();
    std::vector<std::set<Course*>> getPathways();
    std::vector<Course*> getAllPrereqs();
    void setDescription(const std::string& description);
    void addPrereq(Course* prereq);
    void addChoice(std::set<Course*>& choice);
    void addPathway(std::set<Course*>& pathway);
    void printInfo(User& user);
private:
    const std::string m_name;
    std::string m_description;
    std::vector<Course*> m_prereqs;
    std::vector<std::set<Course*>> m_choices;
    std::vector<std::set<Course*>> m_pathways;
};

#endif
