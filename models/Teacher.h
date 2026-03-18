#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include <vector>
#include "Student.h"

class Teacher : public BaseEntity {
private:
    int grade;
    std::string subject;
    double monthlySalary;
    int experienceYears;
    std::string specialization;
    std::vector<std::string> assignedCourseIds;
public:
    void setGrade(int grade);
    void setMonthlySalary(double salary);
    void setSubject(const std::string& subject);
    void setExperienceYears(int years);
    void setSpecialization(const std::string& specialization);

    bool assignCourse(const std::string& courseId);
    bool isCourseAssigned(const std::string& courseId) const;

    bool removeCourse(const std::string& courseId);
    void removeAllCourses();

    int getGrade() const;
    double getMonthlySalary() const;
    std::string getSubject() const;
    int getExperienceYears() const;
    std::string getSpecialization() const;
    const std::vector<std::string>& getAssignedCourses() const;
};

#endif
