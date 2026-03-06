#ifndef SERVICE_H
#define SERVICE_H

#include "../repo/Repo.h"
#include "../validator/Validator.h"
#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────
//  Service interfaces
//
//  Responsibility: business rules for a single entity type, plus
//  cross-entity assignment operations .
// ─────────────────────────────────────────────────────────────────────────

// ─────────────────────────────────────────────
//  TeacherService
// ─────────────────────────────────────────────
class TeacherService {
public:
    virtual Teacher* findTeacherByNationalNumber(const std::string& nationalNumber) = 0;
    virtual Teacher* findTeacherById(const std::string& id) = 0;
    virtual int getMaxTeachersForGrade(int grade) const = 0;
    virtual std::string addTeacher(int grade, Teacher& teacher) = 0;
    virtual std::string editTeacher(const std::string& id, const Teacher& newData) = 0;
    virtual std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds) = 0;

    virtual std::string replaceTeacherInCourse(const std::string& courseId, const std::string& oldTeacherId, const std::string& newTeacherId) = 0;
    virtual std::string removeTeacher(const std::string& id) = 0;

    virtual ~TeacherService() = default;
};

class TeacherServiceImpl : public TeacherService {
private:
    TeacherRepository& teacherRepository;
    CourseRepository& courseRepository;
    StudentRepository& studentRepository;
    TeacherValidator& teacherValidator;

public:
    TeacherServiceImpl(TeacherRepository& teacherRepo, CourseRepository&  courseRepo, StudentRepository& studentRepo, TeacherValidator&  validator);

    Teacher* findTeacherByNationalNumber(const std::string& nationalNumber) override;
    Teacher* findTeacherById(const std::string& id) override;
    int getMaxTeachersForGrade(int grade) const override;
    std::string addTeacher(int grade, Teacher& teacher) override;
    std::string editTeacher(const std::string& id, const Teacher& newData) override;
    std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds) override;
    std::string replaceTeacherInCourse(const std::string& courseId, const std::string& oldTeacherId, const std::string& newTeacherId) override;
    std::string removeTeacher(const std::string& id) override;


private:
    int getMinAvailableSeatsForStage(Stage stage) const;
    int getMaxStudentsForStage(Stage stage) const;
};

// ─────────────────────────────────────────────
//  CourseService
// ─────────────────────────────────────────────
class CourseService {
public:
    virtual Course* findCourseById(const std::string& id) = 0;
    virtual int getMaxCoursesForGrade(int grade) const = 0;
    virtual std::string addCourse(int grade, Course& course) = 0;
    virtual std::string editCourse(const std::string& id, const Course& newData) = 0;
    virtual ~CourseService() = default;
};

class CourseServiceImpl : public CourseService {
private:
    CourseRepository& courseRepository;
    CourseValidator& courseValidator;

public:
    CourseServiceImpl(CourseRepository& courseRepo, CourseValidator& validator);

    Course* findCourseById(const std::string& id) override;
    int getMaxCoursesForGrade(int grade) const override;
    std::string addCourse(int grade, Course& course) override;
    std::string editCourse(const std::string& id, const Course& newData) override;
};

// ─────────────────────────────────────────────
//  StudentService
// ─────────────────────────────────────────────
class StudentService {
public:
    virtual Student* findStudentByNationalNumber(const std::string& nationalNumber) = 0;
    virtual Student* findStudentById(const std::string& id) = 0;
    virtual int getMaxStudentsForGrade(int grade) const = 0;
    virtual std::string addStudent(int grade, Student& student) = 0;
    virtual std::string editStudent(const std::string& id, const Student& newData) = 0;
    virtual std::string assignCoursesToStudent(const std::string& studentId, const std::vector<std::string>& courseIds, const std::vector<std::string>& teacherNames) = 0;
    virtual ~StudentService() = default;
};

class StudentServiceImpl : public StudentService {
private:
    StudentRepository& studentRepository;
    CourseRepository& courseRepository;
    StudentValidator& studentValidator;

public:
    StudentServiceImpl(StudentRepository& studentRepo, CourseRepository& courseRepo, StudentValidator& validator);

    Student* findStudentByNationalNumber(const std::string& nationalNumber) override;
    Student* findStudentById(const std::string& id) override;
    int getMaxStudentsForGrade(int grade) const override;
    std::string addStudent(int grade, Student& student) override;
    std::string editStudent(const std::string& id, const Student& newData) override;
    std::string assignCoursesToStudent(const std::string& studentId, const std::vector<std::string>& courseIds, const std::vector<std::string>& teacherNames) override;
};

#endif
