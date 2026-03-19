#ifndef TEACHER_REPOSITORY_H
#define TEACHER_REPOSITORY_H

#include "../../models/Teacher.h"
#include "../../common/Stage.h"
#include <map>
#include <vector>
#include <string>

class TeacherRepository {
public:
    virtual Teacher* findTeacherByNationalNumber(const std::string& nationalNumber) = 0;
    virtual Teacher* findTeacherById(const std::string& id) = 0;
    virtual int getTeachersInGrade(int grade) const = 0;
    virtual std::vector<Teacher*> getTeachersByGrade(int grade) = 0;
    virtual int getMaxTeachersForGrade(int grade) const = 0;
    virtual std::string addTeacher(int grade, Teacher& teacher) = 0;
    virtual std::string editTeacher(const std::string& id, const Teacher& newData) = 0;
    virtual std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds) = 0;
    virtual std::string removeTeacher(const std::string& id) = 0;
    virtual bool saveToFile(const std::string& filename) = 0;
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
    std::vector<Teacher*> getTeachersByGrade(int grade) override;
    int getMaxTeachersForGrade(int grade) const override;
    std::string addTeacher(int grade, Teacher& teacher) override;
    std::string editTeacher(const std::string& id, const Teacher& newData) override;
    std::string assignCoursesToTeacher(const std::string& teacherId, const std::vector<std::string>& courseIds) override;
    std::string removeTeacher(const std::string& id) override;
    bool saveToFile(const std::string& filename) override;
    void loadFromFile(const std::string& filename);
};

#endif
