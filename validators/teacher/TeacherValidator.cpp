#include "TeacherValidator.h"
using namespace std;

bool TeacherValidator::validateName(const string& name) const {
    return !name.empty();
}

bool TeacherValidator::validateNationalNumber(const string& nationalNumber) const {
    if ((int)nationalNumber.length() != SchoolConstants::NATIONAL_NUMBER_LENGTH)
        return false;
    for (int i = 0; i < (int)nationalNumber.length(); i++)
        if (!isdigit(nationalNumber[i]))
            return false;
    return true;
}

bool TeacherValidator::validateAge(int age) const {
    return age >= SchoolConstants::MIN_TEACHER_AGE && age <= SchoolConstants::MAX_TEACHER_AGE;
}

bool TeacherValidator::validateExperienceYears(int years) const {
    return years >= SchoolConstants::MIN_EXPERIENCE_YEARS;
}

bool TeacherValidator::validateSpecialization(const string& specialization) const {
    return !specialization.empty();
}

bool TeacherValidator::validateMonthlySalary(double salary) const {
    return salary >= SchoolConstants::MIN_MONTHLY_SALARY;
}

bool TeacherValidator::validateGrade(int grade) const {
    return isValidGrade(grade);
}
