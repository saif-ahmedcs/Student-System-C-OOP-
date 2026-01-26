#ifndef SERVICE_H
#define SERVICE_H

#include "../repo/Repo.h"
////////////////// Teacher \\\\\\\\\\\\\\\

class TeacherService {
public:
    virtual bool addTeacher(int grade, Teacher &teacher) = 0;
};

class TeacherServiceImpl : public TeacherService {
private:
    TeacherRepositoryImpl &teacherRepository;

public:
    TeacherServiceImpl(TeacherRepositoryImpl &repo);
    bool addTeacher(int grade, Teacher &teacher) override;
};
////////////////// Course \\\\\\\\\\\\\\\

class CourseService {
public:
    virtual bool addCourse(int grade, Course &course) = 0;
};

class CourseServiceImpl : public CourseService {
private:
    CourseRepositoryImpl &courseRepository;

public:
    CourseServiceImpl(CourseRepositoryImpl &repo);
    bool addCourse(int grade, Course &course) override;
};
////////////////// Student \\\\\\\\\\\\\\\

class StudentService {
public:
    virtual string addStudent(int grade, Student &student) = 0;
};

class StudentServiceImpl : public StudentService {
private:
    StudentRepositoryImpl &studentRepository;

    // Validation functions
    bool validateGpa(double gpa);
    bool validatePhoneNumber(const string &phone);
    bool validateGrade(int grade);
    bool validateName(const string &name);

public:
    StudentServiceImpl(StudentRepositoryImpl &repo);
    string addStudent(int grade, Student &student) override;
};

#endif
