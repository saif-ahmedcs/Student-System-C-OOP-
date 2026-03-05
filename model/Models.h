#ifndef MODELS_H
#define MODELS_H

#include "Stage.h"
#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────
//  ShareData  — root of the model hierarchy
// ─────────────────────────────────────────────────────────────────────────
class ShareData {
private:
    std::string name;
    std::string id;
public:
    virtual ~ShareData() = default;
    void setName(const std::string& name);
    void setId(const std::string& id);
    std::string getName() const;
    std::string getId() const;
};

// ─────────────────────────────────────────────────────────────────────────
//  BaseEntity  — shared human-entity fields
// ─────────────────────────────────────────────────────────────────────────
class BaseEntity : public ShareData {
private:
    int age;
    std::string phoneNumber;
    std::string nationalNumber;
public:
    void setAge(int age);
    void setPhoneNumber(const std::string& phone);
    void setNationalNumber(const std::string& number);
    int getAge() const;
    std::string getPhoneNumber() const;
    std::string getNationalNumber() const;
};

// ─────────────────────────────────────────────────────────────────────────
//  Teacher
// ─────────────────────────────────────────────────────────────────────────
class Teacher : public BaseEntity {
private:
    int grade;
    std::string subject;
    double monthlySalary;
    int experienceYears;
    std::string specialization;
    std::vector<std::string> assignedCourseIds;
    std::vector<int> studentIds;
public:
    void setGrade(int grade);
    void setMonthlySalary(double salary);
    void setSubject(const std::string& subject);
    void setExperienceYears(int years);
    void setSpecialization(const std::string& specialization);

    bool assignCourse(const std::string& courseId);
    bool isCourseAssigned(const std::string& courseId) const;
    void clearAssignedCourses();
    void addStudentId(int id);

    bool removeCourse(const std::string& courseId);
    void removeAllCourses();

    int getGrade() const;
    double getMonthlySalary() const;
    std::string getSubject() const;
    int getExperienceYears() const;
    std::string getSpecialization() const;
    const std::vector<std::string>&  getAssignedCourses() const;
    const std::vector<int>& getStudentIds() const;
    int getNumberOfStudentsInCourse() const;
};

// ─────────────────────────────────────────────────────────────────────────
//  Course
// ─────────────────────────────────────────────────────────────────────────
class Course : public ShareData {
private:
    int grade;
    int subjectHours;
    std::string specialization;
    std::vector<std::string> teacherIds;
    std::vector<std::string> teacherNames;
    std::vector<std::string> assignedStudentIds;
public:
    void setGrade(int grade);
    void setSubjectHours(int hours);
    void setSpecialization(const std::string& specialization);

    bool assignTeacher(const std::string& teacherId, const std::string& teacherName);
    bool assignStudent(const std::string& studentId);
    bool isStudentAssigned(const std::string& studentId) const;

    bool removeTeacherById(const std::string& teacherId);

    int getGrade() const;
    int getSubjectHours() const;
    std::string getSpecialization() const;
    int getNumberOfTeachers() const;
    const std::vector<std::string>& getTeacherIds() const;
    const std::vector<std::string>& getTeacherNames() const;
    const std::vector<std::string>& getAssignedStudents() const;
    int getNumberOfAssignedStudents() const;
};

// ─────────────────────────────────────────────────────────────────────────
//  StudentCourse  — links a course to the teacher chosen for it
// ─────────────────────────────────────────────────────────────────────────
struct StudentCourse {
    std::string courseId;
    std::string teacherName;
};

// ─────────────────────────────────────────────────────────────────────────
//  Student
// ─────────────────────────────────────────────────────────────────────────
class Student : public BaseEntity {
private:
    int grade;
    double gpa = 0.00;
    std::vector<StudentCourse> assignedCourses;
public:
    void setGrade(int grade);
    void setGpa(double gpa);

    bool assignCourse(const std::string& courseId, const std::string& teacherName);
    bool isCourseAssigned(const std::string& courseId) const;

    int getGrade() const;
    double getGpa() const;
    const std::vector<StudentCourse>& getAssignedCourses() const;
    int getNumberOfAssignedCourses() const;
};

#endif
