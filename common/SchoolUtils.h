#ifndef SCHOOL_UTILS_H
#define SCHOOL_UTILS_H

#include "SchoolConstants.h"

inline bool isValidGrade(int grade) {
    return grade >= SchoolConstants::MIN_GRADE && grade <= SchoolConstants::MAX_GRADE;
}

#endif
