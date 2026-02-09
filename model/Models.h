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
    string teacherSpecialization;
    vector<string> assignedCourseIds;
    vector<int> studentIds;
public:
    void setTeacherGrade(int grade);
    void setMonthlySalary(double salary);
    void setTeacherSubject(const string &subject);
    void setExperienceYears(int exY);
    void setTeacherSpecialization(const string& specialization);
    void addAssignedCourse(const string& courseId);
    void clearAssignedCourses();
    void addStudentId(int id);

    int getTeacherGrade() const;
    double getMonthlySalary() const;
    string getTeacherSubject() const;
    int getTeacherExperienceYears() const;
    string getTeacherSpecialization() const;
    string getAssignedCourse() const;
    const vector<string>& getAssignedCourses() const;
    const vector<int>& getStudentsIds() const;
    int getNumberOfStudentsInTeacherCourse() const;
};

// Course
class Course : public ShareData {
private:
    string teacherName;
    string teacherId;
    vector<Teacher> courseTeachers;
    int grade;
    int subjectHours;
    string courseSpecialization;

public:
    void setCourseTeacherName(const string &id);
    void setCourseTeacherId (const string &id);
    void addTeacher(const Teacher& t);
    void setGrade(const int &year);
    void setSubjectHours(int hours);
    void setCourseSpecialization(const string& specialization);


    Teacher getCourseTeacher() const;
    const vector<Teacher>& getCourseTeachers() const;
    int getGrade() const;
    string getCourseTeacherName() const;
    string getCourseTeacherId() const;
    string getCourseSpecialization() const;
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
