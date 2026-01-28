#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../service/Service.h"

class TeacherController {
private:
    TeacherServiceImpl &teacherService;

public:
    TeacherController(TeacherServiceImpl &service);
    string addTeacher(int grade, Teacher &teacher);
};

class CourseController {
private:
    CourseServiceImpl &courseService;

public:
    CourseController(CourseServiceImpl &service);
    string addCourse(int grade, Course &course);
};

class StudentController {
private:
    StudentServiceImpl &studentService;

public:
    StudentController(StudentServiceImpl &service);
    string addStudent(int grade, Student &student);
};

#endif
