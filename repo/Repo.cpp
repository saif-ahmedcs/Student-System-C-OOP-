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










};

// class CourseRepository implementation
class CourseRepositoryImpl : public CourseRepository {










};

////////////////// Student \\\\\\\\\\\\\\\\\\
// interface StudentRepository
class StudentRepository {


  public :

      virtual void addStudent(Student &student) = 0;
};

// class StudentRepository implementation
class StudentRepositoryImpl : public StudentRepository {

  int maxStudentsPerGrade = 320 ;
  vector <Student> students;

  public :

    void addStudent (Student &student){
       if (students.size() >= maxStudentsPerGrade){
         cout <<"Registration full. Cannot add more students." << endl;
       }
       else {
         students.push_back(student);
          cout << "Added successfully." << endl;
       }
     }

};
