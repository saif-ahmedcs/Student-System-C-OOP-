#include "Validator.h"
using namespace std;

// ─────────────────────────────────────────────
//  TeacherValidator
// ─────────────────────────────────────────────
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

// ─────────────────────────────────────────────
//  CourseValidator
// ─────────────────────────────────────────────
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

// ─────────────────────────────────────────────
//  StudentValidator
// ─────────────────────────────────────────────
bool StudentValidator::validateName(const string& name) const {
    return !name.empty();
}

bool StudentValidator::validateNationalNumber(const string& nationalNumber) const {
    if ((int)nationalNumber.length() != SchoolConstants::NATIONAL_NUMBER_LENGTH)
        return false;
    for (int i = 0; i < (int)nationalNumber.length(); i++)
        if (!isdigit(nationalNumber[i]))
            return false;
    return true;
}

bool StudentValidator::validateAge(int age, int grade) const {

    int expectedAge = grade + SchoolConstants::STUDENT_AGE_BASE;
    int tolerance = SchoolConstants::STUDENT_AGE_TOLERANCE;

    return age >= (expectedAge - tolerance) && age <= (expectedAge + tolerance);
}

bool StudentValidator::validatePhoneNumber(const string& phone) const {
    if ((int)phone.length() < SchoolConstants::MIN_PHONE_LENGTH || (int)phone.length() > SchoolConstants::MAX_PHONE_LENGTH)
        return false;
    for (int i = 0; i < (int)phone.length(); i++)
        if (!isdigit(phone[i]))
            return false;
    return true;
}

bool StudentValidator::validateGrade(int grade) const {
    return isValidGrade(grade);
}

bool StudentValidator::validateGpa(float gpa) const {
    return gpa >= SchoolConstants::MIN_GPA && gpa <= SchoolConstants::MAX_GPA;
}
