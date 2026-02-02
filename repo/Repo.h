#ifndef REPO_H
#define REPO_H

#include "../model/Models.h"
using namespace std;

enum Stage { Primary = 1, Middle, Secondary };

Stage getStageFromGrade(int grade); // global function

////////////////// Teacher \\\\\\\\\\\\\\\\\\

class TeacherRepository {
public:

    virtual const vector<Teacher>& getTeachersInGrade(int grade) const = 0;
    virtual int getMaxTeachersForStage(Stage stage) const = 0;
    virtual string addTeacher(int grade, Teacher &teacher) = 0;
    virtual string editTeacher(const string& id, const Teacher& newData) = 0;

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

    const vector<Teacher>& getTeachersInGrade(int grade) const override;

    int getMaxTeachersForStage(Stage stage) const override;

    Teacher* findTeacherById(const string& id);


    string addTeacher(int grade, Teacher &teacher) override;

    string editTeacher(const string& id, const Teacher& newData) override;

};

////////////////// Course \\\\\\\\\\\\\\\\\\

class CourseRepository {
public:
    virtual const vector<Course>& getCoursesInGrade(int grade) const = 0;
    virtual int getMaxCoursesForStage(Stage stage) const = 0;

    virtual string addCourse(int grade, Course &course) = 0;
    virtual string editCourse(const string& id, const Course& newData) = 0;

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

    const vector<Course>& getCoursesInGrade(int grade) const override;
    int getMaxCoursesForStage(Stage stage) const override;

    bool findCourseById(const string& id) const;


    string addCourse(int grade, Course &course) override;
    string editCourse(const string& id, const Course& newData) override;



};

////////////////// Student \\\\\\\\\\\\\\\\\\

class StudentRepository {
public:
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


    string addStudent(int grade, Student &student) override;
    string editStudent(const string& id, const Student& newData) override;

};

#endif
