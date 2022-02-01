#ifndef COURSE_H_
#define COURSE_H_

#include <bits/stdc++.h>
class User;

class Course {
   public:
    Course(const std::string &filename);
    // ~Course();
    std::string getID() const;
    std::string getDepartment() const;
    std::string getDescription() const;
    std::string getName() const;
    std::vector<Course *> getRequiredReqs() const;
    std::vector<std::vector<Course *>> getChoiceReqs() const;
    std::vector<std::vector<std::vector<Course *>>> getSeriesReqs() const;
    std::tuple<std::vector<Course *>, std::vector<std::vector<Course *>>,
               std::vector<std::vector<std::vector<Course *>>>>
    getAllPrereqs();
    void setDescription(const std::string &description);
    void addRequiredReq(Course *requiredReq);
    void addChoiceReq(std::vector<Course *> &choiceReq);
    void addSeriesReq(std::vector<std::vector<Course *>> &seriesReq);
    void printInfo(User *user);
    void printPrereqs();

   private:
    std::string m_ID;
    std::string m_department;
    std::string m_description;
    std::vector<Course *> m_requiredReqs;
    std::vector<std::vector<Course *>> m_choiceReqs;
    std::vector<std::vector<std::vector<Course *>>> m_seriesReqs;
};

#endif
