#ifndef STUDENT_CONTROLLER_H
#define STUDENT_CONTROLLER_H

#include "../../services/student/StudentService.h"
#include "../../services/course/CourseService.h"
#include "../../repositories/student/StudentRepository.h"
#include "../../repositories/course/CourseRepository.h"
#include "../../repositories/teacher/TeacherRepository.h"
#include "../../common/SchoolConstants.h"
#include <iostream>
#include <string>
#include <vector>

class StudentController {
private:
    StudentService& studentService;
    CourseService& courseService;
    StudentRepositoryImpl& studentRepoImpl;
    CourseRepositoryImpl& courseRepoImpl;
    TeacherRepositoryImpl& teacherRepoImpl;
public:
    StudentController(StudentService& sSrv, CourseService& cSrv, StudentRepositoryImpl& sRepo, CourseRepositoryImpl& cRepo, TeacherRepositoryImpl& tRepo);

    Student* findStudentById(const std::string& id);
    int getMaxStudentsForGrade(int grade) const;
    std::string addStudent(int grade, Student& student);
    std::string editStudent(const std::string& id, const Student& newData);
    std::string assignCoursesToStudent(const std::string& studentId, const std::vector<std::string>& courseIds, const std::vector<std::string>& teacherNames);
    void showStudent(const std::string& id);
    std::string removeStudent(const std::string& id);
    void save() const;
};

#endif
