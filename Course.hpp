#ifndef COURSE_H_
#define COURSE_H_

#include <bits/stdc++.h>
class User;

class Course {
   public:
    Course(const std::string &filename);
    ~Course();
    std::string getName() const;
    std::string getDescription() const;
    std::vector<Course *> getPrereqs() const;
    std::vector<std::vector<Course *>> getChoices() const;
    std::vector<std::vector<std::vector<Course *>>> getPathways() const;
    std::tuple<std::vector<Course *>, std::vector<std::vector<Course *>>,
               std::vector<std::vector<std::vector<Course *>>>>
    getAllPrereqs();
    void setDescription(const std::string &description);
    void addPrereq(Course *prereq);
    void addChoice(std::vector<Course *> &choice);
    void addPathway(std::vector<std::vector<Course *>> &pathway);
    void printInfo(User &user);
    void printPrereqs();

   private:
    std::string m_department;
    std::string m_ID;
    std::string m_description;
    std::vector<Course *> m_prereqs;
    std::vector<std::vector<Course *>> m_choices;
    std::vector<std::vector<std::vector<Course *>>> m_pathways;
};

#endif
