#include "TeacherValidator.h"
using namespace std;

bool TeacherValidator::validateAge(int age) const {
    return age >= SchoolConstants::MIN_TEACHER_AGE && age <= SchoolConstants::MAX_TEACHER_AGE;
}

bool TeacherValidator::validateExperienceYears(int years) const {
    return years >= SchoolConstants::MIN_EXPERIENCE_YEARS;
}

bool TeacherValidator::validateMonthlySalary(double salary) const {
    return salary >= SchoolConstants::MIN_MONTHLY_SALARY;
}
