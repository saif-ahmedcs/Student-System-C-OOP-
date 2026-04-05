#ifndef COURSE_VALIDATOR_H
#define COURSE_VALIDATOR_H

#include "../../common/SchoolUtils.h"
#include "../../common/SchoolConstants.h"
#include <string>

class CourseValidator {
public:
    bool validateCourseName(const std::string& name) const;
    bool validateSpecialization(const std::string& specialization) const;
    bool validateSubjectHours(int hours) const;
    bool validateGrade(int grade) const;
};

#endif
