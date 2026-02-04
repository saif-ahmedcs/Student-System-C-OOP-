#include "Models.h"

// ShareData
void ShareData::setName(const string &n){
     name = n;
    }
void ShareData::setId(const string &i){
    id = i;
    }
string ShareData::getName() const {
    return name;
    }
string ShareData::getId() const {
     return id;
    }

// BaseEntity
void BaseEntity::setAge(int a){
    age = a;
    }
void BaseEntity::setPhoneNumber(const string &p){
     phoneNumber = p;
    }
int BaseEntity::getAge() const {
     return age;
    }
string BaseEntity::getPhoneNumber() const {
     return phoneNumber;
    }

// Teacher
void Teacher::setTeacherGrade(int g){
    grade = g;
}
void Teacher::setMonthlySalary(double s){
     monthlySalary = s;
    }
void Teacher::setTeacherSubject(const string &sub){
     teacherSubject = sub;
    }
void Teacher::setExperienceYears(int e){
  experienceYears = e;
}

void Teacher::addStudentId(int id){
    studentIds.push_back(id);
    }

int Teacher::getTeacherGrade() const {
    return grade;
}
double Teacher::getMonthlySalary() const {
    return monthlySalary;
    }
string Teacher::getTeacherSubject() const {
    return teacherSubject;
    }
int Teacher::getTeacherExperienceYears() const {
    return experienceYears;
}
const vector<int>& Teacher::getStudentsIds() const {
    return studentIds;
    }
int Teacher::getNumberOfStudentsInTeacherCourse() const {
     return studentIds.size();
    }

// Course
void Course::setSingleTeacher(const Teacher& t){
     teacher = t;
    }
void Course::addTeacher(const Teacher &t){
     courseTeachers.push_back(t);
    }
void Course::setGrade(const int &year){
     grade = year;
    }
void Course::setSubjectHours(int hours){
    subjectHours = hours;
    }
void Course::setCourseTeacherId(const string &s){
   teacher.setId(s);
    }
string Course::getCourseTeacherId() const {
  return teacher.getId();
    }

Teacher Course::getCourseTeacher() const {
    return teacher;
    }
const vector<Teacher>& Course::getCourseTeachers() const {
    return courseTeachers;
    }
int Course::getGrade() const {
    return grade;
    }
int Course::getSubjectHours() const {
    return subjectHours;
    }
int Course::getNumberOfCourseTeachers() const {
    return courseTeachers.size();
    }

// Student
void Student::setSchoolYear(const int &year){
     schoolYear = year;
    }
void Student::setGpa(double g){
     gpa = g;
    }
void Student::setStudentTeacherCourse(const Teacher &teacher, const Course &course){
    teacherCoursePairs.push_back({teacher, course});
    }
int Student::getSchoolYear() const {
     return schoolYear;
    }
double Student::getGpa() const {
     return gpa;
    }
const vector<TeacherCourse>& Student::getStudentTeacherCourses() const {
     return teacherCoursePairs;
    }
int Student::getNumberOfStudentTeachers() const {
     return teacherCoursePairs.size();
    }
int Student::getNumberOfStudentCourses() const {
    return teacherCoursePairs.size();
    }
