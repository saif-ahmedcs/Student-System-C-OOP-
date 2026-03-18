#ifndef COURSE_CONTROLLER_H
#define COURSE_CONTROLLER_H

#include "../../services/course/CourseService.h"
#include "../../services/student/StudentService.h"
#include "../../repositories/student/StudentRepository.h"
#include "../../repositories/course/CourseRepository.h"
#include "../../repositories/teacher/TeacherRepository.h"
#include "../../common/SchoolConstants.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

class CourseController {
private:
    CourseService& courseService;
    StudentService& studentService;
    StudentRepositoryImpl& studentRepoImpl;
    CourseRepositoryImpl& courseRepoImpl;
    TeacherRepositoryImpl& teacherRepoImpl;
public:
    CourseController(CourseService& cSrv, StudentService& sSrv, StudentRepositoryImpl& sRepo, CourseRepositoryImpl& cRepo, TeacherRepositoryImpl& tRepo);

    Course* findCourseById(const std::string& id);
    std::vector<std::string> getCourseTeacherNames(const std::string& courseId);
    int getMaxCoursesForGrade(int grade) const;
    std::string addCourse(int grade, Course& course);
    std::string editCourse(const std::string& id, const Course& newData);
    std::string removeCourse(const std::string& id);
    void showCourse(const std::string& id);
    void showCourseStudents(const std::string& courseId);
    void showCourseStudentsByTeacher(const std::string& courseId);
    void save() const;
};

#endif
