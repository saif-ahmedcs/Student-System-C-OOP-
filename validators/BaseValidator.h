#ifndef BASE_VALIDATOR_H
#define BASE_VALIDATOR_H

#include "../common/ValidatorUtils.h"
#include "../common/SchoolConstants.h"
#include <string>

class BaseValidator {
public:
    bool validateName(const std::string& name) const {
        return ::validateName(name);
    }

    bool validateNationalNumber(const std::string& nationalNumber) const {
        return ::validateNationalNumber(nationalNumber);
    }

    bool validateGrade(int grade) const {
        return ::validateGrade(grade);
    }

    bool validateSpecialization(const std::string& specialization) const {
        return ::validateSpecialization(specialization);
    }

    virtual ~BaseValidator() = default;
};

#endif
