#include <iostream>
#include <vector>

using namespace std;

class BaseEntity {

 protected :
    string name;
    int age;
    string id;
    string phone_number;

 public :
    // Setters
    void setName(string name){
     this->name = name;
    }

    void setAge(int age){
     this->age = age;
    }

    void setId(string id){
     this->id = id;
    }

    void setPhoneNumber(string phone_number){
     this->phone_number = phone_number;
    }

    // Getters
    string GetName(){
     return name;
    }

    int GetAge(){
     return age;
    }

    string GetId(){
     return id;
    }

    string GetPhoneNumber(){
     return phone_number;
    }


};

class Teacher : public BaseEntity { // Teacher inherits from BaseEntity to reuse common attributes
  private :
      double Monthly_salary;
      string Teacher_subject;
      int numberOfStudents;
      vector <int> StudentsIds;

  public :
    // Setters
    void setMonthlySalary(double Monthly_salary){
     this->Monthly_salary = Monthly_salary;
    }

    void setTeacherSubject(string Teacher_subject){
     this->Teacher_subject = Teacher_subject;
    }


    void setStudentsIds(const vector<int>& ids) {
     numberOfStudents = ids.size();
     StudentsIds = ids;
}


    // Getters
    double GetMonthlySalary(){
     return Monthly_salary;
    }

    string GetTeacherSubject(){
     return Teacher_subject;
    }

    int GetNumberOfStudents() const {
     return numberOfStudents;
}

    vector<int> GetStudentsIds() const {
     return StudentsIds;
}


};

class Course {

  private:
      string subject_name;
      Teacher teacher;
      string AcadimecYear;
      int subject_hours;
      vector <Teacher> CourseTeachers;


  public :
     // Setters
    void setCourseSubjectName(const string &name){
     subject_name = name;
    }

    void setCourseTeacher(const Teacher &t){
     teacher = t;
     CourseTeachers.push_back(teacher);
    }

    void setAcademicYear(const string &AcadimecYear){
     this->AcadimecYear = AcadimecYear;
    }

    void setSubjectHours(int subject_hours){
     this->subject_hours = subject_hours;
    }

    // Getters
    string GetSubjectName() const {
      return subject_name;
    }

    Teacher GetCourseTeacher() const {
      return teacher;
    }

    int GetSubjectHours() const {
      return subject_hours;
    }

    const vector<Teacher> &GetCourseTeachers() const {
     return CourseTeachers;
    }

    int GetNumberOfCourseTeachers() const {
     return CourseTeachers.size();
    }

};

class Student : public BaseEntity { // Student inherits from BaseEntity to reuse common attributes
  private :
      string school_year;
      double gpa;
      vector <Teacher> Student_Teachers;


  public :
     // Setters
    void setSchoolYear(string school_year){
     this->school_year = school_year;
    }

    void setGpa(double gpa){
     this->gpa = gpa;
    }

    void SetTeachers(const vector <Teacher> &Teachers){
     this->Student_Teachers = Teachers;
    }

    // Getters
    string GetSchoolYear(){
     return school_year;
    }

    double GetGpa(){
     return gpa;
    }

    const vector<Teacher> &GetStudentTeachers() const {
     return Student_Teachers;
}

    int GetNumberOfStudentTeachers(){
     return Student_Teachers.size();
    }
};

























