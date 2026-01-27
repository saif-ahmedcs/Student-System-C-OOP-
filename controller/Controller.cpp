#include "Controller.h"

TeacherController::TeacherController(TeacherServiceImpl &service) : teacherService(service) {}

bool TeacherController::addTeacher(int grade, Teacher &teacher) {
    return teacherService.addTeacher(grade, teacher);
}

CourseController::CourseController(CourseServiceImpl &service) : courseService(service) {}

string CourseController::addCourse(int grade, Course &course) {
    return courseService.addCourse(grade, course);
}

StudentController::StudentController(StudentServiceImpl &service) : studentService(service) {}

string StudentController::addStudent(int grade, Student &student) {
    return studentService.addStudent(grade, student);
}
