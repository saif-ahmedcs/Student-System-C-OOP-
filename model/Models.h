#ifndef MODELS_H
#define MODELS_H
#include <bits/stdc++.h>
using namespace std;

class ShareData {
protected:
    string name;
    string id;
public:
    void setName(const string &name);
    void setId(const string &id);
    string getName() const;
    string getId() const;
};

class BaseEntity : public ShareData {
protected:
    int age;
    string phoneNumber;
public:
    void setAge(int age);
    void setPhoneNumber(const string &phone);
    int getAge() const;
    string getPhoneNumber() const;
};

class Teacher : public BaseEntity {
private:
    double monthlySalary;
    string teacherSubject;
    vector<int> studentIds;
public:
    void setMonthlySalary(double salary);
    void setTeacherSubject(const string &subject);
    void addStudentId(int id);
    double getMonthlySalary() const;
    string getTeacherSubject() const;
    const vector<int>& getStudentsIds() const;
    int getNumberOfStudentsInTeacherCourse() const;
};

class Course : public ShareData {
private:
    Teacher teacher;
    vector<Teacher> courseTeachers;
    string AcademicYear;
    int subjectHours;
public:
    void setSingleTeacher(const Teacher& t);
    void addTeacher(const Teacher& t);
    void setAcademicYear(const string &year);
    void setSubjectHours(int hours);
    Teacher getCourseTeacher() const;
    const vector<Teacher>& getCourseTeachers() const;
    string getAcademicYear() const;
    int getSubjectHours() const;
    int getNumberOfCourseTeachers() const;
};

struct TeacherCourse {
    Teacher teacher;
    Course course;
};

class Student : public BaseEntity {
private:
    string schoolYear;
    double gpa;
    vector<TeacherCourse> teacherCoursePairs;
public:
    void setSchoolYear(const string &year);
    void setGpa(double g);
    void setStudentTeacherCourse(const Teacher &teacher, const Course &course);
    string getSchoolYear() const;
    double getGpa() const;
    const vector<TeacherCourse>& getStudentTeacherCourses() const;
    int getNumberOfStudentTeachers() const;
    int getNumberOfStudentCourses() const;
};

#endif
