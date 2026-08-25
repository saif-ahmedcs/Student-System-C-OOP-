#ifndef VALIDATOR_UTILS_H
#define VALIDATOR_UTILS_H

#include "SchoolConstants.h"
#include "SchoolUtils.h"
#include <string>
#include <cctype>

inline bool validateName(const std::string& name) {
    return !name.empty();
}

inline bool validateNationalNumber(const std::string& nationalNumber) {
    if ((int)nationalNumber.length() != SchoolConstants::NATIONAL_NUMBER_LENGTH) {
        return false;
    }
    for (int i = 0; i < (int)nationalNumber.length(); i++) {
        if (!isdigit(nationalNumber[i])) {
            return false;
        }
    }
    return true;
}

inline bool validateGrade(int grade) {
    return isValidGrade(grade);
}

inline bool validateSpecialization(const std::string& specialization) {
    return !specialization.empty();
}

#endif
