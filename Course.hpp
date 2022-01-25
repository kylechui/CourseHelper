#ifndef COURSE_H_
#define COURSE_H_

#include <bits/stdc++.h>
class User;

class Course {
   public:
    Course(const std::string& name);
    ~Course();
    std::string getName() const;
    std::string getDescription() const;
    std::vector<const Course*> getPrereqs() const;
    std::vector<std::set<const Course*>> getChoices() const;
    std::vector<std::set<const Course*>> getPathways() const;
    std::vector<const Course*> getAllPrereqs() const;
    void setDescription(const std::string& description);
    void addPrereq(const Course* prereq);
    void addChoice(std::set<const Course*>& choice);
    void addPathway(std::set<const Course*>& pathway);
    void printInfo(const User& user) const;
    void printPrereqs() const;

   private:
    const std::string m_name;
    std::string m_description;
    std::vector<const Course*> m_prereqs;
    std::vector<std::set<const Course*>> m_choices;
    std::vector<std::set<const Course*>> m_pathways;
};

#endif
