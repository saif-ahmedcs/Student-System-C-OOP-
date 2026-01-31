#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../service/Service.h"

// Teacher
class TeacherController {
private:
    TeacherServiceImpl &teacherService;

public:
    TeacherController(TeacherServiceImpl &service);
    string addTeacher(int grade, Teacher &teacher);
};

//Course
class CourseController {
private:
    CourseServiceImpl &courseService;

public:
    CourseController(CourseServiceImpl &service);
    string addCourse(int grade, Course &course);
    string editCourse(const string& id, const Course& newData);

};

//Student
class StudentController {
private:
    StudentServiceImpl &studentService;

public:
    StudentController(StudentServiceImpl &service);
    string addStudent(int grade, Student &student);
    string editStudent(const string& id, const Student& newData);

};

#endif
