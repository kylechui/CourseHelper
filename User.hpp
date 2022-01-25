#ifndef USER_H_
#define USER_H_

#include <bits/stdc++.h>
class Course;

class User {
   public:
    User(const std::unordered_map<std::string, Course*>& courseMap,
         const std::string& file);
    std::set<const Course*> getTakenCourses() const;
    void addCourses(const std::unordered_map<std::string, Course*>& courseMap,
                    const std::vector<std::string>& courseNames);
    void removeCourses(
        const std::unordered_map<std::string, Course*>& courseMap,
        const std::vector<std::string>& courseNames);
    void printTakenCourses() const;
    bool hasTaken(const Course*& course) const;
    bool hasAllPrereqs(const Course*& course) const;

   private:
    std::set<const Course*> m_takenCourses;
    const std::string m_file;
};

#endif
