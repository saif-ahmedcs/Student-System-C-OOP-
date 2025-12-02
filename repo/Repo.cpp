#include "../model/Models.cpp"
#include <bits/stdc++.h>
using namespace std;

const int firstGrade = 1;
const int lastGrade = 12;
const int MaxStudentsInSchool = 1440;
enum Stage {Primary = 1, Middle, Secondary};

////////////////// Teacher \\\\\\\\\\\\\\\\\\
// interface TeacherRepository
class TeacherRepository {

      virtual bool addTeacher(Stage stage, Course &course) = 0;







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

  const map <Stage,int> maxCoursesPerGradeInStage = {
  {Stage::Primary,7},
  {Stage::Middle, 9},
  {Stage::Secondary,12}
  };

  map <Stage,vector<Course>> coursesInGrade;
  map <Stage,vector<Course>> coursesInStage;
  vector <Course> coursesInSchool;
  public :

     bool addCourse(Stage stage, Course &course) override {

       if(coursesInGrade[stage].size() >= maxCoursesPerGradeInStage.at(stage)){
         return false;
       }

         coursesInGrade[stage].push_back(course);
         coursesInStage[stage].push_back(course);
         coursesInSchool.push_back(course);
         return true;

     }

};

////////////////// Student \\\\\\\\\\\\\\\\\\
// interface StudentRepository
class StudentRepository {


  public :

      virtual Stage getStageFromGrade(int grade) = 0;
      virtual bool addStudent(int grade, Student &student) = 0;
};

// class StudentRepository implementation
class StudentRepositoryImpl : public StudentRepository {

  const int maxStudentsPerGrade = 120 ;

  map <int,vector<Student>> studentsInGrade;
  map <Stage,vector<Student>> studentsInStage;
  vector <Student>studentsInSchool;

  public :


      Stage getStageFromGrade(int grade) override {
        if (grade >=1 && grade <=6) {
            return Stage::Primary;
          }

        else if (grade >=7 && grade <=9){
           return Stage::Middle;
          }

        else if (grade > 9 && grade <= 12 ){
           return Stage::Secondary; // 10-12
          }

         throw invalid_argument("Invalid grade");

      }

     bool addStudent(int grade, Student &student) override {

       if (grade < firstGrade || grade > lastGrade || studentsInGrade[grade].size() >= maxStudentsPerGrade ){
        return false;
       }


       Stage stage = getStageFromGrade(grade);

         studentsInGrade[grade].push_back(student);
         studentsInStage[stage].push_back(student);
         studentsInSchool.push_back(student);

        return true;

     }

};
