#ifndef USER_H_
#define USER_H_

#include <bits/stdc++.h>
class Course;

class User {
   public:
    User();
    User(const std::unordered_map<std::string, Course*>& courseMap,
         const std::string& file);
    std::set<Course*> getTakenCourses() const;
    void addCourses(const std::unordered_map<std::string, Course*>& courseMap,
                    const std::vector<std::string>& courseNames);
    void removeCourses(
        const std::unordered_map<std::string, Course*>& courseMap,
        const std::vector<std::string>& courseNames);
    void printTakenCourses() const;
    bool hasTaken(Course* course);
    // bool hasAllPrereqs(Course* course);
    std::tuple<std::vector<Course*>, std::vector<std::vector<Course*>>,
               std::vector<std::vector<std::vector<Course*>>>>
    getRemainingPrereqs(Course* course);

   private:
    std::set<Course*> m_takenCourses;
    const std::string m_file;
};

#endif
