#ifndef COURSE_H_
#define COURSE_H_

#include <bits/stdc++.h>

class Course {
public:
    Course(const std::string& name, const std::string& description);
    ~Course();
    std::string getName();
    std::string getDescription();
    void setDescription(const std::string& description);
    std::vector<Course*> getPrereqs();
    std::vector<std::set<Course*>> getChoices();
    void addPrereq(Course* prereq);
    void addChoice(std::set<Course*>& choice);
private:
    const std::string m_name;
    std::string m_description;
    std::vector<Course*> m_prereqs;
    std::vector<std::set<Course*>> m_choices;
};

#endif
