#include "StudentController.h"
#include "../../common/SaveManager.h"
#include "../../common/TablePrinter.h"
#include <iomanip>
using namespace std;

StudentController::StudentController(StudentService& sSrv, CourseService& cSrv, StudentRepository& sRepo, CourseRepository& cRepo, TeacherRepository& tRepo)
    : studentService(sSrv), courseService(cSrv), studentRepo(sRepo), courseRepo(cRepo), teacherRepo(tRepo) {}

void StudentController::save() const {
    saveAll(studentRepo, courseRepo, teacherRepo);
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

static const string CYAN = "\033[36m";
static const int STUDENT_TABLE_WIDTH = 65;

static void printStudentRow(const string& color, int number, Student* s, const string& lastCell) {
    string classLabel = to_string(s->getGrade()) + "/" + to_string(s->getClassNumber());
    printTableCell(color, 5);  cout << (number);
    printTableCell(color, 25); cout << s->getName();
    printTableCell(color, 13); cout << s->getId();
    printTableCell(color, 9);  cout << classLabel;
    printTableCell(color, 9);  cout << lastCell;
    printTableRowEnd(color);
}

void StudentController::listStudentsByGrade(int grade) {
    vector<Student*> students = studentRepo.getStudentsByGrade(grade);

    cout << "Students in Grade " << grade << " (" << students.size() << ")\n";

    vector<TableColumn> cols = {
        {"No.",          5},
        {"Student Name", 25},
        {"Student ID",   13},
        {"Class",        9},
        {"Courses",      9}
    };
    printTableHeader(CYAN, STUDENT_TABLE_WIDTH, cols);

    if (students.empty())
    {
        printEmptyTableNotice(CYAN, STUDENT_TABLE_WIDTH, "No students found in this grade.");
        return;
    }

    for (int i = 0; i < (int)students.size(); i++)
    {
        printStudentRow(CYAN, i + 1, students[i], to_string(students[i]->getNumberOfAssignedCourses()));
    }
    printTableDivider(CYAN, STUDENT_TABLE_WIDTH);
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

    vector<TableColumn> cols = {
        {"No.",          5},
        {"Student Name", 25},
        {"Student ID",   13},
        {"Class",        9},
        {"GPA",          9}
    };
    printTableHeader(CYAN, STUDENT_TABLE_WIDTH, cols);

    if (students.empty())
    {
        printEmptyTableNotice(CYAN, STUDENT_TABLE_WIDTH, "No students found in this grade.");
        return;
    }

    for (int i = 0; i < (int)students.size(); i++)
    {
        ostringstream gpaStr;
        gpaStr << fixed << setprecision(2) << students[i]->getGpa();
        printStudentRow(CYAN, i + 1, students[i], gpaStr.str());
    }
    printTableDivider(CYAN, STUDENT_TABLE_WIDTH);
}
