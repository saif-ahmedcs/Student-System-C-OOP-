#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>
#include <map>
#include "Student.h"

class Course : public ShareData {
private:
    int grade;
    int subjectHours;
    std::string specialization;
    std::vector<std::string> teacherIds;
    std::vector<std::string> teacherNames;
    std::vector<std::string> assignedStudentIds;
    std::map<int, std::string> classTeacherMap;
public:
    void setGrade(int grade);
    void setSubjectHours(int hours);
    void setSpecialization(const std::string& specialization);

    bool assignTeacher(const std::string& teacherId, const std::string& teacherName);
    bool assignStudent(const std::string& studentId);
    bool isStudentAssigned(const std::string& studentId) const;

    bool assignTeacherToClass(int classNum, const std::string& teacherId);
    bool isClassTaken(int classNum) const;
    std::string getTeacherForClass(int classNum) const;
    const std::map<int, std::string>& getClassTeacherMap() const;

    bool removeTeacherById(const std::string& teacherId);
    bool removeStudentById(const std::string& studentId);

    int getGrade() const;
    int getSubjectHours() const;
    std::string getSpecialization() const;
    int getNumberOfTeachers() const;
    const std::vector<std::string>& getTeacherIds() const;
    const std::vector<std::string>& getTeacherNames() const;
    const std::vector<std::string>& getAssignedStudents() const;
    int getNumberOfAssignedStudents() const;
};

#endif
