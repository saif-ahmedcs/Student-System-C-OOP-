#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../service/Service.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

class TeacherController {
private:
    TeacherService& teacherService;
public:
    explicit TeacherController(TeacherService& service);

    Teacher* findTeacherById(const std::string& id);
    int getMaxTeachersForGrade(int grade) const;
    std::string addTeacher(int grade, Teacher& teacher);
    std::string editTeacher(const std::string& id, const Teacher& newData);
    std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds);
    void showTeacher(const std::string& id);
    std::string removeTeacher(const std::string& id);
    std::string replaceTeacherInCourse(const std::string& courseId, const std::string& oldTeacherId, const std::string& newTeacherId);
};
class CourseController {
private:
    CourseService&  courseService;
    StudentService& studentService;
public:
    CourseController(CourseService& cSrv, StudentService& sSrv);

    Course* findCourseById(const std::string& id);
    std::vector<std::string> getCourseTeacherNames(const std::string& courseId);
    int getMaxCoursesForGrade(int grade) const;
    std::string addCourse(int grade, Course& course);
    std::string editCourse(const std::string& id, const Course& newData);
    void showCourse(const std::string& id);
    void showCourseStudents(const std::string& courseId);
    void showCourseStudentsByTeacher(const std::string& courseId);
};

class StudentController {
private:
    StudentService& studentService;
    CourseService& courseService;
public:
    StudentController(StudentService& sSrv, CourseService& cSrv);

    Student* findStudentById(const std::string& id);
    int getMaxStudentsForGrade(int grade) const;
    std::string addStudent(int grade, Student& student);
    std::string editStudent(const std::string& id, const Student& newData);
    std::string assignCoursesToStudent(const std::string& studentId, const std::vector<std::string>& courseIds, const std::vector<std::string>& teacherNames);
    void showStudent(const std::string& id);
};

#endif
