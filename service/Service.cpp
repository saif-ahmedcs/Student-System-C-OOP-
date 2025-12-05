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

string StudentServiceImpl::addStudent(int grade, Student &student) {
    return studentRepository.addStudent(grade, student);
}
