#ifndef COURSE_H_
#define COURSE_H_

#include <bits/stdc++.h>

class Course {
public:
    Course(const std::string& name, const std::string& description);
    ~Course();
    std::string getName();
    std::string getDescription();
    std::vector<Course*> getPrereqs();
    std::vector<std::set<Course*>> getChoices();
    void addPrereq(Course* other);
    void addPrereqs(std::set<Course*>& other);
private:
    const std::string m_name;
    const std::string m_description;
    std::vector<Course*> m_prereqs;
    std::vector<std::set<Course*>> m_choices;
};

#endif
