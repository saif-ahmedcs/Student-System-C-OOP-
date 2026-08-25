#include "StudentValidator.h"
using namespace std;

bool StudentValidator::validateAge(int age, int grade) const {
    int expectedAge = grade + SchoolConstants::STUDENT_AGE_BASE;
    int tolerance = SchoolConstants::STUDENT_AGE_TOLERANCE;
    return age >= (expectedAge - tolerance) && age <= (expectedAge + tolerance);
}

bool StudentValidator::validatePhoneNumber(const string& phone) const {
    if ((int)phone.length() < SchoolConstants::MIN_PHONE_LENGTH || (int)phone.length() > SchoolConstants::MAX_PHONE_LENGTH) {
        return false;
    }
    for (int i = 0; i < (int)phone.length(); i++) {
        if (!isdigit(phone[i])) {
            return false;
        }
    }
    return true;
}

bool StudentValidator::validateGpa(float gpa) const {
    return gpa >= SchoolConstants::MIN_GPA && gpa <= SchoolConstants::MAX_GPA;
}
