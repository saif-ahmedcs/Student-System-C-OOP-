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

void Teacher::setTeacherNationalNum(string n){
 nationalNumber = n;
}
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
void Teacher::setTeacherSpecialization(const string& specialization){
    teacherSpecialization = specialization;
}
void Teacher::addAssignedCourse(const string& courseId) {
    assignedCourseIds.push_back(courseId);
}
bool Teacher::isCourseAssigned(const string& courseId) const {
    for (int i = 0; i < assignedCourseIds.size(); i++) {
        if (assignedCourseIds[i] == courseId) {
            return true;
        }
    }
    return false;
}

void Teacher::clearAssignedCourses() {
    assignedCourseIds.clear();
}

void Teacher::addStudentId(int id){
    studentIds.push_back(id);
    }


string Teacher::getTeacherNationalNum() const {
  return nationalNumber;
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
string Teacher::getTeacherSpecialization() const {
    return teacherSpecialization;
}
const vector<string>& Teacher::getAssignedCourses() const {
    return assignedCourseIds;
}
const vector<int>& Teacher::getStudentsIds() const {
    return studentIds;
    }
int Teacher::getNumberOfStudentsInTeacherCourse() const {
     return studentIds.size();
    }

// Course
void Course::addTeacherName(const string& teacherName) {
    if(std::find(teacherNames.begin(), teacherNames.end(), teacherName) == teacherNames.end()) {
        teacherNames.push_back(teacherName);
    }
}

void Course::addTeacher(Teacher* t){
    courseTeachers.push_back(t);
    addTeacherName(t->getName());
}
void Course::setGrade(const int &year){
     grade = year;
    }
void Course::setSubjectHours(int hours){
    subjectHours = hours;
    }
void Course::setCourseTeacherName(const string &s){
    teacherName=s;
    }
void Course::setCourseTeacherId(const string& id){
    teacherId = id;
    }
void Course::setCourseSpecialization(const string& specialization){
    courseSpecialization = specialization;
    }



   const vector<string>& Course::getTeacherNames() const {
    return teacherNames;
}









string Course::getCourseTeacherName() const {
  return teacherName;
    }
string Course::getCourseTeacherId() const {
  return teacherId;
}

const vector<Teacher*>& Course::getCourseTeachers() const {
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
string Course::getCourseSpecialization() const {
    return courseSpecialization;
}


// Student
void Student::setStudentNationalNum(string n){
 nationalNumber = n;
}
void Student::setSchoolYear(const int &year){
     schoolYear = year;
    }
void Student::setGpa(double g){
     gpa = g;
    }
void Student::setStudentTeacherCourse(const Teacher &teacher, const Course &course){
    teacherCoursePairs.push_back({teacher, course});
    }

string Student::getStudentNationalNum() const {
  return nationalNumber;
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
