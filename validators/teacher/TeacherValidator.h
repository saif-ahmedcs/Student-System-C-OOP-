#ifndef TEACHER_VALIDATOR_H
#define TEACHER_VALIDATOR_H

#include "../../common/SchoolUtils.h"
#include "../../common/SchoolConstants.h"
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

#endif
