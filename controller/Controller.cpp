#include "../service/Service.cpp"
#include <bits/stdc++.h>
using namespace std;

////////////////// Teacher \\\\\\\\\\\\\\\\\\

class TeacherController {

private :
    TeacherService &teacherService;

public :
    bool addTeacher(int grade, Teacher &teacher){
      return teacherService.addTeacher(grade,teacher);
    }
};

////////////////// Course \\\\\\\\\\\\\\\\\\

class CourseController {

private :
    CourseService &courseService;

public :
    bool addCourse(int grade, Course &course){
      return courseService.addCourse(grade,course);
    }
};

////////////////// Student \\\\\\\\\\\\\\\\\\

class StudentController {

private :
    StudentService &studentService;

public :

    bool addStudent(int grade, Student &student){
      return studentService.addStudent(grade,student);
     }

};
