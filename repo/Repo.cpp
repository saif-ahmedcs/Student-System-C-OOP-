#include "../model/Models.cpp"
#include <bits/stdc++.h>
using namespace std;

const int firstGrade = 1;
const int lastGrade = 12;
enum Stage {Primary = 1, Middle, Secondary};

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

      virtual bool addCourse(Stage stage, Course &course) = 0;

};

// class CourseRepository implementation
class CourseRepositoryImpl : public CourseRepository {


  const map <Stage,int> maxCoursesPerStage = {
  {Stage::Primary,7},
  {Stage::Middle, 9},
  {Stage::Secondary,12}
  };

  map <Stage,vector<Course>> coursesByGrade;

  public :

     bool addCourse(Stage stage, Course &course) override {

       if(coursesByGrade[stage].size() >= maxCoursesPerStage.at(stage)){
         return false;
       }

         coursesByGrade[stage].push_back(course);
         return true;

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
