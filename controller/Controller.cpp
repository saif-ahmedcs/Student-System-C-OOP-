#include "Controller.h"
#include <limits>
using namespace std;

// ─── TeacherController ──────────────────────────────────────────────────

TeacherController::TeacherController(TeacherService& service)
    : teacherService(service) {}

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
    if (!t) {
        cout << "Teacher not found.\n"; return;
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
    if (courses.empty()) {
        cout << "NONE";
    } else {
        for (int i = 0; i < (int)courses.size(); i++) {
            cout << courses[i];
            if (i != (int)courses.size() - 1) cout << ", ";
        }
    }
    cout << "\n";
    cout << "Monthly Salary: $" << t->getMonthlySalary() << "\n";
    cout << "--------------------------\n";
}

string TeacherController::removeTeacher(const string& id) {
    return teacherService.removeTeacher(id);
}

// ─── CourseController ───────────────────────────────────────────────────

CourseController::CourseController(CourseService& cSrv, StudentService& sSrv)
    : courseService(cSrv), studentService(sSrv) {}

Course* CourseController::findCourseById(const string& id) {
    return courseService.findCourseById(id);
}

vector<string> CourseController::getCourseTeacherNames(const string& courseId) {
    Course* c = courseService.findCourseById(courseId);
    if (!c)
        return vector<string>();
    return c->getTeacherNames();
}

int CourseController::getMaxCoursesForGrade(int grade) const {
    return courseService.getMaxCoursesForGrade(grade);
}

string CourseController::addCourse(int grade, Course& course) {
    return courseService.addCourse(grade, course);
}

string CourseController::editCourse(const string& id, const Course& newData) {
    return courseService.editCourse(id, newData);
}

void CourseController::showCourse(const string& id) {
    Course* c = courseService.findCourseById(id);
    if (!c) {
        cout << "Course not found.\n"; return;
    }

    cout << "-----------------------------------\n";
    cout << "Course Name: " << c->getName() << "\n";
    cout << "Course ID: " << c->getId() << "\n";
    cout << "Grade: " << c->getGrade() << "\n";
    cout << "Subject Hours: " << c->getSubjectHours() << "\n";

    const vector<string>& teachers = c->getTeacherNames();
    if (teachers.empty()) {
        cout << "Teachers: NONE\n";
    } else {
        cout << "Teachers: ";
        for (int i = 0; i < (int)teachers.size(); i++) {
            cout << teachers[i];
            if (i != (int)teachers.size() - 1)
                cout << ", ";
        }
        cout << "\n";
    }
    cout << "-----------------------------------\n";
}

void CourseController::showCourseStudents(const string& courseId) {
    Course* c = courseService.findCourseById(courseId);
    if (!c) {
        cout << "Course not found.\n"; return;
    }

    const vector<string>& assigned = c->getAssignedStudents();

    cout << "\n\033[35m==================================================\033[0m\n";
    cout << left << setw(14) << "Course Name" << ": " << c->getName()  << "\n";
    cout << left << setw(14) << "Course ID"   << ": " << c->getId()    << "\n";
    cout << left << setw(14) << "Grade"       << ": " << c->getGrade() << "\n";
    cout << "\033[35m==================================================\033[0m\n";

    if (assigned.empty()) {
        cout << "No students assigned in this course yet.\n";
        cout << "\033[35m==================================================\033[0m\n";
        return;
    }

    cout << "\nTotal Assigned Students: " << assigned.size() << "\n";
    cout << "\033[35m--------------------------------------------------\033[0m\n";
    cout << "\033[35m|\033[0m " << left << setw(5)  << "No."
         << "\033[35m|\033[0m " << left << setw(25) << "Student Name"
         << "\033[35m|\033[0m " << left << setw(13) << "Student ID"
         << "\033[35m|\033[0m\n";
    cout << "\033[35m--------------------------------------------------\033[0m\n";

    for (int i = 0; i < (int)assigned.size(); i++) {
        Student* s = studentService.findStudentById(assigned[i]);
        if (s) {
            cout << "\033[35m|\033[0m " << left << setw(5)  << (i + 1)
                 << "\033[35m|\033[0m " << left << setw(25) << s->getName()
                 << "\033[35m|\033[0m " << left << setw(13) << s->getId()
                 << "\033[35m|\033[0m\n";
        }
    }
    cout << "\033[35m--------------------------------------------------\033[0m\n";
}

void CourseController::showCourseStudentsByTeacher(const string& courseId) {
    Course* c = courseService.findCourseById(courseId);
    if (!c) {
       cout << "Course not found.\n";
       return;
    }

    const vector<string>& teachers = c->getTeacherNames();

    if (teachers.empty()) {
        cout << "No teachers assigned to this course.\n"; return;
    }

    string selectedTeacher;
    if (teachers.size() == 1) {
        selectedTeacher = teachers[0];
    } else {
        cout << "\nTeachers for " << c->getName() << ":\n";
        for (int i = 0; i < (int)teachers.size(); i++)
            cout << (i + 1) << ". " << teachers[i] << "\n";

        cout << "\nSelect teacher number (1-" << teachers.size() << "): ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > (int)teachers.size()) {
            cout << "Invalid choice.\n"; return;
        }
        selectedTeacher = teachers[choice - 1];
    }

    cout << "\n\033[35m==================================================\033[0m\n";
    cout << left << setw(14) << "Course Name"  << ": " << c->getName()    << "\n";
    cout << left << setw(14) << "Teacher Name" << ": " << selectedTeacher << "\n";
    cout << "\033[35m==================================================\033[0m\n";

    const vector<string>& allStudents = c->getAssignedStudents();
    vector<Student*> result;

    for (int i = 0; i < (int)allStudents.size(); i++) {
        Student* s = studentService.findStudentById(allStudents[i]);
        if (!s) {
            continue;
        }

        const vector<StudentCourse>& sc = s->getAssignedCourses();

        for (int j = 0; j < (int)sc.size(); j++) {
            if (sc[j].courseId == c->getId() && sc[j].teacherName == selectedTeacher) {
                result.push_back(s);
                break;
            }
        }
    }

    if (result.empty()) {
        cout << "No students registered with this teacher.\n";
        cout << "\033[35m==================================================\033[0m\n";
        return;
    }

    cout << "\nRegistered Students (" << result.size() << "):\n";
    cout << "\033[35m--------------------------------------------------\033[0m\n";
    cout << "\033[35m|\033[0m " << left << setw(5)  << "No."
         << "\033[35m|\033[0m " << left << setw(25) << "Student Name"
         << "\033[35m|\033[0m " << left << setw(13) << "Student ID"
         << "\033[35m|\033[0m\n";
    cout << "\033[35m--------------------------------------------------\033[0m\n";

    for (int i = 0; i < (int)result.size(); i++) {
        cout << "\033[35m|\033[0m " << left << setw(5)  << (i + 1)
             << "\033[35m|\033[0m " << left << setw(25) << result[i]->getName()
             << "\033[35m|\033[0m " << left << setw(13) << result[i]->getId()
             << "\033[35m|\033[0m\n";
    }
    cout << "\033[35m--------------------------------------------------\033[0m\n";
}

// ─── StudentController ──────────────────────────────────────────────────

StudentController::StudentController(StudentService& sSrv, CourseService& cSrv)
    : studentService(sSrv), courseService(cSrv) {}

Student* StudentController::findStudentById(const string& id) {
    return studentService.findStudentById(id);
}

int StudentController::getMaxStudentsForGrade(int grade) const {
    return studentService.getMaxStudentsForGrade(grade);
}

string StudentController::addStudent(int grade, Student& student) {
    return studentService.addStudent(grade, student);
}

string StudentController::editStudent(const string& id, const Student& newData) {
    return studentService.editStudent(id, newData);
}

string StudentController::assignCoursesToStudent(const string& studentId, const vector<string>& courseIds, const vector<string>& teacherNames) {
    return studentService.assignCoursesToStudent(studentId, courseIds, teacherNames);
}

void StudentController::showStudent(const string& id) {
    Student* s = studentService.findStudentById(id);
    if (!s) {
      cout << "Student not found.\n";
      return;
    }

    cout << "-----------------------------------\n";
    cout << "Student Name: " << s->getName() << "\n";
    cout << "Student ID: " << s->getId() << "\n";
    cout << "School Grade: " << s->getGrade() << "\n";
    cout << "Age: " << s->getAge() << "\n";
    cout << "Phone Number: " << s->getPhoneNumber() << "\n";
    cout << "GPA: " << s->getGpa() << "\n";
    cout << "-----------------------------------\n";

    const vector<StudentCourse>& courses = s->getAssignedCourses();
    int courseCount = (int)courses.size();
    cout << "Registered Courses: " << courseCount << "\n";
    cout << "-----------------------------------\n";

    if (courseCount == 0) {
        cout << "No courses registered yet.\n";
    } else {
        for (int i = 0; i < courseCount; i++) {
            Course* course = courseService.findCourseById(courses[i].courseId);
            if (course) {
                cout << "Course #" << (i + 1) << ": " << course->getName() << "\n";
                cout << "Teacher: " << courses[i].teacherName << "\n\n";
            }
        }
    }

    int required  = courseService.getMaxCoursesForGrade(s->getGrade());
    int remaining = required - courseCount;
    cout << "-----------------------------------\n";
    cout << "Required courses for this grade: " << required  << "\n";
    cout << "Courses still needed: "            << remaining << "\n";
    cout << "-----------------------------------\n";
}
