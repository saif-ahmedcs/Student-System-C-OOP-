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
    virtual string addCourse(int grade, Course &course) = 0;
    virtual bool isCourseAlreadyRegistered(int grade, const Course &course) = 0;

};

class CourseServiceImpl : public CourseService {
private:
    CourseRepositoryImpl &courseRepository;
    // Validation
    bool validateCourseName(const string &name);
    bool validateAcademicYear(const string &year);
    bool validateSubjectHours(int hours);
    bool validateGrade(int grade);


public:
    CourseServiceImpl(CourseRepositoryImpl &repo);
    bool isCourseAlreadyRegistered(int grade, const Course &course) override;
    string addCourse(int grade, Course &course) override;
};
////////////////// Student \\\\\\\\\\\\\\\

class StudentService {
public:
    virtual string addStudent(int grade, Student &student) = 0;
};

class StudentServiceImpl : public StudentService {
private:
    StudentRepositoryImpl &studentRepository;

    // Validation
    bool validateName(const string &name);
    bool validatePhoneNumber(const string &phone);
    bool validateGrade(int grade);
    bool isCourseAlreadyRegistered(int grade, const Course &course);

public:
    StudentServiceImpl(StudentRepositoryImpl &repo);
    string addStudent(int grade, Student &student) override;
};

#endif
