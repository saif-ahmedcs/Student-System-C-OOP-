#include "CourseValidator.h"
using namespace std;

bool CourseValidator::validateCourseName(const string& name) const {
    return ::validateName(name);
}

bool CourseValidator::validateSubjectHours(int hours) const {
    return hours >= SchoolConstants::MIN_SUBJECT_HOURS && hours <= SchoolConstants::MAX_SUBJECT_HOURS;
}
