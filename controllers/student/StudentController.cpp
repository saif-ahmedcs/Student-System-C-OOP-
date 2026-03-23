#include "StudentController.h"
#include <iomanip>
using namespace std;

StudentController::StudentController(StudentService& sSrv, CourseService& cSrv, StudentRepository& sRepo, CourseRepository& cRepo, TeacherRepository& tRepo)
    : studentService(sSrv), courseService(cSrv), studentRepo(sRepo), courseRepo(cRepo), teacherRepo(tRepo) {}

void StudentController::save() const {
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
    if (!s)
    {
        cout << "Student not found.\n";
        return;
    }

    cout << "-----------------------------------\n";
    cout << "Student Name: " << s->getName() << "\n";
    cout << "Student ID: " << s->getId() << "\n";
    cout << "School Grade: " << s->getGrade() << "\n";
    cout << "Class: " << s->getGrade() << "/" << s->getClassNumber() << "\n";
    cout << "Age: " << s->getAge() << "\n";
    cout << "Phone Number: " << s->getPhoneNumber() << "\n";
    cout << "GPA: " << s->getGpa() << "\n";
    cout << "-----------------------------------\n";

    const vector<StudentCourse>& courses = s->getAssignedCourses();
    int courseCount = (int)courses.size();
    cout << "Registered Courses: " << courseCount << "\n";
    cout << "-----------------------------------\n";

    if (courseCount == 0)
    {
        cout << "No courses registered yet.\n";
    }
    else
    {
        for (int i = 0; i < courseCount; i++)
        {
            Course* course = courseService.findCourseById(courses[i].courseId);
            if (course)
            {
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

string StudentController::removeStudent(const string& id) {
    return studentService.removeStudent(id);
}

void StudentController::listStudentsByGrade(int grade) {
    vector<Student*> students = studentRepo.getStudentsByGrade(grade);

    cout << "Students in Grade " << grade << " (" << students.size() << ")\n";
    cout << "\033[36m-----------------------------------------------------------------\033[0m\n";
    cout << "\033[36m|\033[0m " << left << setw(5)  << "No."
         << "\033[36m|\033[0m " << left << setw(25) << "Student Name"
         << "\033[36m|\033[0m " << left << setw(13) << "Student ID"
         << "\033[36m|\033[0m " << left << setw(9)  << "Class"
         << "\033[36m|\033[0m " << left << setw(9)  << "Courses"
         << "\033[36m|\033[0m\n";
    cout << "\033[36m-----------------------------------------------------------------\033[0m\n";

    if (students.empty())
    {
        cout << "No students found in this grade.\n";
        cout << "\033[36m=================================================================\033[0m\n";
        return;
    }

    for (int i = 0; i < (int)students.size(); i++)
    {
        string classLabel = to_string(students[i]->getGrade()) + "/" + to_string(students[i]->getClassNumber());
        cout << "\033[36m|\033[0m " << left << setw(5)  << (i + 1)
             << "\033[36m|\033[0m " << left << setw(25) << students[i]->getName()
             << "\033[36m|\033[0m " << left << setw(13) << students[i]->getId()
             << "\033[36m|\033[0m " << left << setw(9)  << classLabel
             << "\033[36m|\033[0m " << left << setw(9)  << students[i]->getNumberOfAssignedCourses()
             << "\033[36m|\033[0m\n";
    }
    cout << "\033[36m-----------------------------------------------------------------\033[0m\n";
}

void StudentController::listStudentsByGpa(int grade) {
    vector<Student*> students = studentRepo.getStudentsByGrade(grade);

    for (int i = 0; i < (int)students.size() - 1; i++) {
        for (int j = i + 1; j < (int)students.size(); j++) {
            if (students[j]->getGpa() > students[i]->getGpa()) {
                Student* tmp = students[i];
                students[i] = students[j];
                students[j] = tmp;
            }
        }
    }

    cout << "Students in Grade " << grade << " Sorted by GPA (" << students.size() << ")\n";
    cout << "\033[36m-----------------------------------------------------------------\033[0m\n";
    cout << "\033[36m|\033[0m " << left << setw(5)  << "No."
         << "\033[36m|\033[0m " << left << setw(25) << "Student Name"
         << "\033[36m|\033[0m " << left << setw(13) << "Student ID"
         << "\033[36m|\033[0m " << left << setw(9)  << "Class"
         << "\033[36m|\033[0m " << left << setw(9)  << "GPA"
         << "\033[36m|\033[0m\n";
    cout << "\033[36m-----------------------------------------------------------------\033[0m\n";

    if (students.empty())
    {
        cout << "No students found in this grade.\n";
        cout << "\033[36m=================================================================\033[0m\n";
        return;
    }

    for (int i = 0; i < (int)students.size(); i++)
    {
        string classLabel = to_string(students[i]->getGrade()) + "/" + to_string(students[i]->getClassNumber());
        cout << "\033[36m|\033[0m " << left << setw(5)  << (i + 1)
             << "\033[36m|\033[0m " << left << setw(25) << students[i]->getName()
             << "\033[36m|\033[0m " << left << setw(13) << students[i]->getId()
             << "\033[36m|\033[0m " << left << setw(9)  << classLabel
             << "\033[36m|\033[0m " << left << setw(9)  << fixed << setprecision(2) << students[i]->getGpa()
             << "\033[36m|\033[0m\n";
    }
    cout << "\033[36m-----------------------------------------------------------------\033[0m\n";
}
