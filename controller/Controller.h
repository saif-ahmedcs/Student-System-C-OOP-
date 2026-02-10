#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../service/Service.h"

// Teacher
class TeacherController {
private:
    TeacherServiceImpl &teacherService;

public:
    TeacherController(TeacherServiceImpl &service);

    Teacher *findTeacherById(const string& id);
    string addTeacher(int grade, Teacher &teacher);
    string editTeacher(const string& id, const Teacher& newData);
    string assignCoursesToTeacher(const string& teacherId,const vector<string>& courseIds);

    void showTeacher(const string& id);

};

//Course
class CourseController {
private:
    CourseServiceImpl &courseService;

public:
    CourseController(CourseServiceImpl &service);
    Course *findCourseById(const string& id);
    string addCourse(int grade, Course &course);
    string editCourse(const string& id, const Course& newData);
    void showCourse(const string& id);


};

//Student
class StudentController {
private:
    StudentServiceImpl &studentService;

public:
    StudentController(StudentServiceImpl &service);
    Student *findStudentById(const string& id);
    string addStudent(int grade, Student &student);
    string editStudent(const string& id, const Student& newData);
    void showStudent(const string& id);

};

#endif
