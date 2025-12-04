#include "../repo/Repo.cpp"
#include <bits/stdc++.h>
using namespace std;

////////////////// Teacher \\\\\\\\\\\\\\\\\\
// interface TeacherService
class TeacherService {

public :

    virtual bool addTeacher(int grade, Teacher &teacher) = 0;



};

// class TeacherService implementation
class TeacherServiceImpl : public TeacherService {

private :
    TeacherRepository &teacherRepository;

public :

    bool addTeacher(int grade, Teacher &teacher){
      return teacherRepository.addTeacher(grade,teacher);
    }




};

////////////////// Course \\\\\\\\\\\\\\\\\\
// interface CourseService
class CourseService {

public :

    virtual bool addCourse(int grade, Course &course) = 0;


};

// class CourseService implementation
class CourseServiceImpl : public CourseService{

private :
    CourseRepository &courseRepository;

public :

   bool addCourse(int grade, Course &course){
      return courseRepository.addCourse(grade,course);
     }

};

////////////////// Student \\\\\\\\\\\\\\\\\\
// interface StudentService
class StudentService {

public :

 virtual bool addStudent(int grade, Student &student) = 0;




};
// class StudentService implementation
class StudentServiceImpl : public StudentService{

private :
    StudentRepository &studentRepository;

public :

   bool addStudent(int grade, Student &student){
      return studentRepository.addStudent(grade,student);
     }


};
