#include "Controller.h"

// Teacher
TeacherController::TeacherController(TeacherServiceImpl &service) : teacherService(service) {}

string TeacherController::addTeacher(int grade, Teacher &teacher) {
    return teacherService.addTeacher(grade, teacher);
}

// Course
CourseController::CourseController(CourseServiceImpl &service) : courseService(service) {}

string CourseController::addCourse(int grade, Course &course) {
    return courseService.addCourse(grade, course);
}

string CourseController::editCourse(const string& id, const Course& newData) {
    return courseService.editCourse(id, newData);
}

// Student
StudentController::StudentController(StudentServiceImpl &service) : studentService(service) {}

string StudentController::addStudent(int grade, Student &student) {
    return studentService.addStudent(grade, student);
}

string StudentController::editStudent(const string& id, const Student& newData) {
    return studentService.editStudent(id, newData);
}
