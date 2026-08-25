#include "TeacherController.h"
#include "../../common/SaveManager.h"
#include "../../common/TablePrinter.h"
#include <iomanip>
#include <map>
using namespace std;

TeacherController::TeacherController(TeacherService& service, StudentRepository& sRepo, CourseRepository& cRepo, TeacherRepository& tRepo)
    : teacherService(service), studentRepo(sRepo), courseRepo(cRepo), teacherRepo(tRepo) {}

void TeacherController::save() const {
    saveAll(studentRepo, courseRepo, teacherRepo);
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

string TeacherController::assignCoursesToTeacher(const string& teacherId, const vector<string>& courseIds, const vector<vector<int>>& courseClasses) {
    return teacherService.assignCoursesToTeacher(teacherId, courseIds, courseClasses);
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
    cout << "Specialization: " << t->getSpecialization() << "\n";
    cout << "Grade: " << t->getGrade() << "\n";

    const vector<string>& courses = t->getAssignedCourses();
    if (courses.empty())
    {
        cout << "Assigned Courses: NONE\n";
    }
    else
    {
        cout << "Assigned Courses:\n";
        for (int i = 0; i < (int)courses.size(); i++)
        {
            cout << "  - " << courses[i];
            Course* c = courseRepo.findCourseById(courses[i]);
            if (c)
            {
                const map<int, string>& ctm = c->getClassTeacherMap();
                vector<int> myClasses;
                for (map<int, string>::const_iterator it = ctm.begin(); it != ctm.end(); ++it)
                {
                    if (it->second == t->getId())
                    {
                        myClasses.push_back(it->first);
                    }
                }
                if (!myClasses.empty())
                {
                    cout << " (Classes: ";
                    for (int j = 0; j < (int)myClasses.size(); j++)
                    {
                        cout << c->getGrade() << "/" << myClasses[j];
                        if (j != (int)myClasses.size() - 1)
                        {
                            cout << ", ";
                        }
                    }
                    cout << ")";
                }
            }
            cout << "\n";
        }
    }

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

static const string YELLOW = "\033[33m";
static const int TEACHER_TABLE_WIDTH = 69;

void TeacherController::listTeachersByGrade(int grade) {
    vector<Teacher*> teachers = teacherRepo.getTeachersByGrade(grade);

    cout << "Teachers in Grade " << grade << " (" << teachers.size() << ")\n";

    vector<TableColumn> cols = {
        {"No.",          5},
        {"Teacher Name", 25},
        {"Teacher ID",   13},
        {"Students",     10}
    };
    printTableHeader(YELLOW, TEACHER_TABLE_WIDTH, cols);

    if (teachers.empty())
    {
        printEmptyTableNotice(YELLOW, TEACHER_TABLE_WIDTH, "No teachers found in this grade.");
        return;
    }

    vector<Student*> gradeStudents = studentRepo.getStudentsByGrade(grade);

    for (int i = 0; i < (int)teachers.size(); i++)
    {
        int counter = 0;
        for (int j = 0; j < (int)gradeStudents.size(); j++)
        {
            const vector<StudentCourse>& sc = gradeStudents[j]->getAssignedCourses();
            for (int k = 0; k < (int)sc.size(); k++)
            {
                if (sc[k].teacherName == teachers[i]->getName())
                {
                    counter++;
                    break;
                }
            }
        }

        printTableCell(YELLOW, 5);  cout << (i + 1);
        printTableCell(YELLOW, 25); cout << teachers[i]->getName();
        printTableCell(YELLOW, 13); cout << teachers[i]->getId();
        printTableCell(YELLOW, 10); cout << counter;
        printTableRowEnd(YELLOW);
    }
    printTableDivider(YELLOW, TEACHER_TABLE_WIDTH);
}
