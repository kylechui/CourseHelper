#include <bits/stdc++.h>
#include "Course.hpp"

Course::Course(std::string& ID, std::string& name, std::string& description) {
    m_name = name;
    m_ID = ID;
    m_description = description;
}

Course::~Course() {
    for (Course* p : m_prereqs) {
        delete(p);
    }
}
