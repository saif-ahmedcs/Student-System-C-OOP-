#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../service/Service.h"

class TeacherController {
private:
    TeacherServiceImpl &teacherService;

public:
    TeacherController(TeacherServiceImpl &service);
    bool addTeacher(int grade, Teacher &teacher);
};

class CourseController {
private:
    CourseServiceImpl &courseService;

public:
    CourseController(CourseServiceImpl &service);
    bool addCourse(int grade, Course &course);
};

class StudentController {
private:
    StudentServiceImpl &studentService;

public:
    StudentController(StudentServiceImpl &service);
    bool addStudent(int grade, Student &student);
};

#endif
