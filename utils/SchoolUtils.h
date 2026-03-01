#ifndef SCHOOL_UTILS_H
#define SCHOOL_UTILS_H

#include "../SchoolConstants.h"

// ─────────────────────────────────────────────────────────────────────────
//  isValidGrade
//
//  Shared predicate used by all three Validators.
//  Defined once here so the identical check is never duplicated.
// ─────────────────────────────────────────────────────────────────────────
inline bool isValidGrade(int grade) {
    return grade >= SchoolConstants::MIN_GRADE && grade <= SchoolConstants::MAX_GRADE;
}
#endif
