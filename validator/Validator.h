#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "../utils/SchoolUtils.h"
#include "../SchoolConstants.h"
#include <string>

// ─────────────────────────────────────────────────────────────────────────
//  Validator classes — input validation ONLY
//
//  Each validator answers one question: "is this input value acceptable?"
//  Validators do NOT:
//    - query repositories
//    - enforce business limits (max courses, capacities)
//    - check for duplicates
// ─────────────────────────────────────────────────────────────────────────

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
