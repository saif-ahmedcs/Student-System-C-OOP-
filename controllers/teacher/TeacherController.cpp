#include "TeacherController.h"
using namespace std;

TeacherController::TeacherController(TeacherService& service, StudentRepository& sRepo, CourseRepository& cRepo, TeacherRepository& tRepo)
    : teacherService(service), studentRepo(sRepo), courseRepo(cRepo), teacherRepo(tRepo) {}

void TeacherController::save() const {
    bool ok = true;
    if (!studentRepo.saveToFile(SchoolConstants::FILE_STUDENTS))
    {
        cout << "[ERROR] Failed to save student data.\n";
        ok = false;
    }
    if (!courseRepo.saveToFile(SchoolConstants::FILE_COURSES))
    {
        cout << "[ERROR] Failed to save course data.\n";
        ok = false;
    }
    if (!teacherRepo.saveToFile(SchoolConstants::FILE_TEACHERS))
    {
        cout << "[ERROR] Failed to save teacher data.\n";
        ok = false;
    }
    if (ok)
    {
        cout << "[Saved]\n";
    }
}

Teacher* TeacherController::findTeacherById(const string& id) {
    return teacherService.findTeacherById(id);
}

int TeacherController::getMaxTeachersForGrade(int grade) const {
    return teacherService.getMaxTeachersForGrade(grade);
}

string TeacherController::addTeacher(int grade, Teacher& teacher) {
    return teacherService.addTeacher(grade, teacher);
}

string TeacherController::editTeacher(const string& id, const Teacher& newData) {
    return teacherService.editTeacher(id, newData);
}

string TeacherController::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds) {
    return teacherService.assignCoursesToTeacher(teacherId, courseIds);
}

void TeacherController::showTeacher(const string& id) {
    Teacher* t = teacherService.findTeacherById(id);
    if (!t)
    {
        cout << "Teacher not found.\n";
        return;
    }

    cout << "--------------------------\n";
    cout << "Teacher Name: " << t->getName() << "\n";
    cout << "Age: " << t->getAge() << "\n";
    cout << "Experience Years: " << t->getExperienceYears() << "\n";
    cout << "Subject: " << t->getSubject() << "\n";
    cout << "Specialization: " << t->getSpecialization() << "\n";
    cout << "Grade: " << t->getGrade() << "\n";
    cout << "Assigned Courses: ";

    const vector<string>& courses = t->getAssignedCourses();
    if (courses.empty())
    {
        cout << "NONE";
    }
    else
    {
        for (int i = 0; i < (int)courses.size(); i++)
        {
            cout << courses[i];
            if (i != (int)courses.size() - 1)
            {
                cout << ", ";
            }
        }
    }
    cout << "\n";
    cout << "Monthly Salary: $" << t->getMonthlySalary() << "\n";
    cout << "--------------------------\n";
}

string TeacherController::removeTeacher(const string& id) {
    return teacherService.removeTeacher(id);
}

string TeacherController::replaceTeacherInCourse(const string& courseId, const string& oldTeacherId, const string& newTeacherId) {
    return teacherService.replaceTeacherInCourse(courseId, oldTeacherId, newTeacherId);
}

string TeacherController::unassignCourseFromTeacher(const string& teacherId, const string& courseId) {
    return teacherService.unassignCourseFromTeacher(teacherId, courseId);
}
