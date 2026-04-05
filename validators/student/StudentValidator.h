#ifndef STUDENT_VALIDATOR_H
#define STUDENT_VALIDATOR_H

#include "../../common/SchoolUtils.h"
#include "../../common/SchoolConstants.h"
#include <string>

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
