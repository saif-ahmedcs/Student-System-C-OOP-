#ifndef SERVICE_H
#define SERVICE_H
#include "../repo/Repo.h"

////////////////// Teacher \\\\\\\\\\\\\\\

class TeacherService {
public:
    virtual Teacher* findTeacherById(const string& id) = 0;
    virtual string addTeacher(int grade, Teacher &teacher) = 0;
    virtual string editTeacher(const string& id, const Teacher& newData) = 0;
    virtual void showTeacher(const string& id) = 0;

};

class TeacherServiceImpl : public TeacherService {
private:
    TeacherRepositoryImpl &teacherRepository;
    // Validation
    bool validateTeacherName(const string &name);
    bool validateTeacherAge(int age);
    bool validateTeacherExperience(int experienceYears);
    bool validateTeacherGrade(int grade);
    bool validateTeachersLimit(int grade);


public:
    TeacherServiceImpl(TeacherRepositoryImpl &repo);
    Teacher* findTeacherById(const string& id) override;
    string addTeacher(int grade, Teacher &teacher) override;
    string editTeacher(const string& id, const Teacher& newData) override;
    void showTeacher(const string& id) override;


};
////////////////// Course \\\\\\\\\\\\\\\

class CourseService {
public:
    virtual Course* findCourseById(const string& id) = 0;
    virtual string addCourse(int grade, Course &course) = 0;
    virtual string editCourse(const string& id, const Course& newData) = 0;
    virtual void showCourse(const string &id) = 0;


};

class CourseServiceImpl : public CourseService {
private:
    CourseRepositoryImpl &courseRepository;
    // Validation
    bool validateCourseName(const string &name);
    bool validateCourseTeacherName(const string &name);
    bool validateSubjectHours(int hours);
    bool validateGrade(int grade);
    bool validateCoursesLimit(int grade);


public:
    CourseServiceImpl(CourseRepositoryImpl &repo);
    Course* findCourseById(const string& id) override;
    string addCourse(int grade, Course &course) override;
    string editCourse(const string& id, const Course& newData) override;
    void showCourse(const string &id) override;

};
////////////////// Student \\\\\\\\\\\\\\\

class StudentService {
public:
    virtual Student* findStudentById(const string& id) = 0;
    virtual string addStudent(int grade, Student &student) = 0;
    virtual string editStudent(const string& id, const Student& newData) = 0;
    virtual void showStudent(const string &id) = 0;

};

class StudentServiceImpl : public StudentService {
private:
    StudentRepositoryImpl &studentRepository;

    // Validation
    bool validateName(const string &name);
    bool validateAge (int age, int grade);
    bool validatePhoneNumber(const string &phone);
    bool validateGrade(int grade);
    bool validateNewGpa(float gpa);

public:
    StudentServiceImpl(StudentRepositoryImpl &repo);
    Student* findStudentById(const string& id) override;
    string addStudent(int grade, Student &student) override;
    string editStudent(const string& id, const Student& newData) override;
    void showStudent(const string &id) override;


};

#endif
