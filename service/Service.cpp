#include "../repo/Repo.cpp"
#include <bits/stdc++.h>
using namespace std;

class TeacherService {

public :

    virtual bool addTeacher(int grade, Teacher &teacher) = 0;



};

class TeacherServiceImpl {

private :
    TeacherRepository &teacherRepository;

public :

    bool addTeacher(int grade, Teacher &teacher){
      return teacherRepository.addTeacher(grade,teacher);
    }




};


class CourseService {

public :

    virtual bool addCourse(int grade, Course &course) = 0;


};

class CourseServiceImpl {

private :
    CourseRepository &courseRepository;

public :

   bool addCourse(int grade, Course &course){
      return courseRepository.addCourse(grade,course);
     }

};


class StudentService {

public :

 virtual bool addStudent(int grade, Student &student) = 0;




};

class StudentServiceImpl {

private :
    StudentRepository &studentRepository;

public :

   bool addStudent(int grade, Student &student){
      return studentRepository.addStudent(grade,student);
     }





};
