#ifndef SERVICE_H
#define SERVICE_H
#include "../repo/Repo.h"


class CourseService; //  forward declaration

////////////////// Teacher \\\\\\\\\\\\\\\

class TeacherService {
public:
    virtual Teacher* findTeacherByNationalNumber(const string& nationalNumber) = 0;
    virtual Teacher* findTeacherById(const string& id) = 0;
    virtual int getMaxTeachersForGrade(int grade) const = 0;
    virtual string addTeacher(int grade, Teacher &teacher) = 0;
    virtual string editTeacher(const string& id, const Teacher& newData) = 0;
    virtual string assignCoursesToTeacher(const string& teacherId,const vector<string>& courseIds) = 0;
    virtual void showTeacher(const string& id) = 0;

};

class TeacherServiceImpl : public TeacherService {
private:
    TeacherRepositoryImpl &teacherRepository;
    CourseService& courseService;
    // Validation
    bool isTeacherAlreadyExists(const string& nationalNumber);
    bool validateTeacherName(const string &name);
    bool validateTeacherAge(int age);
    bool validateTeacherExperience(int experienceYears);
    bool validateTeacherGrade(int grade);
    bool validateTeachersLimit(int grade);


public:
    TeacherServiceImpl(TeacherRepositoryImpl &repo, CourseService &courseSrv);

    Teacher* findTeacherByNationalNumber(const string& nationalNumber) override;
    Teacher* findTeacherById(const string& id) override;
    int getMaxTeachersForGrade(int grade) const override;
    string addTeacher(int grade, Teacher &teacher) override;
    string editTeacher(const string& id, const Teacher& newData) override;
    string assignCoursesToTeacher(const string& teacherId,const vector<string>& courseIds) override;
    void showTeacher(const string& id) override;


};

////////////////// Course \\\\\\\\\\\\\\\

class CourseService {
public:
    virtual Course* findCourseById(const string& id) = 0;
    virtual int getMaxCoursesForGrade(int grade) const = 0;
    virtual bool validateCourseTeacherStage(const string& teacherId, int courseGrade) = 0;
    virtual bool validateCourseTeacherSpecialization(const string& teacherId, const string& courseSpecialization) = 0;
    virtual string addCourse(int grade, Course &course) = 0;
    virtual string editCourse(const string& id, const Course& newData) = 0;
    virtual void showCourse(const string &id) = 0;
    virtual void showCourseStudents(const string &courseId) = 0;
    virtual void showCourseStudentsByTeacher(const string &courseId) = 0;


};

class CourseServiceImpl : public CourseService {
private:
    CourseRepositoryImpl &courseRepository;
    TeacherRepositoryImpl& teacherRepository;
    StudentRepositoryImpl& studentRepository;
    // Validation
    bool isCourseAlreadyExists(const string& name, int grade, const string& specialization);
    bool validateCourseName(const string &name);
    bool validateCourseTeacherStage(const string& teacherId,int courseGrade);
    bool validateCourseTeacherSpecialization(const string& teacherId,const string& courseSpecialization);
    bool validateSubjectHours(int hours);
    bool validateGrade(int grade);
    bool validateCoursesLimit(int grade);
    bool validateCourseStudentsLimit(int grade, int currentStudents);



public:
    CourseServiceImpl(CourseRepositoryImpl& courseRepo,TeacherRepositoryImpl& teacherRepo, StudentRepositoryImpl& studentRepo);
    Course* findCourseById(const string& id) override;
    int getMaxCoursesForGrade(int grade) const override;
    string addCourse(int grade, Course &course) override;
    string editCourse(const string& id, const Course& newData) override;
    void showCourse(const string &id) override;
    void showCourseStudents(const string &courseId) override;
    void showCourseStudentsByTeacher(const string &courseId) override;

};

////////////////// Student \\\\\\\\\\\\\\\

class StudentService {
public:
    virtual Student* findStudentByNationalNumber(const string& nationalNumber) = 0;
    virtual Student* findStudentById(const string& id) = 0;
    virtual int getMaxStudentsForGrade(int grade) const = 0;
    virtual string addStudent(int grade, Student &student) = 0;
    virtual string editStudent(const string& id, const Student& newData) = 0;
    virtual void showStudent(const string &id) = 0;
    virtual string assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) = 0;

};

class StudentServiceImpl : public StudentService {
private:
    StudentRepositoryImpl &studentRepository;
    CourseRepositoryImpl &courseRepository;

    // Validation
    bool isStudentAlreadyExists(const string& nationalNumber);
    bool validateName(const string &name);
    bool validateAge (int age, int grade);
    bool validatePhoneNumber(const string &phone);
    bool validateGrade(int grade);
    bool validateStudentsLimit(int grade);
    bool validateNewGpa(float gpa);

public:
    StudentServiceImpl(StudentRepositoryImpl &repo, CourseRepositoryImpl &courseRepo);

    Student* findStudentByNationalNumber(const string& nationalNumber) override;
    Student* findStudentById(const string& id) override;
    int getMaxStudentsForGrade(int grade) const override;
    string addStudent(int grade, Student &student) override;
    string editStudent(const string& id, const Student& newData) override;
    void showStudent(const string &id) override;
    string assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) override;


};

#endif
