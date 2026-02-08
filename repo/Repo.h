#ifndef REPO_H
#define REPO_H

#include "../model/Models.h"
using namespace std;

enum Stage { Primary = 1, Middle, Secondary };

Stage getStageFromGrade(int grade); // global function

////////////////// Teacher \\\\\\\\\\\\\\\\\\

class TeacherRepository {
public:
    virtual Teacher* findTeacherById(const string& id) = 0;
    virtual int getTeachersInGrade(int grade) const = 0;
    virtual int getMaxTeachesForGrade(int grade) const = 0;
    virtual string addTeacher(int grade, Teacher &teacher) = 0;
    virtual string editTeacher(const string& id, const Teacher& newData) = 0;

};

class TeacherRepositoryImpl : public TeacherRepository {
private:
    map<int, vector<Teacher>> teachersInGrade;
    map<Stage, vector<Teacher>> teachersInStage;
    vector<Teacher> teachersInSchool;

public:
    void addTeacherInGrade(int grade, Teacher &teacher);
    void addTeacherInStage(int grade, Teacher &teacher);
    void addTeacherInSchool(Teacher &teacher);

    int getTeachersInGrade(int grade) const override;

    int getMaxTeachesForGrade(int grade) const override;

    Teacher* findTeacherById(const string& id) override;
    string addTeacher(int grade, Teacher &teacher) override;
    string editTeacher(const string& id, const Teacher& newData) override;

};

////////////////// Course \\\\\\\\\\\\\\\\\\

class CourseRepository {
public:
    virtual int getCoursesInGrade(int grade) const = 0;
    virtual int getMaxCoursesForGrade(int grade) const = 0;
    virtual string addCourse(int grade, Course &course) = 0;
    virtual string editCourse(const string& id, const Course& newData) = 0;

};

class CourseRepositoryImpl : public CourseRepository {
private:
    map<int, vector<Course>> coursesInGrade;
    map<Stage, vector<Course>> coursesInStage;
    vector<Course> coursesInSchool;

public:
    void addCourseInGrade(int grade, Course &course);
    void addCourseInStage(int grade, Course &course);
    void addCourseInSchool(Course &course);
    int getCoursesInGrade(int grade) const override;
    int getMaxCoursesForGrade(int grade) const override;
    Course* findCourseById(const string& id);
    string addCourse(int grade, Course &course) override;
    string editCourse(const string& id, const Course& newData) override;



};

////////////////// Student \\\\\\\\\\\\\\\\\\

class StudentRepository {
public:
    virtual int getStudentsInGrade(int grade) const = 0;
    virtual int getMaxStudentsForGrade(int grade) const = 0;
    virtual string addStudent(int grade, Student &student) = 0;
    virtual string editStudent(const string& id, const Student& newData) = 0;

};

class StudentRepositoryImpl : public StudentRepository {
private:
    map<int, vector<Student>> studentsInGrade;
    map<Stage, vector<Student>> studentsInStage;
    vector<Student> studentsInSchool;

public:
    void addStudentInGrade(int grade, Student &student);
    void addStudentInStage(int grade, Student &student);
    void addStudentInSchool(Student &student);
    Student* findStudentById(const string& id);

    int getStudentsInGrade(int grade) const override;
    int getMaxStudentsForGrade(int grade) const override;

    string addStudent(int grade, Student &student) override;
    string editStudent(const string& id, const Student& newData) override;

};

#endif
