#ifndef TEACHER_VALIDATOR_H
#define TEACHER_VALIDATOR_H

#include "../BaseValidator.h"
#include <string>

class TeacherValidator : public BaseValidator {
public:
    bool validateAge(int age) const;
    bool validateExperienceYears(int years) const;
    bool validateMonthlySalary(double salary) const;
};

#endif
