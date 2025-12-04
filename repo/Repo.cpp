#include "../model/Models.cpp"
#include <bits/stdc++.h>
using namespace std;
//Global variables
const int firstGrade = 1;
const int lastGrade = 12;
const int MaxStudentsInSchool = 1440;
enum Stage {Primary = 1, Middle, Secondary};

//Global Function
    Stage getStageFromGrade(int grade) {
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

////////////////// Teacher \\\\\\\\\\\\\\\\\\
// interface TeacherRepository
class TeacherRepository {

public :
      virtual bool addTeacher(int grade, Teacher &teacher) = 0;




};
// class TeacherRepository implementation
class TeacherRepositoryImpl : public TeacherRepository {


  const map <Stage,int> maxTeachersPerGradeInStage = {
  {Stage::Primary,7},
  {Stage::Middle, 9},
  {Stage::Secondary,12}
  };

  map <int,vector<Teacher>> teachersInGrade;
  map <Stage,vector<Teacher>> teachersInStage;
  vector <Teacher> teachersInSchool;

  public :


    void addTeacherInGrade(int grade, Teacher &teacher) {
        teachersInGrade[grade].push_back(teacher);
     }


    void addTeacherInStage(int grade, Teacher &teacher) {
        Stage stage = getStageFromGrade(grade);
        teachersInStage[stage].push_back(teacher);
     }

    void addTeacherInSchool(Teacher &teacher) {
        teachersInSchool.push_back(teacher);
     }



    bool addTeacher(int grade, Teacher &teacher) override {
        Stage stage = getStageFromGrade(grade);


        if (grade < firstGrade || grade > lastGrade || teachersInGrade[grade].size() >= maxTeachersPerGradeInStage.at(stage))
            return false;

        addTeacherInGrade(grade, teacher);
        addTeacherInStage(grade, teacher);
        addTeacherInSchool(teacher);

        return true;
     }


};

////////////////// Course \\\\\\\\\\\\\\\\\\
// interface CourseRepository
class CourseRepository {

  public :

      virtual bool addCourse(int grade, Course &course) = 0;

};

// class CourseRepository implementation
class CourseRepositoryImpl : public CourseRepository {

  const map <Stage,int> maxCoursesPerGradeInStage = {
  {Stage::Primary,7},
  {Stage::Middle, 9},
  {Stage::Secondary,12}
  };

  map <int,vector<Course>> coursesInGrade;
  map <Stage,vector<Course>> coursesInStage;
  vector <Course> coursesInSchool;

  public :


    void addCourseInGrade(int grade, Course &course) {
        coursesInGrade[grade].push_back(course);
     }


    void addCourseInStage(int grade, Course &course) {
        Stage stage = getStageFromGrade(grade);
        coursesInStage[stage].push_back(course);
     }

    void addCourseInSchool(Course &course) {
        coursesInSchool.push_back(course);
     }



    bool addCourse(int grade, Course &course) override {
        Stage stage = getStageFromGrade(grade);


        if (grade < firstGrade || grade > lastGrade || coursesInGrade[grade].size() >= maxCoursesPerGradeInStage.at(stage))
            return false;

        addCourseInGrade(grade, course);
        addCourseInStage(grade, course);
        addCourseInSchool(course);

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

  map <int,vector<Student>> studentsInGrade;
  map <Stage,vector<Student>> studentsInStage;
  vector <Student>studentsInSchool;

  public :


   void addStudentInGrade(int grade, Student &student) {
        studentsInGrade[grade].push_back(student);
    }

   void addStudentInStage(int grade, Student &student) {
        Stage stage = getStageFromGrade(grade);
        studentsInStage[stage].push_back(student);
    }


   void addStudentInSchool(Student &student) {
        studentsInSchool.push_back(student);
    }


   bool addStudent(int grade, Student &student) override {
        if (grade < firstGrade || grade > lastGrade || studentsInGrade[grade].size() >= maxStudentsPerGrade) {
            return false;
        }

        addStudentInGrade(grade, student);
        addStudentInStage(grade, student);
        addStudentInSchool(student);

        return true;
    }









};
