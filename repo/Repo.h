#ifndef REPO_H
#define REPO_H

#include "../model/Models.h"
using namespace std;

enum Stage { Primary = 1, Middle, Secondary };

Stage getStageFromGrade(int grade); // global function

////////////////// Teacher \\\\\\\\\\\\\\\\\\

class TeacherRepository {
public:
    virtual string addTeacher(int grade, Teacher &teacher) = 0;
};

class TeacherRepositoryImpl : public TeacherRepository {
private:
    const map<Stage, int> maxTeachersPerGradeInStage = { // Max number of teachers that can be in any grade in a certain stage
        {Stage::Primary, 7},
        {Stage::Middle, 9},
        {Stage::Secondary, 12}
    };

    map<int, vector<Teacher>> teachersInGrade;
    map<Stage, vector<Teacher>> teachersInStage;
    vector<Teacher> teachersInSchool;

public:
    void addTeacherInGrade(int grade, Teacher &teacher);
    void addTeacherInStage(int grade, Teacher &teacher);
    void addTeacherInSchool(Teacher &teacher);

    string addTeacher(int grade, Teacher &teacher) override;
};

////////////////// Course \\\\\\\\\\\\\\\\\\

class CourseRepository {
public:
    virtual string addCourse(int grade, Course &course) = 0;
};

class CourseRepositoryImpl : public CourseRepository {
private:
    const map<Stage, int> maxCoursesPerGradeInStage = {
        {Stage::Primary, 7},
        {Stage::Middle, 9},
        {Stage::Secondary, 12}
    };

    map<int, vector<Course>> coursesInGrade;
    map<Stage, vector<Course>> coursesInStage;
    vector<Course> coursesInSchool;

public:
    void addCourseInGrade(int grade, Course &course);
    void addCourseInStage(int grade, Course &course);
    void addCourseInSchool(Course &course);

    const vector<Course>& getCoursesInGrade(int grade) const;

    string addCourse(int grade, Course &course) override;

};

////////////////// Student \\\\\\\\\\\\\\\\\\

class StudentRepository {
public:
    virtual string addStudent(int grade, Student &student) = 0;
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

    string addStudent(int grade, Student &student) override;
};

#endif
