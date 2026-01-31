#ifndef SERVICE_H
#define SERVICE_H
#include "../repo/Repo.h"
////////////////// Teacher \\\\\\\\\\\\\\\

class TeacherService {
public:
    virtual string addTeacher(int grade, Teacher &teacher) = 0;
};

class TeacherServiceImpl : public TeacherService {
private:
    TeacherRepositoryImpl &teacherRepository;
    // Validation
    bool validateTeacherName(const string &name);
    bool validateTeacherGrade(int grade);
    bool validateTeachersLimit(int grade);


public:
    TeacherServiceImpl(TeacherRepositoryImpl &repo);
    string addTeacher(int grade, Teacher &teacher) override;
};
////////////////// Course \\\\\\\\\\\\\\\

class CourseService {
public:
    virtual string addCourse(int grade, Course &course) = 0;
    virtual string editCourse(const string& id, const Course& newData) = 0;

};

class CourseServiceImpl : public CourseService {
private:
    CourseRepositoryImpl &courseRepository;
    // Validation
    bool validateCourseName(const string &name);
    bool validateSubjectHours(int hours);
    bool validateGrade(int grade);
    bool validateCoursesLimit(int grade);


public:
    CourseServiceImpl(CourseRepositoryImpl &repo);
    string addCourse(int grade, Course &course) override;
    string editCourse(const string& id, const Course& newData) override;

};
////////////////// Student \\\\\\\\\\\\\\\

class StudentService {
public:
    virtual string addStudent(int grade, Student &student) = 0;
    virtual string editStudent(const string& id, const Student& newData) = 0;

};

class StudentServiceImpl : public StudentService {
private:
    StudentRepositoryImpl &studentRepository;

    // Validation
    bool validateName(const string &name);
    bool validatePhoneNumber(const string &phone);
    bool validateGrade(int grade);
    bool validateNewGpa(float gpa);

public:
    StudentServiceImpl(StudentRepositoryImpl &repo);
    string addStudent(int grade, Student &student) override;
    string editStudent(const string& id, const Student& newData) override;


};

#endif
