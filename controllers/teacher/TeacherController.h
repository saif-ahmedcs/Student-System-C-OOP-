#ifndef TEACHER_CONTROLLER_H
#define TEACHER_CONTROLLER_H

#include "../../services/teacher/TeacherService.h"
#include "../../repositories/student/StudentRepository.h"
#include "../../repositories/course/CourseRepository.h"
#include "../../repositories/teacher/TeacherRepository.h"
#include "../../common/SchoolConstants.h"
#include <iostream>
#include <string>
#include <vector>

class TeacherController {
private:
    TeacherService& teacherService;
    StudentRepository& studentRepo;
    CourseRepository& courseRepo;
    TeacherRepository& teacherRepo;
public:
    TeacherController(TeacherService& service, StudentRepository& sRepo, CourseRepository& cRepo, TeacherRepository& tRepo);

    Teacher* findTeacherById(const std::string& id);
    int getMaxTeachersForGrade(int grade) const;
    std::string addTeacher(int grade, Teacher& teacher);
    std::string editTeacher(const std::string& id, const Teacher& newData);
    std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds);
    void showTeacher(const std::string& id);
    std::string removeTeacher(const std::string& id);
    std::string replaceTeacherInCourse(const std::string& courseId, const std::string& oldTeacherId, const std::string& newTeacherId);
    std::string unassignCourseFromTeacher(const std::string& teacherId, const std::string& courseId);
    void listTeachersByGrade(int grade);
    void save() const;
};

#endif
