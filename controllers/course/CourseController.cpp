#include "CourseController.h"
#include <limits>
using namespace std;

CourseController::CourseController(CourseService& cSrv, StudentService& sSrv, StudentRepository& sRepo, CourseRepository& cRepo, TeacherRepository& tRepo)
    : courseService(cSrv), studentService(sSrv), studentRepo(sRepo), courseRepo(cRepo), teacherRepo(tRepo) {}

void CourseController::save() const {
    bool ok = true;
    if (!studentRepo.saveToFile(SchoolConstants::FILE_STUDENTS)) {
        cout << "[ERROR] Failed to save student data.\n";
        ok = false;
    }
    if (!courseRepo.saveToFile(SchoolConstants::FILE_COURSES)) {
        cout << "[ERROR] Failed to save course data.\n";
        ok = false;
    }
    if (!teacherRepo.saveToFile(SchoolConstants::FILE_TEACHERS)) {
        cout << "[ERROR] Failed to save teacher data.\n";
        ok = false;
    }
    if (ok)
    {
        cout << "[Saved]\n";
    }
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

void CourseController::showCourseStudents(const string& courseId) {
    Course* c = courseService.findCourseById(courseId);
    if (!c)
    {
        cout << "Course not found.\n";
        return;
    }

    const vector<string>& assigned = c->getAssignedStudents();

    cout << "\n\033[35m==================================================\033[0m\n";
    cout << left << setw(14) << "Course Name" << ": " << c->getName()  << "\n";
    cout << left << setw(14) << "Course ID"   << ": " << c->getId()    << "\n";
    cout << left << setw(14) << "Grade"       << ": " << c->getGrade() << "\n";
    cout << "\033[35m==================================================\033[0m\n";

    if (assigned.empty())
    {
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

    for (int i = 0; i < (int)assigned.size(); i++)
    {
        Student* s = studentService.findStudentById(assigned[i]);
        if (s)
        {
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

    cout << "\n\033[35m==================================================\033[0m\n";
    cout << left << setw(14) << "Course Name"  << ": " << c->getName()    << "\n";
    cout << left << setw(14) << "Teacher Name" << ": " << selectedTeacher << "\n";
    cout << "\033[35m==================================================\033[0m\n";

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

    for (int i = 0; i < (int)result.size(); i++)
    {
        cout << "\033[35m|\033[0m " << left << setw(5)  << (i + 1)
             << "\033[35m|\033[0m " << left << setw(25) << result[i]->getName()
             << "\033[35m|\033[0m " << left << setw(13) << result[i]->getId()
             << "\033[35m|\033[0m\n";
    }
    cout << "\033[35m--------------------------------------------------\033[0m\n";
}

void CourseController::listCoursesByGrade(int grade) {
    vector<Course> all = courseRepo.getCoursesInSchoolVector();

    cout << "Courses in Grade " << grade << "\n";
    cout << "\033[35m----------------------------------------------------------\033[0m\n";
    cout << "\033[35m|\033[0m " << left << setw(5)  << "No."
         << "\033[35m|\033[0m " << left << setw(25) << "Course Name"
         << "\033[35m|\033[0m " << left << setw(10) << "Students"
         << "\033[35m|\033[0m " << left << setw(10) << "Teachers"
         << "\033[35m|\033[0m\n";
    cout << "\033[35m--------------------------------------------------\033[0m\n";

    int count = 0;
    for (int i = 0; i < (int)all.size(); i++)
    {
        if (all[i].getGrade() != grade)
        {
            continue;
        }
        count++;
        cout << "\033[35m|\033[0m " << left << setw(5)  << count
             << "\033[35m|\033[0m " << left << setw(25) << all[i].getName()
             << "\033[35m|\033[0m " << left << setw(10) << all[i].getNumberOfAssignedStudents()
             << "\033[35m|\033[0m " << left << setw(10) << all[i].getNumberOfTeachers()
             << "\033[35m|\033[0m\n";
    }

    if (count == 0)
    {
        cout << "No courses found in this grade.\n";
    }
    cout << "\033[35m--------------------------------------------------\033[0m\n";
}
