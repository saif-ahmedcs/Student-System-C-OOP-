#ifndef UI_HANDLERS_H
#define UI_HANDLERS_H

#include "../controllers/student/StudentController.h"
#include "../controllers/course/CourseController.h"
#include "../controllers/teacher/TeacherController.h"
#include <string>

int readInt(const std::string& prompt);
double readDouble(const std::string& prompt);
bool confirmAction(const std::string& message);

void displayMainMenu();
void showProcesses(const std::string& s);

void handleAddStudent(StudentController& studentCtrl);
void handleRemoveStudent(StudentController& studentCtrl);
void handleEditStudent(StudentController& studentCtrl);
void handleAssignCoursesToStudent(StudentController& studentCtrl, CourseController& courseCtrl);
void handleStudentOperations(StudentController& studentCtrl, CourseController& courseCtrl);

void handleAddCourse(CourseController& courseCtrl);
void handleRemoveCourse(CourseController& courseCtrl);
void handleEditCourse(CourseController& courseCtrl);
void handleReplaceTeacherInCourse(TeacherController& teacherCtrl, CourseController& courseCtrl);
void handleCourseOperations(CourseController& courseCtrl, TeacherController& teacherCtrl);

void handleAddTeacher(TeacherController& teacherCtrl);
void handleRemoveTeacher(TeacherController& teacherCtrl);
void handleEditTeacher(TeacherController& teacherCtrl);
void handleAssignCoursesToTeacher(TeacherController& teacherCtrl);
void handleTeacherOperations(TeacherController& teacherCtrl, CourseController& courseCtrl);

#endif
