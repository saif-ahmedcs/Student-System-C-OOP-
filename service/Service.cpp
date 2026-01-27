#include "Service.h"

////////////////// TeacherServiceImpl \\\\\\\\\\\\\\\

TeacherServiceImpl::TeacherServiceImpl(TeacherRepositoryImpl &repo) : teacherRepository(repo) {}

bool TeacherServiceImpl::addTeacher(int grade, Teacher &teacher) {
    return teacherRepository.addTeacher(grade, teacher);
}

////////////////// CourseServiceImpl \\\\\\\\\\\\\\\

CourseServiceImpl::CourseServiceImpl(CourseRepositoryImpl &repo) : courseRepository(repo) {}

bool CourseServiceImpl::addCourse(int grade, Course &course) {
    return courseRepository.addCourse(grade, course);
}

////////////////// StudentServiceImpl \\\\\\\\\\\\\\\

StudentServiceImpl::StudentServiceImpl(StudentRepositoryImpl &repo) : studentRepository(repo) {}

bool StudentServiceImpl::validateName(const string &name) {
    return !name.empty();
}

bool StudentServiceImpl::validatePhoneNumber(const string &phone) {
    if (phone.length() < 10 || phone.length() > 12)
        return false;

    for (char c : phone) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool StudentServiceImpl::validateGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

string StudentServiceImpl::addStudent(int grade, Student &student) {

    string errors = "";

    if (!validateName(student.getName()))
        errors += "- Name cannot be empty.\n";

    if (!validatePhoneNumber(student.getPhoneNumber()))
        errors += "- Phone number must be 10 --> 12 digits and contain digits only.\n";

    if (!validateGrade(grade))
        errors += "- Grade must be between 1 and 12.\n";

    // If any validation errors exist
    if (!errors.empty())
        return "Invalid student data. Please review the following errors:\n" + errors;

    // All validations passed
    return studentRepository.addStudent(grade, student);
}
