#ifndef SCHOOL_UTILS_H
#define SCHOOL_UTILS_H

#include "SchoolConstants.h"
#include <stdexcept>
#include <string>

enum class Stage { Primary, Middle, Secondary };

inline Stage getStageFromGrade(int grade) {
    if (grade >= 1  && grade <= 6)
        return Stage::Primary;
    if (grade >= 7  && grade <= 9)
        return Stage::Middle;
    if (grade >= 10 && grade <= 12)
        return Stage::Secondary;
    throw std::invalid_argument(
        "getStageFromGrade: grade must be 1-12, got " + std::to_string(grade));
}

inline bool isValidGrade(int grade) {
    return grade >= SchoolConstants::MIN_GRADE && grade <= SchoolConstants::MAX_GRADE;
}

inline int getMaxStudentsForStage(Stage stage) {
    switch (stage) {
        case Stage::Primary: return SchoolConstants::MAX_STUDENTS_IN_PRIMARY;
        case Stage::Middle: return SchoolConstants::MAX_STUDENTS_IN_MIDDLE;
        case Stage::Secondary: return SchoolConstants::MAX_STUDENTS_IN_SECONDARY;
    }
    return 0;
}

inline int getMinAvailableSeatsForStage(Stage stage) {
    switch (stage) {
        case Stage::Primary: return SchoolConstants::MIN_AVAILABLE_SEATS_PRIMARY;
        case Stage::Middle: return SchoolConstants::MIN_AVAILABLE_SEATS_MIDDLE;
        case Stage::Secondary: return SchoolConstants::MIN_AVAILABLE_SEATS_SECONDARY;
    }
    return 0;
}

inline int getMaxTeachersForStage(Stage stage) {
    switch (stage) {
        case Stage::Primary: return SchoolConstants::MAX_TEACHERS_IN_PRIMARY;
        case Stage::Middle: return SchoolConstants::MAX_TEACHERS_IN_MIDDLE;
        case Stage::Secondary: return SchoolConstants::MAX_TEACHERS_IN_SECONDARY;
    }
    return 0;
}

inline int getMaxCoursesForStage(Stage stage) {
    switch (stage) {
        case Stage::Primary: return SchoolConstants::MAX_COURSES_IN_PRIMARY;
        case Stage::Middle: return SchoolConstants::MAX_COURSES_IN_MIDDLE;
        case Stage::Secondary: return SchoolConstants::MAX_COURSES_IN_SECONDARY;
    }
    return 0;
}

inline int getMaxStudentsForGrade(int grade) {
    return getMaxStudentsForStage(getStageFromGrade(grade));
}

inline int getMaxTeachersForGrade(int grade) {
    return getMaxTeachersForStage(getStageFromGrade(grade));
}

inline int getMaxCoursesForGrade(int grade) {
    return getMaxCoursesForStage(getStageFromGrade(grade));
}

#endif
