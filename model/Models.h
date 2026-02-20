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
    string nationalNumber;
    int grade;
    string teacherSubject;
    double monthlySalary;
    int experienceYears;
    string teacherSpecialization;
    vector<string> assignedCourseIds;
    vector<int> studentIds;
public:
    void setTeacherNationalNum(string n);
    void setTeacherGrade(int grade);
    void setMonthlySalary(double salary);
    void setTeacherSubject(const string &subject);
    void setExperienceYears(int exY);
    void setTeacherSpecialization(const string& specialization);
    void addAssignedCourse(const string& courseId);
    bool isCourseAssigned(const string& courseId) const;
    void clearAssignedCourses();
    void addStudentId(int id);

    string getTeacherNationalNum() const;
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
vector<Teacher*> courseTeachers;
     int grade;
    int subjectHours;
    string courseSpecialization;
    vector<string> teacherNames;
    vector<string> assignedStudentIds;

public:
    void setCourseTeacherName(const string &id);
    void setCourseTeacherId (const string &id);
    void addTeacher(Teacher* t);
    void setGrade(const int &year);
    void setSubjectHours(int hours);
    void setCourseSpecialization(const string& specialization);
    void addTeacherName(const string& teacherName);//ddd
    void addAssignedStudent(const string& studentId);
    bool isStudentAssigned(const string& studentId) const;


    Teacher getCourseTeacher() const;
    const vector<string>& getTeacherNames() const; //ddddd
    const vector<Teacher*>& getCourseTeachers() const;
    int getGrade() const;
    string getCourseTeacherName() const;
    string getCourseTeacherId() const;
    string getCourseSpecialization() const;
    int getSubjectHours() const;
    int getNumberOfCourseTeachers() const;
    const vector<string>& getAssignedStudents() const;
    int getNumberOfAssignedStudents() const;
};

struct StudentCourse {
    string courseId;
    string teacherName;
};

//Student
class Student : public BaseEntity {
private:
    string nationalNumber;
    int schoolYear;
    double gpa = 0.00;
    vector<StudentCourse> assignedCourses;
public:
    void setStudentNationalNum(string n);
    void setSchoolYear(const int &year);
    void setGpa(double g);
    void addAssignedCourse(const string& courseId, const string& teacherName);
    bool isCourseAssigned(const string& courseId) const;

    string getStudentNationalNum() const;
    int getSchoolYear() const;
    double getGpa() const;
    const vector<StudentCourse>& getAssignedCourses() const;
    int getNumberOfAssignedCourses() const;
};

#endif
