#include "ui/UIHandlers.h"
#include "common/SchoolConstants.h"
#include "repositories/student/StudentRepository.h"
#include "repositories/course/CourseRepository.h"
#include "repositories/teacher/TeacherRepository.h"
#include "services/student/StudentService.h"
#include "services/course/CourseService.h"
#include "services/teacher/TeacherService.h"
#include "common/Validators.h"
using namespace std;

int main() {

    // ── Repositories ──────────────────────────────────────────────────────────
    StudentRepositoryImpl studentRepoImpl;
    CourseRepositoryImpl courseRepoImpl;
    TeacherRepositoryImpl teacherRepoImpl;

    studentRepoImpl.loadFromFile(SchoolConstants::FILE_STUDENTS);
    courseRepoImpl.loadFromFile(SchoolConstants::FILE_COURSES);
    teacherRepoImpl.loadFromFile(SchoolConstants::FILE_TEACHERS);

    StudentRepository& studentRepo = studentRepoImpl;
    CourseRepository& courseRepo = courseRepoImpl;
    TeacherRepository& teacherRepo = teacherRepoImpl;

    // ── Validators ────────────────────────────────────────────────────────────
    TeacherValidator teacherValidator;
    CourseValidator courseValidator;
    StudentValidator studentValidator;

    // ── Services ──────────────────────────────────────────────────────────────
    CourseServiceImpl courseServiceImpl(courseRepo, teacherRepo, courseValidator);
    TeacherServiceImpl teacherServiceImpl(teacherRepo, courseRepo, studentRepo, teacherValidator);
    StudentServiceImpl studentServiceImpl(studentRepo, courseRepo, studentValidator);

    CourseService& courseService = courseServiceImpl;
    TeacherService& teacherService = teacherServiceImpl;
    StudentService& studentService = studentServiceImpl;

    // ── Controllers ───────────────────────────────────────────────────────────
    CourseController  courseController(courseService, studentService, studentRepoImpl, courseRepoImpl, teacherRepoImpl);
    TeacherController teacherController(teacherService, studentRepoImpl, courseRepoImpl, teacherRepoImpl);
    StudentController studentController(studentService, courseService, studentRepoImpl, courseRepoImpl, teacherRepoImpl);

    displayMainMenu();

    bool running = true;

    while (running) {

        int choice = readInt("\nEnter your choice (1-4): ");

        switch (choice) {
            case 1:
                handleStudentOperations(studentController, courseController);
                break;

            case 2:
                handleCourseOperations(courseController, teacherController);
                break;

            case 3:
                handleTeacherOperations(teacherController, courseController);
                break;

            case 4:
                cout << "\nSaving data...\n"; teacherController.save();
                cout << "Exiting program...\n"; running = false;
                break;

            default:
               cout << "\nInvalid Choice. Please enter a number between 1-4.\n";
        }
    }

    return 0;
}
