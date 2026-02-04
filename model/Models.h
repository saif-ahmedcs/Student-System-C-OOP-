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

// Teacher
class Teacher : public BaseEntity {
private:
    int grade;
    string teacherSubject;
    double monthlySalary;
    int experienceYears;
    vector<int> studentIds;
public:
    void setTeacherGrade(int grade);
    void setMonthlySalary(double salary);
    void setTeacherSubject(const string &subject);
    void setExperienceYears(int exY);
    void addStudentId(int id);

    int getTeacherGrade() const;
    double getMonthlySalary() const;
    string getTeacherSubject() const;
    int getTeacherExperienceYears() const;
    const vector<int>& getStudentsIds() const;
    int getNumberOfStudentsInTeacherCourse() const;
};

// Course
class Course : public ShareData {
private:
    Teacher teacher;
    vector<Teacher> courseTeachers;
    int grade;
    int subjectHours;
public:
    void setSingleTeacher(const Teacher& t);
    void setCourseTeacherId(const string &id);
    void addTeacher(const Teacher& t);
    void setGrade(const int &year);
    void setSubjectHours(int hours);
    Teacher getCourseTeacher() const;
    const vector<Teacher>& getCourseTeachers() const;
    int getGrade() const;
    string getCourseTeacherId() const;
    int getSubjectHours() const;
    int getNumberOfCourseTeachers() const;
};

struct TeacherCourse {
    Teacher teacher;
    Course course;
};

//Student
class Student : public BaseEntity {
private:
    int schoolYear;
    double gpa = 0.00;
    vector<TeacherCourse> teacherCoursePairs;
public:
    void setSchoolYear(const int &year);
    void setGpa(double g);
    void setStudentTeacherCourse(const Teacher &teacher, const Course &course);
    int getSchoolYear() const;
    double getGpa() const;
    const vector<TeacherCourse>& getStudentTeacherCourses() const;
    int getNumberOfStudentTeachers() const;
    int getNumberOfStudentCourses() const;
};

#endif
