#ifndef COURSE_H_
#define COURSE_H_

#include <bits/stdc++.h>

class Course {
public:
    Course(std::string& ID, std::string& name, std::string& description);
    ~Course();
private:
    std::string m_ID;
    std::string m_name;
    std::string m_description;
    std::vector<Course*> m_prereqs;
};

#endif
