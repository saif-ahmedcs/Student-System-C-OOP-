#include "Validator.h"
using namespace std;

// ─────────────────────────────────────────────
//  TeacherValidator
// ─────────────────────────────────────────────
bool TeacherValidator::validateName(const string& name) const {
    return !name.empty();
}

bool TeacherValidator::validateAge(int age) const {
    return age >= SchoolConstants::MIN_TEACHER_AGE && age <= SchoolConstants::MAX_TEACHER_AGE;
}

bool TeacherValidator::validateExperienceYears(int years) const {
    return years >= SchoolConstants::MIN_EXPERIENCE_YEARS;
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
