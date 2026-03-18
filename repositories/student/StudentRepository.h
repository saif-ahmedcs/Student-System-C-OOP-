#ifndef STUDENT_REPOSITORY_H
#define STUDENT_REPOSITORY_H

#include "../../models/Student.h"
#include "../../common/Stage.h"
#include <map>
#include <vector>
#include <string>

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
    bool saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};

#endif
