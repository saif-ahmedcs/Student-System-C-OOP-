#include <iostream>
#include <vector>

using namespace std;

class ShareData {

 protected :
    string name;
    string id;

 public :
    //Setters
     void setName(string name){
     this->name = name;
    }

     void setId(string id){
     this->id = id;
    }
    //Getters
    string getName() const {
     return name;
    }

    string getId() const {
     return id;
    }

};


class BaseEntity : public ShareData { // BaseEntity inherits from ShareData to reuse common attributes

 protected :
    int age;
    string phoneNumber;

 public :
    // Setters
    void setAge(int age){
     this->age = age;
    }

    void setPhoneNumber(string phone_number){
     phoneNumber = phone_number;
    }
    // Getters
    int getAge() const {
     return age;
    }

    string getPhoneNumber() const {
     return phoneNumber;
    }


};

class Teacher : public BaseEntity { // Teacher inherits from BaseEntity to reuse common attributes

  private :
      double monthlySalary;
      string teacherSubject;
      vector <int> StudentsIds;

  public :
    // Setters
    void setMonthlySalary(double Monthly_salary){
     monthlySalary = Monthly_salary;
    }

    void setTeacherSubject(string Teacher_subject){
     teacherSubject = Teacher_subject;
    }

    void setStudentsIds(const vector<int>& ids) {
     StudentsIds = ids;
    }

    // Getters
    double getMonthlySalary() const {
     return monthlySalary;
    }

    string getTeacherSubject() const {
     return teacherSubject;
    }

    const vector<int> &getStudentsIds() const {
     return StudentsIds;
    }

    int getNumberOfStudents() const {
     return StudentsIds.size();
    }
};

class Course : public ShareData { // Course inherits from ShareData to reuse common attributes

  private:
      Teacher teacher;
      string AcademicYear;
      int subjectHours;
      vector <Teacher> CourseTeachers;


  public :

    // Setters
    void setSingleTeacher(const Teacher& t){
    teacher = t;
    }

    void addTeacher(const Teacher &t){
     CourseTeachers.push_back(t);
    }

    void setAcademicYear(const string &Academic_Year){
     AcademicYear = Academic_Year;
    }

    void setSubjectHours(int subject_hours){
     subjectHours = subject_hours;
    }

    // Getters
    Teacher getCourseTeacher() const {
      return teacher;
    }

    string getAcademicYear() const {
      return AcademicYear;
    }

    int getSubjectHours() const {
      return subjectHours;
    }

    const vector<Teacher> &getCourseTeachers() const {
     return CourseTeachers;
    }

    int getNumberOfCourseTeachers() const {
     return CourseTeachers.size();
    }

};

class Student : public BaseEntity { // Student inherits from BaseEntity to reuse common attributes

  private :
      string schoolYear;
      double gpa;
      vector <Teacher> Student_Teachers;
      vector <Course>  Student_courses;


  public :
     // Setters
    void setSchoolYear(string school_year){
     schoolYear = school_year;
    }

    void setGpa(double gpa){
     this->gpa = gpa;
    }

    void setStudentTeachers(const vector <Teacher> &Teachers){
     Student_Teachers = Teachers;
    }

    void setStudentCourses(const vector <Course> &Courses){
     Student_courses = Courses;
    }

    // Getters
    string getSchoolYear() const {
     return schoolYear;
    }

    double getGpa() const {
     return gpa;
    }

    const vector<Teacher> &getStudentTeachers() const {
     return Student_Teachers;
    }

    int getNumberOfStudentTeachers() const {
     return Student_Teachers.size();
    }

     const vector<Course> &getStudentCourses() const {
     return Student_courses;
    }

     int getNumberOfStudentCourses() const {
     return Student_courses.size();
    }


};

























