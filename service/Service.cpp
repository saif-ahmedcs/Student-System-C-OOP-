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

bool StudentServiceImpl::validateGpa(double gpa) {
    return gpa >= 0.0 && gpa <= 4.0;
}

bool StudentServiceImpl::validatePhoneNumber(const string &phone) {
    return phone.length() >= 10 && phone.length() <= 12;
}

bool StudentServiceImpl::validateGrade(int grade) {
    return grade >= 1 && grade <= 12;
}

bool StudentServiceImpl::validateName(const string &name) {
    return !name.empty();
}

string StudentServiceImpl::addStudent(int grade, Student &student) {
    // 1. Validate inputs
    if (!validateName(student.getName()))
        return "Invalid name!";
    if (!validateGpa(student.getGpa()))
        return "Invalid GPA! Must be between 0.0 and 4.0";
    if (!validatePhoneNumber(student.getPhoneNumber()))
        return "Invalid phone number! Must be 12 digits";
    if (!validateGrade(grade))
        return "Invalid grade! Must be between 1 and 12";

    // 2. If everything is valid, add to repository
    return studentRepository.addStudent(grade, student);
}
