#ifndef VALIDATORS_H
#define VALIDATORS_H

#include "SchoolUtils.h"
#include "SchoolConstants.h"
#include <string>

class TeacherValidator {
public:
    bool validateName(const std::string& name) const;
    bool validateNationalNumber(const std::string& nationalNumber) const;
    bool validateAge(int age) const;
    bool validateExperienceYears(int years) const;
    bool validateSpecialization(const std::string& specialization) const;
    bool validateMonthlySalary(double salary) const;
    bool validateGrade(int grade) const;
};

class CourseValidator {
public:
    bool validateCourseName(const std::string& name) const;
    bool validateSpecialization(const std::string& specialization) const;
    bool validateSubjectHours(int hours) const;
    bool validateGrade(int grade) const;
};

class StudentValidator {
public:
    bool validateName(const std::string& name) const;
    bool validateNationalNumber(const std::string& nationalNumber) const;
    bool validateAge(int age, int grade) const;
    bool validatePhoneNumber(const std::string& phone) const;
    bool validateGrade(int grade) const;
    bool validateGpa(float gpa) const;
};

#endif
