#ifndef TEACHER_SERVICE_H
#define TEACHER_SERVICE_H

#include "../../repositories/teacher/TeacherRepository.h"
#include "../../repositories/course/CourseRepository.h"
#include "../../repositories/student/StudentRepository.h"
#include "../../common/Validators.h"
#include <string>
#include <vector>

class TeacherService {
public:
    virtual Teacher* findTeacherByNationalNumber(const std::string& nationalNumber) = 0;
    virtual Teacher* findTeacherById(const std::string& id) = 0;
    virtual int getMaxTeachersForGrade(int grade) const = 0;
    virtual std::string addTeacher(int grade, Teacher& teacher) = 0;
    virtual std::string editTeacher(const std::string& id, const Teacher& newData) = 0;
    virtual std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds, const std::vector<std::vector<int>>& courseClasses) = 0;
    virtual std::string replaceTeacherInCourse(const std::string& courseId, const std::string& oldTeacherId, const std::string& newTeacherId) = 0;
    virtual std::string unassignCourseFromTeacher(const std::string& teacherId, const std::string& courseId) = 0;
    virtual std::string removeTeacher(const std::string& id) = 0;
    virtual ~TeacherService() = default;
};

class TeacherServiceImpl : public TeacherService {
private:
    TeacherRepository& teacherRepository;
    CourseRepository& courseRepository;
    StudentRepository& studentRepository;
    TeacherValidator& teacherValidator;

    int getMinAvailableSeatsForStage(Stage stage) const;
    int getMaxStudentsForStage(Stage stage) const;

public:
    TeacherServiceImpl(TeacherRepository& teacherRepo, CourseRepository& courseRepo, StudentRepository& studentRepo, TeacherValidator& validator);

    Teacher* findTeacherByNationalNumber(const std::string& nationalNumber) override;
    Teacher* findTeacherById(const std::string& id) override;
    int getMaxTeachersForGrade(int grade) const override;
    std::string addTeacher(int grade, Teacher& teacher) override;
    std::string editTeacher(const std::string& id, const Teacher& newData) override;
    std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds, const std::vector<std::vector<int>>& courseClasses) override;
    std::string replaceTeacherInCourse(const std::string& courseId, const std::string& oldTeacherId, const std::string& newTeacherId) override;
    std::string unassignCourseFromTeacher(const std::string& teacherId, const std::string& courseId) override;
    std::string removeTeacher(const std::string& id) override;
};

#endif
