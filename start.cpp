#include <iostream>
#include <limits>
#include "controller/Controller.h"
using namespace std;

void displaySystem() {
    cout << "\033[41m\033[37m\t\t********************* Welcome To The School System *********************\n\n\033[0m";
    cout << "The Process you need is:\n";
    cout << "1- About Student\t2- About Course\n";
    cout << "3- About Teacher\t4- Exit\n";
}

void showProcesses(const string& s) {
    cout << "Please enter the process you need!\n\n";
    if (s == "Teacher") {
        cout << "1- Add " << s << "\t\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t\t4- Assign Courses to " << s << "\n";
        cout << "5- Show " << s << " Info\n";
    }
    else if (s == "Student") {
        cout << "1- Add " << s << "\t\t\t2- Remove " << s << "\n";
        cout << "3- Edit " << s << "\t\t\t4- Assign Courses to " << s << "\n";
        cout << "5- Show " << s << " Info\n";
    }
    else if (s == "Course") {
       cout << "1- Add " << s << "\t\t2- Remove " << s << "\n";
       cout << "3- Edit " << s << "\t\t4- Show ALL Assigned Students\n";
       cout << "5- Show " << s << " Info\t6- Show Course students (By Teacher)\n";
       cout << "7- Replace Teacher in Course\n";  // NEW
    }

    cout << "\n";
}

int main() {

    // ── Infrastructure (repositories) ────────────────────────────────────
    StudentRepositoryImpl studentRepoImpl;
    CourseRepositoryImpl  courseRepoImpl;
    TeacherRepositoryImpl teacherRepoImpl;

    StudentRepository& studentRepo = studentRepoImpl;
    CourseRepository&  courseRepo  = courseRepoImpl;
    TeacherRepository& teacherRepo = teacherRepoImpl;

    // ── Validators ───────────────────────────────────────────────────────
    TeacherValidator teacherValidator;
    CourseValidator  courseValidator;
    StudentValidator studentValidator;

    // ── Services ─────────────────────────────────────────────────────────
    CourseServiceImpl  courseServiceImpl(courseRepo, courseValidator);
    CourseService&     courseService = courseServiceImpl;

    TeacherServiceImpl teacherServiceImpl(teacherRepo, courseRepo, studentRepo, teacherValidator);
    TeacherService&    teacherService = teacherServiceImpl;

    StudentServiceImpl studentServiceImpl(studentRepo, courseRepo, studentValidator);
    StudentService&    studentService = studentServiceImpl;

    // ── Controllers ──────────────────────────────────────────────────────
    CourseController  courseController(courseService, studentService);
    TeacherController teacherController(teacherService);
    StudentController studentController(studentService, courseService);

    displaySystem();

    int  process;
    bool flag = true;

    while (flag) {
        cout << "\nEnter your choice (1-4): ";
        cin >> process;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (process) {

            // ==================== STUDENT ====================
            case 1: {
                cout << "\n\t************ STUDENT BRANCH ************\n\n";
                showProcesses("Student");

                int studentProcess;
                cin >> studentProcess;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n";

                if (studentProcess < 1 || studentProcess > 5) {
                    cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN\n";
                    break;
                }

                // ── Add Student ────────────────────────────────────────
                if (studentProcess == 1) {
                    Student student;
                    cout << "- Please enter the required student information below:\n";

                    cout << "Student Full Name: ";
                    string name;
                    getline(cin, name);
                    student.setName(name);

                    cout << "Student National Number: ";
                    string nn;
                    getline(cin, nn);
                    student.setNationalNumber(nn);

                    cout << "Student School Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    student.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Student Age: ";
                    int age;
                    cin >> age;
                    student.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Student Phone Number (10-12 digits): ";
                    string phone;
                    getline(cin, phone);
                    student.setPhoneNumber(phone);
                    cout << "\n";

                    cout << studentController.addStudent(grade, student) << "\n";
                }
                // ── Remove Student ─────────────────────────────────────
                else if (studentProcess == 2) {
                    cout << "Remove Student is not yet supported in this version.\n";
                }
                // ── Edit Student ───────────────────────────────────────
                else if (studentProcess == 3) {
                    cout << "\nEnter Student ID to edit: ";
                    string id;
                    getline(cin, id);
                    if (!studentController.findStudentById(id)) {
                        cout << "Student not found.\n";
                        break;
                    }

                    Student newData;
                    cout << "------- Enter NEW DATA : -------\n";

                    cout << "Full Name: ";
                    string name;
                    getline(cin, name);
                    newData.setName(name);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    newData.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Age: ";
                    int age;
                    cin >> age;
                    newData.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Phone Number: ";
                    string phone;
                    getline(cin, phone);
                    newData.setPhoneNumber(phone);

                    cout << "GPA: ";
                    double gpa;
                    cin >> gpa;
                    newData.setGpa(gpa);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "\n";
                    cout << studentController.editStudent(id, newData) << "\n";
                }
                // ── Assign Courses to Student ──────────────────────────
                else if (studentProcess == 4) {
                    cout << "\nEnter Student ID: ";
                    string studentId;
                    getline(cin, studentId);

                    cout << "How many courses to assign? ";
                    int numCourses;
                    cin >> numCourses;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    vector<string> courseIds, teacherNames;

                    for (int i = 0; i < numCourses; i++) {
                        cout << "\n--- Course #" << (i + 1) << " ---\n";
                        cout << "Enter Course ID: ";
                        string cid;
                        getline(cin, cid);

                        vector<string> av = courseController.getCourseTeacherNames(cid);

                        if (!av.empty()) {
                            cout << "Available teachers:\n";
                            for (int j = 0; j < (int)av.size(); j++)
                                cout << (j + 1) << ". " << av[j] << "\n";

                            string sel;
                            if (av.size() == 1) {
                                sel = av[0];
                                cout << "Auto-selected: " << sel << "\n";
                            } else {
                                cout << "Select teacher number: ";
                                int choice;
                                cin >> choice;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                if (choice >= 1 && choice <= (int)av.size())
                                    sel = av[choice - 1];
                            }
                            courseIds.push_back(cid);
                            teacherNames.push_back(sel);
                        } else {
                            courseIds.push_back(cid);
                            teacherNames.push_back("");
                        }
                    }

                    cout << "\n" << studentController.assignCoursesToStudent(studentId, courseIds, teacherNames) << "\n";
                }
                // ── Show Student ───────────────────────────────────────
                else if (studentProcess == 5) {
                    cout << "\nEnter Student ID: ";
                    string id; getline(cin, id);
                    if (!studentController.findStudentById(id)) {
                        cout << "Student not found.\n";
                        break;
                    }
                    studentController.showStudent(id);
                }
                break;
            }

            // ==================== COURSE ====================
            case 2: {
                cout << "\n\t************ COURSE BRANCH ************\n\n";
                showProcesses("Course");

                int courseProcess;
                cin >> courseProcess;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n";

                if (courseProcess < 1 || courseProcess > 7) {
                    cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN\n";
                    break;
                }

                // ── Add Course ─────────────────────────────────────────
                if (courseProcess == 1) {
                    Course course;
                    cout << "- Please Enter Course Data:\n";

                    cout << "Course Name: ";
                    string name;
                    getline(cin, name);
                    course.setName(name);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    course.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Subject Hours (2-6): ";
                    int hours;
                    cin >> hours;
                    course.setSubjectHours(hours);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Specialization: ";
                    string spec;
                    getline(cin, spec);
                    course.setSpecialization(spec);
                    cout << "\n";

                    cout << courseController.addCourse(grade, course) << "\n";
                }
                // ── Remove Course ──────────────────────────────────────
                else if (courseProcess == 2) {
                    cout << "Remove Course is not yet supported in this version.\n";
                }
                // ── Edit Course ────────────────────────────────────────
                else if (courseProcess == 3) {
                    cout << "\nEnter Course ID to edit: ";
                    string id;
                    getline(cin, id);
                    if (!courseController.findCourseById(id)) {
                        cout << "Course not found.\n";
                        break;
                    }

                    Course newData;
                    cout << "------- Enter NEW DATA : -------\n";

                    cout << "Course Name: ";
                    string name;
                    getline(cin, name);
                    newData.setName(name);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    newData.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Subject Hours (2-6): ";
                    int hours;
                    cin >> hours;
                    newData.setSubjectHours(hours);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "\n";
                    cout << courseController.editCourse(id, newData) << "\n";
                }
                // ── Show All Assigned Students ─────────────────────────
                else if (courseProcess == 4) {
                    cout << "\nEnter Course ID: ";
                    string courseId; getline(cin, courseId);
                    if (!courseController.findCourseById(courseId)) {
                        cout << "Course not found.\n";
                        break;
                    }
                    courseController.showCourseStudents(courseId);
                }
                // ── Show Course Info ───────────────────────────────────
                else if (courseProcess == 5) {
                    cout << "\nEnter Course ID: ";
                    string id; getline(cin, id);
                    if (!courseController.findCourseById(id)) {
                        cout << "Course not found.\n";
                        break;
                    }
                    courseController.showCourse(id);
                }
                // ── Show Course Students by Teacher ────────────────────
                else if (courseProcess == 6) {
                    cout << "\nEnter Course ID: ";
                    string courseId; getline(cin, courseId);
                    if (!courseController.findCourseById(courseId)) {
                        cout << "Course not found.\n";
                        break;
                    }
                    courseController.showCourseStudentsByTeacher(courseId);
                }
                // ── Replace Teacher in Course ──────────────────────────
                else if (courseProcess == 7) {
                    cout << "\nEnter Course ID: ";
                    string courseId;
                    getline(cin, courseId);
                    if (!courseController.findCourseById(courseId)) {
                        cout << "Course not found.\n";
                        break;
                    }
                    cout << "Enter OLD Teacher ID: ";
                    string oldTeacherId;
                    getline(cin, oldTeacherId);

                    cout << "Enter NEW Teacher ID: ";
                    string newTeacherId;
                    getline(cin, newTeacherId);

                    cout << "\n" << teacherController.replaceTeacherInCourse(courseId, oldTeacherId, newTeacherId) << "\n";
            }
                break;
            }

            // ==================== TEACHER ====================
            case 3: {
                cout << "\n\t************ TEACHER BRANCH ************\n";
                showProcesses("Teacher");

                int teacherProcess;
                cin >> teacherProcess;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n";

                if (teacherProcess < 1 || teacherProcess > 5) {
                    cout << "ERROR! INVALID OPTION PLEASE TRY AGAIN\n";
                    break;
                }

                // ── Add Teacher ────────────────────────────────────────
                if (teacherProcess == 1) {
                    Teacher teacher;
                    cout << "- Please Enter Teacher Data:\n";

                    cout << "Teacher Name: ";
                    string name;
                    getline(cin, name);
                    teacher.setName(name);

                    cout << "Teacher National Number: ";
                    string nn;
                    getline(cin, nn);
                    teacher.setNationalNumber(nn);

                    cout << "Teacher Age: ";
                    int age;
                    cin >> age;
                    teacher.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher experience years: ";
                    int ey;
                    cin >> ey;
                    teacher.setExperienceYears(ey);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher Specialization: ";
                    string spec;
                    getline(cin, spec);
                    teacher.setSpecialization(spec);

                    cout << "Teacher Subject: ";
                    string sub;
                    getline(cin, sub);
                    teacher.setSubject(sub);

                    cout << "Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    teacher.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Monthly Salary: ";
                    double sal;
                    cin >> sal;
                    teacher.setMonthlySalary(sal);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "\n";
                    cout << teacherController.addTeacher(grade, teacher) << "\n";
                }
                // ── Remove Teacher ─────────────────────────────────────
                else if (teacherProcess == 2) {
                    cout << "\nEnter Teacher ID to remove: ";
                    string id;
                    getline(cin, id);

                    Teacher* t = teacherController.findTeacherById(id);
                    if (!t) {
                        cout << "Teacher not found.\n";
                    } else {
                        cout << "\nYou are about to remove teacher: " << t->getName() << " (ID: " << t->getId() << ")\n";
                        cout << "This will unassign the teacher from any courses that currently have no students enrolled.\n";
                        cout << "Are you sure? (y/n): ";

                        char confirm;
                        cin >> confirm;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (confirm == 'y' || confirm == 'Y') {
                            cout << teacherController.removeTeacher(id) << "\n";
                        } else {
                            cout << "Operation cancelled.\n";
                        }
                    }
                }
                // ── Edit Teacher ───────────────────────────────────────
                else if (teacherProcess == 3) {
                    cout << "\nEnter Teacher Id to edit: ";
                    string id;
                    getline(cin, id);
                    if (!teacherController.findTeacherById(id)) {
                        cout << "Teacher not found.\n";
                        break;
                    }

                    Teacher newData;
                    cout << "------- Enter NEW DATA : -------\n";

                    cout << "Teacher Name: ";
                    string name;
                    getline(cin, name);
                    newData.setName(name);

                    cout << "Teacher Age: ";
                    int age;
                    cin >> age;
                    newData.setAge(age);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher experience years: ";
                    int ey;
                    cin >> ey;
                    newData.setExperienceYears(ey);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher Grade (1-12): ";
                    int grade;
                    cin >> grade;
                    newData.setGrade(grade);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher monthly salary: ";
                    double sal;
                    cin >> sal;
                    newData.setMonthlySalary(sal);
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Teacher Specialization: ";
                    string spec;
                    getline(cin, spec);
                    newData.setSpecialization(spec);

                    cout << "Teacher subject: ";
                    string sub;
                    getline(cin, sub);
                    newData.setSubject(sub);

                    cout << "\n";
                    cout << teacherController.editTeacher(id, newData) << "\n";
                }
                // ── Assign Courses to Teacher ──────────────────────────
                else if (teacherProcess == 4) {
                    cout << "\nEnter Teacher ID to assign courses: ";
                    string teacherId;
                    getline(cin, teacherId);

                    cout << "How many courses to assign (1-3)? ";
                    int numCourses;
                    cin >> numCourses;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (numCourses < 1 || numCourses > 3) {
                        cout << "Invalid number.\n";
                        break;
                    }

                    vector<string> courseIds;
                    for (int i = 0; i < numCourses; i++) {
                        cout << "Enter Course ID #" << (i + 1) << ": ";
                        string cid; getline(cin, cid);
                        courseIds.push_back(cid);
                    }

                    cout << teacherController.assignCoursesToTeacher(teacherId, courseIds) << "\n";
                }
                // ── Show Teacher Info ──────────────────────────────────
                else if (teacherProcess == 5) {
                    cout << "\nEnter Teacher ID: ";
                    string id;
                    getline(cin, id);
                    if (!teacherController.findTeacherById(id)) {
                        cout << "Teacher not found.\n";
                        break;
                    }
                    teacherController.showTeacher(id);
                }
                break;
            }

            case 4:
                cout << "\nExiting program...\n";
                flag = false;
                break;

            default:
                cout << "\nInvalid Choice. Please enter a number between 1-4.\n";
        }
    }

    return 0;
}
