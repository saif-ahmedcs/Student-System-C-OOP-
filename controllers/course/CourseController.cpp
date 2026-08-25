#include "CourseController.h"
#include "../../common/SaveManager.h"
#include "../../common/TablePrinter.h"
#include <limits>
using namespace std;

CourseController::CourseController(CourseService& cSrv, StudentService& sSrv, StudentRepository& sRepo, CourseRepository& cRepo, TeacherRepository& tRepo)
    : courseService(cSrv), studentService(sSrv), studentRepo(sRepo), courseRepo(cRepo), teacherRepo(tRepo) {}

void CourseController::save() const {
    saveAll(studentRepo, courseRepo, teacherRepo);
}

Course* CourseController::findCourseById(const string& id) {
    return courseService.findCourseById(id);
}

vector<string> CourseController::getCourseTeacherNames(const string& courseId) {
    Course* c = courseService.findCourseById(courseId);
    if (!c)
    {
        return vector<string>();
    }
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

string CourseController::removeCourse(const string& id) {
    return courseService.removeCourse(id);
}

void CourseController::showCourse(const string& id) {
    Course* c = courseService.findCourseById(id);
    if (!c)
    {
        cout << "Course not found.\n";
        return;
    }

    cout << "-----------------------------------\n";
    cout << "Course Name: " << c->getName() << "\n";
    cout << "Course ID: " << c->getId() << "\n";
    cout << "Grade: " << c->getGrade() << "\n";
    cout << "Subject Hours: " << c->getSubjectHours() << "\n";

    const vector<string>& teachers = c->getTeacherNames();
    if (teachers.empty())
    {
        cout << "Teachers: NONE\n";
    }
    else
    {
        cout << "Teachers: ";
        for (int i = 0; i < (int)teachers.size(); i++)
        {
            cout << teachers[i];
            if (i != (int)teachers.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << "\n";
    }
    cout << "-----------------------------------\n";
}

static const string MAGENTA = "\033[35m";
static const int COURSE_STUDENTS_TABLE_WIDTH = 50;
static const int COURSE_LIST_TABLE_WIDTH = 74;

static void printStudentTableRow(const string& color, int number, Student* s) {
    printTableCell(color, 5);  cout << number;
    printTableCell(color, 25); cout << s->getName();
    printTableCell(color, 13); cout << s->getId();
    printTableRowEnd(color);
}

void CourseController::showCourseStudents(const string& courseId) {
    Course* c = courseService.findCourseById(courseId);
    if (!c)
    {
        cout << "Course not found.\n";
        return;
    }

    const vector<string>& assigned = c->getAssignedStudents();

    cout << "\n" << MAGENTA << "==================================================" << "\033[0m\n";
    cout << left << setw(14) << "Course Name" << ": " << c->getName()  << "\n";
    cout << left << setw(14) << "Course ID"   << ": " << c->getId()    << "\n";
    cout << left << setw(14) << "Grade"       << ": " << c->getGrade() << "\n";
    cout << MAGENTA << "==================================================" << "\033[0m\n";

    if (assigned.empty())
    {
        cout << "No students assigned in this course yet.\n";
        cout << MAGENTA << "==================================================" << "\033[0m\n";
        return;
    }

    cout << "\nTotal Assigned Students: " << assigned.size() << "\n";

    vector<TableColumn> cols = {
        {"No.",          5},
        {"Student Name", 25},
        {"Student ID",   13}
    };
    printTableHeader(MAGENTA, COURSE_STUDENTS_TABLE_WIDTH, cols);

    for (int i = 0; i < (int)assigned.size(); i++)
    {
        Student* s = studentService.findStudentById(assigned[i]);
        if (s)
        {
            printStudentTableRow(MAGENTA, i + 1, s);
        }
    }
    printTableDivider(MAGENTA, COURSE_STUDENTS_TABLE_WIDTH);
}

void CourseController::showCourseStudentsByTeacher(const string& courseId) {
    Course* c = courseService.findCourseById(courseId);
    if (!c)
    {
        cout << "Course not found.\n";
        return;
    }

    const vector<string>& teachers = c->getTeacherNames();
    if (teachers.empty())
    {
        cout << "No teachers assigned to this course.\n";
        return;
    }

    string selectedTeacher;
    if (teachers.size() == 1)
    {
        selectedTeacher = teachers[0];
    }
    else
    {
        cout << "\nTeachers for " << c->getName() << ":\n";
        for (int i = 0; i < (int)teachers.size(); i++)
        {
            cout << (i + 1) << ". " << teachers[i] << "\n";
        }

        cout << "\nSelect teacher number (1-" << teachers.size() << "): ";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > (int)teachers.size())
        {
            cout << "Invalid choice.\n";
            return;
        }
        selectedTeacher = teachers[choice - 1];
    }

    cout << "\n" << MAGENTA << "==================================================" << "\033[0m\n";
    cout << left << setw(14) << "Course Name"  << ": " << c->getName()    << "\n";
    cout << left << setw(14) << "Teacher Name" << ": " << selectedTeacher << "\n";
    cout << MAGENTA << "==================================================" << "\033[0m\n";

    const vector<string>& allStudents = c->getAssignedStudents();
    vector<Student*> result;

    for (int i = 0; i < (int)allStudents.size(); i++)
    {
        Student* s = studentService.findStudentById(allStudents[i]);
        if (!s)
        {
            continue;
        }
        const vector<StudentCourse>& sc = s->getAssignedCourses();
        for (int j = 0; j < (int)sc.size(); j++)
        {
            if (sc[j].courseId == c->getId() && sc[j].teacherName == selectedTeacher)
            {
                result.push_back(s);
                break;
            }
        }
    }

    if (result.empty())
    {
        cout << "No students registered with this teacher.\n";
        cout << MAGENTA << "==================================================" << "\033[0m\n";
        return;
    }

    cout << "\nRegistered Students (" << result.size() << "):\n";

    vector<TableColumn> cols = {
        {"No.",          5},
        {"Student Name", 25},
        {"Student ID",   13}
    };
    printTableHeader(MAGENTA, COURSE_STUDENTS_TABLE_WIDTH, cols);

    for (int i = 0; i < (int)result.size(); i++)
    {
        printStudentTableRow(MAGENTA, i + 1, result[i]);
    }
    printTableDivider(MAGENTA, COURSE_STUDENTS_TABLE_WIDTH);
}

void CourseController::listCoursesByGrade(int grade) {
    vector<Course> all = courseRepo.getCoursesInSchoolVector();

    cout << "Courses in Grade " << grade << "\n";

    vector<TableColumn> cols = {
        {"No.",        5},
        {"Course Name",25},
        {"Course ID",  20},
        {"Students",   10},
        {"Teachers",   10}
    };
    printTableHeader(MAGENTA, COURSE_LIST_TABLE_WIDTH, cols);

    int count = 0;
    for (int i = 0; i < (int)all.size(); i++)
    {
        if (all[i].getGrade() != grade)
        {
            continue;
        }
        count++;
        printTableCell(MAGENTA, 5);  cout << count;
        printTableCell(MAGENTA, 25); cout << all[i].getName();
        printTableCell(MAGENTA, 20); cout << all[i].getId();
        printTableCell(MAGENTA, 10); cout << all[i].getNumberOfAssignedStudents();
        printTableCell(MAGENTA, 10); cout << all[i].getNumberOfTeachers();
        printTableRowEnd(MAGENTA);
    }

    if (count == 0)
    {
        cout << "No courses found in this grade.\n";
    }
    printTableDivider(MAGENTA, COURSE_LIST_TABLE_WIDTH);
}
