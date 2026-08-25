#ifndef COURSE_VALIDATOR_H
#define COURSE_VALIDATOR_H

#include "../BaseValidator.h"
#include <string>

class CourseValidator : public BaseValidator {
public:
    bool validateCourseName(const std::string& name) const;
    bool validateSubjectHours(int hours) const;
};

#endif
