#include "../model/Models.cpp"
#include <bits/stdc++.h>
using namespace std;

////////////////// Teacher \\\\\\\\\\\\\\\\\\
// interface TeacherRepository
class TeacherRepository {








};
// class TeacherRepository implementation
class TeacherRepositoryImpl : public TeacherRepository {










};

////////////////// Course \\\\\\\\\\\\\\\\\\
// interface CourseRepository
class CourseRepository {

  public :

      virtual void addCourse(Course &course) = 0;

};

// class CourseRepository implementation
class CourseRepositoryImpl : public CourseRepository {

  int MaxNumberOfCoursesPerGrade = 10;
  vector <Course> courses;

  public :

    virtual void addCourse(Course &course){
       if (students.size() >= maxStudentsPerGrade){
         cout <<"Registration full. Cannot add more students." << endl;
       }
       else {
         students.push_back(student);
          cout << "Added successfully." << endl;
       }
     }









};

////////////////// Student \\\\\\\\\\\\\\\\\\
// interface StudentRepository
class StudentRepository {


  public :

      virtual bool addStudent(int grade, Student &student) = 0;
};

// class StudentRepository implementation
class StudentRepositoryImpl : public StudentRepository {

  const int maxStudentsPerGrade = 120 ;
  const int firstGrade = 1;
  const int lastGrade = 12;
  map <int,vector<Student>> studentsByGrade;

  public :

    bool addStudent(int grade, Student &student) override {

       if (grade < firstGrade || grade > lastGrade || studentsByGrade[grade].size() >= maxStudentsPerGrade ){
        return false;
       }

       else {
         studentsByGrade[grade].push_back(student);
          return true;
       }
     }

};
