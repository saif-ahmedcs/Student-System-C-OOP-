#ifndef STUDENT_VALIDATOR_H
#define STUDENT_VALIDATOR_H

#include "../BaseValidator.h"
#include <string>

class StudentValidator : public BaseValidator {
public:
    bool validateAge(int age, int grade) const;
    bool validatePhoneNumber(const std::string& phone) const;
    bool validateGpa(float gpa) const;
};

#endif
