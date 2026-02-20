#include "Controller.h"

// Teacher
TeacherController::TeacherController(TeacherServiceImpl &service) : teacherService(service) {}

Teacher* TeacherController::findTeacherById(const string& id){
    return teacherService.findTeacherById(id);
}

int TeacherController::getMaxTeachersForGrade(int grade)const {
    return teacherService.getMaxTeachersForGrade(grade);
}

string TeacherController::addTeacher(int grade, Teacher &teacher) {
    return teacherService.addTeacher(grade, teacher);
}

string TeacherController::editTeacher(const string& id, const Teacher& newData){
    return teacherService.editTeacher(id,newData);
}

string TeacherController::assignCoursesToTeacher(const string& teacherId,const vector<string>& courseIds) {
    return teacherService.assignCoursesToTeacher(teacherId, courseIds);
}

void TeacherController::showTeacher(const string& id){
    return teacherService.showTeacher(id);
}

// Course
CourseController::CourseController(CourseServiceImpl &service) : courseService(service) {}


Course* CourseController::findCourseById(const string& id){
    return courseService.findCourseById(id);
}

int CourseController::getMaxCoursesForGrade(int grade)const {
    return courseService.getMaxCoursesForGrade(grade);
}


string CourseController::addCourse(int grade, Course &course) {
    return courseService.addCourse(grade, course);
}

string CourseController::editCourse(const string& id, const Course& newData) {
    return courseService.editCourse(id, newData);
}
void CourseController::showCourse(const string& id){
    return courseService.showCourse(id);
}

void CourseController::showCourseStudents(const string& courseId){
    return courseService.showCourseStudents(courseId);
}

// Student
StudentController::StudentController(StudentServiceImpl &service) : studentService(service) {}

Student* StudentController::findStudentById(const string& id){
    return studentService.findStudentById(id);
}
int StudentController::getMaxStudentsForGrade(int grade)const {
    return studentService.getMaxStudentsForGrade(grade);
}

string StudentController::addStudent(int grade, Student &student) {
    return studentService.addStudent(grade, student);
}

string StudentController::editStudent(const string& id, const Student& newData) {
    return studentService.editStudent(id, newData);
}

void StudentController::showStudent(const string& id){
    return studentService.showStudent(id);
}

string StudentController::assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames){
    return studentService.assignCoursesToStudent(studentId, courseIds, teacherNames);
}
