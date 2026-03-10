#ifndef REPO_H
#define REPO_H

#include "../model/Models.h"
#include <map>
#include <vector>
#include <string>

// ─────────────────────────────────────────────────────────────────────────
//  Repository interfaces
//
//  Responsibility: data storage and retrieval ONLY.
// ─────────────────────────────────────────────────────────────────────────

// ─────────────────────────────────────────────
//  TeacherRepository
// ─────────────────────────────────────────────
class TeacherRepository {
public:
    virtual Teacher* findTeacherByNationalNumber(const std::string& nationalNumber) = 0;
    virtual Teacher* findTeacherById(const std::string& id) = 0;
    virtual int getTeachersInGrade(int grade) const = 0;
    virtual int getMaxTeachersForGrade(int grade) const = 0;
    virtual std::string addTeacher(int grade, Teacher& teacher) = 0;
    virtual std::string editTeacher(const std::string& id, const Teacher& newData) = 0;
    virtual std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds) = 0;
    virtual std::string removeTeacher(const std::string& id) = 0;

    virtual ~TeacherRepository() = default;
};

class TeacherRepositoryImpl : public TeacherRepository {
private:
    std::vector<Teacher> allTeachers;
    std::map<int, std::vector<int>> gradeIndex;
    std::map<Stage, std::vector<int>> stageIndex;
public:
    Teacher* findTeacherByNationalNumber(const std::string& nationalNumber) override;
    Teacher* findTeacherById(const std::string& id) override;
    int getTeachersInGrade(int grade) const override;
    int getMaxTeachersForGrade(int grade) const override;
    std::string addTeacher(int grade, Teacher& teacher) override;
    std::string editTeacher(const std::string& id, const Teacher& newData) override;
    std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds) override;
    std::string removeTeacher(const std::string& id) override;
};

// ─────────────────────────────────────────────
//  CourseRepository
// ─────────────────────────────────────────────
class CourseRepository {
public:
    virtual Course* findCourseById(const std::string& id) = 0;
    virtual int getNumberOfCoursesInGrade(int grade) const = 0;
    virtual std::vector<Course> getCoursesInSchoolVector() = 0;
    virtual int getMaxCoursesForGrade(int grade) const = 0;
    virtual std::string addCourse(int grade, Course& course) = 0;
    virtual std::string editCourse(const std::string& id, const Course& newData) = 0;
    virtual std::string assignTeacherToCourse(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) = 0;
    virtual std::string assignTeacherToCourseForReplace(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) = 0;
    virtual std::string assignStudentToCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::string removeStudentFromCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::string removeCourse(const std::string& id) = 0;

    virtual ~CourseRepository() = default;
};

class CourseRepositoryImpl : public CourseRepository {
private:
    std::vector<Course> allCourses;
    std::map<int, std::vector<int>> gradeIndex;
    std::map<Stage, std::vector<int>> stageIndex;
public:
    Course* findCourseById(const std::string& id) override;
    int getNumberOfCoursesInGrade(int grade) const override;
    std::vector<Course> getCoursesInSchoolVector() override;
    int getMaxCoursesForGrade(int grade) const override;
    std::string addCourse(int grade, Course& course) override;
    std::string editCourse(const std::string& id, const Course& newData) override;
    std::string assignTeacherToCourse(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) override;
    std::string assignTeacherToCourseForReplace(const std::string& courseId, const std::string& teacherId, const std::string& teacherName) override;
    std::string assignStudentToCourse(const std::string& studentId, const std::string& courseId) override;
    std::string removeStudentFromCourse(const std::string& studentId, const std::string& courseId) override;
    std::string removeCourse(const std::string& id) override;
};

// ─────────────────────────────────────────────
//  StudentRepository
// ─────────────────────────────────────────────
class StudentRepository {
public:
    virtual Student* findStudentByNationalNumber(const std::string& nationalNumber) = 0;
    virtual Student* findStudentById(const std::string& id) = 0;
    virtual int getStudentsInGrade(int grade) const = 0;
    virtual int getMaxStudentsForGrade(int grade) const = 0;
    virtual std::string addStudent(int grade, Student& student) = 0;
    virtual std::string editStudent(const std::string& id, const Student& newData) = 0;
    virtual std::string assignCoursesToStudent(const std::string& studentId, const std::vector<std::string>& courseIds, const std::vector<std::string>& teacherNames) = 0;
    virtual std::string removeStudent(const std::string& id) = 0;

    virtual ~StudentRepository() = default;
};

class StudentRepositoryImpl : public StudentRepository {
private:
    std::vector<Student> allStudents;
    std::map<int, std::vector<int>> gradeIndex;
    std::map<Stage, std::vector<int>> stageIndex;
public:
    Student* findStudentByNationalNumber(const std::string& nationalNumber) override;
    Student* findStudentById(const std::string& id) override;
    int getStudentsInGrade(int grade) const override;
    int getMaxStudentsForGrade(int grade) const override;
    std::string addStudent(int grade, Student& student) override;
    std::string editStudent(const std::string& id, const Student& newData) override;
    std::string assignCoursesToStudent(const std::string& studentId, const std::vector<std::string>& courseIds, const std::vector<std::string>& teacherNames) override;
    std::string removeStudent(const std::string& id) override;
};

#endif
