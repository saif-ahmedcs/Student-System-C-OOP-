#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include "../../repositories/student/StudentRepository.h"
#include "../../repositories/course/CourseRepository.h"
#include "../../common/Validators.h"
#include <string>
#include <vector>

class StudentService {
public:
    virtual Student* findStudentByNationalNumber(const std::string& nationalNumber) = 0;
    virtual Student* findStudentById(const std::string& id) = 0;
    virtual int getMaxStudentsForGrade(int grade) const = 0;
    virtual std::string addStudent(int grade, Student& student) = 0;
    virtual std::string editStudent(const std::string& id, const Student& newData) = 0;
    virtual std::string assignCoursesToStudent(const std::string& studentId, const std::vector<std::string>& courseIds, const std::vector<std::string>& teacherNames) = 0;
    virtual std::string removeStudent(const std::string& id) = 0;
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
    std::string removeStudent(const std::string& id) override;
};

#endif
