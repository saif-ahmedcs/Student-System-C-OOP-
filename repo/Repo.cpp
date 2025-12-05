#include "Repo.h"

////////////////// Global function \\\\\\\\\\\\\\\

Stage getStageFromGrade(int grade) {
    if (grade >= 1 && grade <= 6) return Stage::Primary;
    if (grade >= 7 && grade <= 9) return Stage::Middle;
    if (grade >= 10 && grade <= 12) return Stage::Secondary;
    throw invalid_argument("Invalid grade");
}

////////////////// TeacherRepositoryImpl \\\\\\\\\\\\\\\

void TeacherRepositoryImpl::addTeacherInGrade(int grade, Teacher &teacher) {
    teachersInGrade[grade].push_back(teacher);
}

void TeacherRepositoryImpl::addTeacherInStage(int grade, Teacher &teacher) {
    Stage stage = getStageFromGrade(grade);
    teachersInStage[stage].push_back(teacher);
}

void TeacherRepositoryImpl::addTeacherInSchool(Teacher &teacher) {
    teachersInSchool.push_back(teacher);
}

bool TeacherRepositoryImpl::addTeacher(int grade, Teacher &teacher) {
    Stage stage = getStageFromGrade(grade);
    if (grade < 1 || grade > 12 || teachersInGrade[grade].size() >= maxTeachersPerGradeInStage.at(stage))
        return false;

    addTeacherInGrade(grade, teacher);
    addTeacherInStage(grade, teacher);
    addTeacherInSchool(teacher);
    return true;
}

////////////////// CourseRepositoryImpl \\\\\\\\\\\\\\\

void CourseRepositoryImpl::addCourseInGrade(int grade, Course &course) {
    coursesInGrade[grade].push_back(course);
}

void CourseRepositoryImpl::addCourseInStage(int grade, Course &course) {
    Stage stage = getStageFromGrade(grade);
    coursesInStage[stage].push_back(course);
}

void CourseRepositoryImpl::addCourseInSchool(Course &course) {
    coursesInSchool.push_back(course);
}

bool CourseRepositoryImpl::addCourse(int grade, Course &course) {
    Stage stage = getStageFromGrade(grade);
    if (grade < 1 || grade > 12 || coursesInGrade[grade].size() >= maxCoursesPerGradeInStage.at(stage))
        return false;

    addCourseInGrade(grade, course);
    addCourseInStage(grade, course);
    addCourseInSchool(course);
    return true;
}

////////////////// StudentRepositoryImpl \\\\\\\\\\\\\\\

void StudentRepositoryImpl::addStudentInGrade(int grade, Student &student) {
    studentsInGrade[grade].push_back(student);
}

void StudentRepositoryImpl::addStudentInStage(int grade, Student &student) {
    Stage stage = getStageFromGrade(grade);
    studentsInStage[stage].push_back(student);
}

void StudentRepositoryImpl::addStudentInSchool(Student &student) {
    studentsInSchool.push_back(student);
}

bool StudentRepositoryImpl::addStudent(int grade, Student &student) {
    if (grade < 1 || grade > 12 || studentsInGrade[grade].size() >= maxStudentsPerGrade)
        return false;

    addStudentInGrade(grade, student);
    addStudentInStage(grade, student);
    addStudentInSchool(student);
    return true;
}
