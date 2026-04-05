#include "CourseValidator.h"
using namespace std;

bool CourseValidator::validateCourseName(const string& name) const {
    return !name.empty();
}

bool CourseValidator::validateSpecialization(const string& specialization) const {
    return !specialization.empty();
}

bool CourseValidator::validateSubjectHours(int hours) const {
    return hours >= SchoolConstants::MIN_SUBJECT_HOURS && hours <= SchoolConstants::MAX_SUBJECT_HOURS;
}

bool CourseValidator::validateGrade(int grade) const {
    return isValidGrade(grade);
}
