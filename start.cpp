#include <iostream>
#include <limits>
#include "controller/Controller.h"
#include "SchoolConstants.h"
using namespace std;


static int readInt(const string& prompt) {
    while (true) {
        if (!prompt.empty()) cout << prompt;
        string line;
        getline(cin, line);
        bool valid = !line.empty();
        for (int i = 0; i < (int)line.size() && valid; i++)
            if (!isdigit(line[i])) valid = false;
        if (valid) return stoi(line);
        cout << "Invalid input! Please enter a whole number: ";
    }
}

static double readDouble(const string& prompt) {
    while (true) {
        if (!prompt.empty()) cout << prompt;
        string line;
        getline(cin, line);
        bool valid = !line.empty();
        bool hasDot = false;
        for (int i = 0; i < (int)line.size() && valid; i++) {
            if (line[i] == '.' && !hasDot) { hasDot = true; continue; }
            if (!isdigit(line[i])) valid = false;
        }
        if (valid)
          return stod(line);
        cout << "Invalid input! Please enter a number: ";
    }
}

static void saveAll(StudentRepositoryImpl& sRepo, CourseRepositoryImpl& cRepo, TeacherRepositoryImpl& tRepo) {

    bool ok = true;
    if (!sRepo.saveToFile(SchoolConstants::FILE_STUDENTS)) {
        cout << "[ERROR] Failed to save student to \"" << SchoolConstants::FILE_STUDENTS << "\".\n";
        ok = false;
    }
    if (!cRepo.saveToFile(SchoolConstants::FILE_COURSES)) {
        cout << "[ERROR] Failed to save course to \"" << SchoolConstants::FILE_COURSES << "\".\n";
        ok = false;
    }
    if (!tRepo.saveToFile(SchoolConstants::FILE_TEACHERS)) {
        cout << "[ERROR] Failed to save teacher to \"" << SchoolConstants::FILE_TEACHERS << "\".\n";
        ok = false;
    }
    if (ok)
        cout << "[Saved]\n";
}

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
        cout << "5- Show " << s << " Info\t\t6- Unassign Course from " << s << "\n";
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
       cout << "7- Replace Teacher in Course\n";
    }
    cout << "\n";
}

int main() {

    // ── Repositories ─────────────────────────────────────────────────────
    StudentRepositoryImpl studentRepoImpl;
    CourseRepositoryImpl courseRepoImpl;
    TeacherRepositoryImpl teacherRepoImpl;

    studentRepoImpl.loadFromFile(SchoolConstants::FILE_STUDENTS);
    courseRepoImpl.loadFromFile(SchoolConstants::FILE_COURSES);
    teacherRepoImpl.loadFromFile(SchoolConstants::FILE_TEACHERS);

    StudentRepository& studentRepo = studentRepoImpl;
    CourseRepository& courseRepo = courseRepoImpl;
    TeacherRepository& teacherRepo = teacherRepoImpl;

    // ── Validators ───────────────────────────────────────────────────────
    TeacherValidator teacherValidator;
    CourseValidator  courseValidator;
    StudentValidator studentValidator;

    // ── Services ─────────────────────────────────────────────────────────
    CourseServiceImpl courseServiceImpl(courseRepo, teacherRepo, courseValidator);
    CourseService& courseService = courseServiceImpl;

    TeacherServiceImpl teacherServiceImpl(teacherRepo, courseRepo, studentRepo, teacherValidator);
    TeacherService& teacherService = teacherServiceImpl;

    StudentServiceImpl studentServiceImpl(studentRepo, courseRepo, studentValidator);
    StudentService& studentService = studentServiceImpl;

    // ── Controllers ──────────────────────────────────────────────────────
    CourseController courseController(courseService, studentService);
    TeacherController teacherController(teacherService);
    StudentController studentController(studentService, courseService);

    displaySystem();

    int  process;
    bool flag = true;

    while (flag) {
        cout << "\nEnter your choice (1-4): ";
        process = readInt("");

        switch (process) {

            // ==================== STUDENT ====================
            case 1: {
                cout << "\n\t************ STUDENT BRANCH ************\n\n";
                showProcesses("Student");

                int studentProcess;
                studentProcess = readInt("");
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
                    grade = readInt("");
                    student.setGrade(grade);

                    cout << "Student Age: ";
                    int age;
                    age = readInt("");
                    student.setAge(age);

                    cout << "Student Phone Number (10-12 digits): ";
                    string phone;
                    getline(cin, phone);
                    student.setPhoneNumber(phone);
                    cout << "\n";

                    string result = studentController.addStudent(grade, student);
                    cout << result << "\n";
                    if (result.find("failed") == string::npos && result.find("exists") == string::npos && result.find("Invalid") == string::npos && result.find("Maximum") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                }
                // ── Remove Student ─────────────────────────────────────
                else if (studentProcess == 2) {
                    cout << "\nEnter Student ID to remove: ";
                    string id;
                    getline(cin, id);

                    Student* s = studentController.findStudentById(id);
                    if (!s) {
                        cout << "Student not found.\n";
                    } else {
                        cout << "\nYou are about to remove student: " << s->getName() << " (ID: " << s->getId() << ")\n";
                        cout << "Are you sure? (y/n): ";

                        char confirm;
                        cin >> confirm;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (confirm == 'y' || confirm == 'Y') {
                            string result = studentController.removeStudent(id);
                            cout << result << "\n";
                            if (result.find("not found") == string::npos)
                                saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                        } else {
                            cout << "Operation cancelled.\n";
                        }
                    }
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
                    grade = readInt("");
                    newData.setGrade(grade);

                    cout << "Age: ";
                    int age;
                    age = readInt("");
                    newData.setAge(age);

                    cout << "Phone Number: ";
                    string phone;
                    getline(cin, phone);
                    newData.setPhoneNumber(phone);

                    cout << "GPA: ";
                    double gpa;
                    gpa = readDouble("");
                    newData.setGpa(gpa);

                    cout << "\n";
                    string result = studentController.editStudent(id, newData);
                    cout << result << "\n";
                    if (result.find("not found") == string::npos && result.find("cannot") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                }
                // ── Assign Courses to Student ──────────────────────────
                else if (studentProcess == 4) {
                    cout << "\nEnter Student ID: ";
                    string studentId;
                    getline(cin, studentId);

                    Student* stu = studentController.findStudentById(studentId);
                    if (!stu) {
                        cout << "Student not found.\n";
                        break;
                    }
                    int maxCourses = courseController.getMaxCoursesForGrade(stu->getGrade());

                    cout << "How many courses to assign (1-" << maxCourses << ")? ";
                    int numCourses;
                    numCourses = readInt("");

                    if (numCourses < 1 || numCourses > maxCourses) {
                        cout << "Invalid number. Must be between 1 and " << maxCourses << ".\n";
                        break;
                    }

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
                                choice = readInt("");
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

                    string result = studentController.assignCoursesToStudent(studentId, courseIds, teacherNames);
                    cout << "\n" << result << "\n";
                    if (result.find("not found") == string::npos && result.find("failed") == string::npos &&
                        result.find("No courses") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
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
                courseProcess = readInt("");
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
                    grade = readInt("");
                    course.setGrade(grade);

                    cout << "Subject Hours (2-6): ";
                    int hours;
                    hours = readInt("");
                    course.setSubjectHours(hours);

                    cout << "Specialization: ";
                    string spec;
                    getline(cin, spec);
                    course.setSpecialization(spec);
                    cout << "\n";

                    string result = courseController.addCourse(grade, course);
                    cout << result << "\n";
                    if (result.find("cannot") == string::npos && result.find("exists") == string::npos &&
                        result.find("Invalid") == string::npos && result.find("Maximum") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                }
                // ── Remove Course ──────────────────────────────────────
                else if (courseProcess == 2) {
                    cout << "\nEnter Course ID to remove: ";
                    string id;
                    getline(cin, id);

                    Course* c = courseController.findCourseById(id);
                    if (!c) {
                        cout << "Course not found.\n";
                    }
                    else {
                        cout << "\nYou are about to remove course: " << c->getName() << " (ID: " << c->getId() << ")\n";
                        cout << "Are you sure? (y/n): ";

                        char confirm;
                        cin >> confirm;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (confirm == 'y' || confirm == 'Y') {
                            string result = courseController.removeCourse(id);
                            cout << result << "\n";
                            if (result.find("not found") == string::npos && result.find("Cannot") == string::npos)
                                saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                        }
                        else {
                            cout << "Operation cancelled.\n";
                        }
                    }
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
                    grade = readInt("");
                    newData.setGrade(grade);

                    cout << "Subject Hours (2-6): ";
                    int hours;
                    hours = readInt("");
                    newData.setSubjectHours(hours);

                    cout << "\n";
                    string result = courseController.editCourse(id, newData);
                    cout << result << "\n";
                    if (result.find("not found") == string::npos && result.find("cannot") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
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

                    string result = teacherController.replaceTeacherInCourse(courseId, oldTeacherId, newTeacherId);
                    cout << "\n" << result << "\n";
                    if (result.find("successfully") != string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                }
                break;
            }

            // ==================== TEACHER ====================
            case 3: {
                cout << "\n\t************ TEACHER BRANCH ************\n";
                showProcesses("Teacher");

                int teacherProcess;
                teacherProcess = readInt("");
                cout << "\n";

                if (teacherProcess < 1 || teacherProcess > 6) {
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
                    age = readInt("");
                    teacher.setAge(age);

                    cout << "Teacher experience years: ";
                    int ey;
                    ey = readInt("");
                    teacher.setExperienceYears(ey);

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
                    grade = readInt("");
                    teacher.setGrade(grade);

                    cout << "Monthly Salary: ";
                    double sal;
                    sal = readDouble("");
                    teacher.setMonthlySalary(sal);

                    cout << "\n";
                    string result = teacherController.addTeacher(grade, teacher);
                    cout << result << "\n";
                    if (result.find("cannot") == string::npos && result.find("exists") == string::npos &&
                        result.find("Invalid") == string::npos && result.find("Maximum") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
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
                            string result = teacherController.removeTeacher(id);
                            cout << result << "\n";
                            if (result.find("not found") == string::npos && result.find("Cannot") == string::npos)
                                saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
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
                    age = readInt("");
                    newData.setAge(age);

                    cout << "Teacher experience years: ";
                    int ey;
                    ey = readInt("");
                    newData.setExperienceYears(ey);

                    cout << "Teacher Grade (1-12): ";
                    int grade;
                    grade = readInt("");
                    newData.setGrade(grade);

                    cout << "Teacher monthly salary: ";
                    double sal;
                    sal = readDouble("");
                    newData.setMonthlySalary(sal);

                    cout << "Teacher Specialization: ";
                    string spec;
                    getline(cin, spec);
                    newData.setSpecialization(spec);

                    cout << "Teacher subject: ";
                    string sub;
                    getline(cin, sub);
                    newData.setSubject(sub);

                    cout << "\n";
                    string result = teacherController.editTeacher(id, newData);
                    cout << result << "\n";
                    if (result.find("not found") == string::npos && result.find("cannot") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                }
                // ── Assign Courses to Teacher ──────────────────────────
                else if (teacherProcess == 4) {
                    cout << "\nEnter Teacher ID to assign courses: ";
                    string teacherId;
                    getline(cin, teacherId);

                    if (!teacherController.findTeacherById(teacherId)) {
                        cout << "Teacher not found.\n";
                        break;
                    }

                    cout << "How many courses to assign (1-" << SchoolConstants::MAX_COURSES_PER_TEACHER << ")? ";
                    int numCourses;
                    numCourses = readInt("");

                    if (numCourses < 1 || numCourses > SchoolConstants::MAX_COURSES_PER_TEACHER) {
                        cout << "Invalid number. Must be between 1 and "
                             << SchoolConstants::MAX_COURSES_PER_TEACHER << ".\n";
                        break;
                    }

                    vector<string> courseIds;
                    for (int i = 0; i < numCourses; i++) {
                        cout << "Enter Course ID #" << (i + 1) << ": ";
                        string cid; getline(cin, cid);
                        courseIds.push_back(cid);
                    }

                    string result = teacherController.assignCoursesToTeacher(teacherId, courseIds);
                    cout << result << "\n";
                    if (result.find("failed") == string::npos && result.find("not found") == string::npos &&
                        result.find("cannot") == string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
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
                // ── Unassign Course from Teacher ───────────────────────
                else if (teacherProcess == 6) {
                    cout << "\nEnter Teacher ID: ";
                    string teacherId;
                    getline(cin, teacherId);

                    cout << "Enter Course ID to unassign: ";
                    string courseId;
                    getline(cin, courseId);

                    string result = teacherController.unassignCourseFromTeacher(teacherId, courseId);
                    cout << result << "\n";
                    if (result.find("successfully") != string::npos)
                        saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                }
                break;
            }

            case 4:
                cout << "\nSaving data...\n";
                saveAll(studentRepoImpl, courseRepoImpl, teacherRepoImpl);
                cout << "Exiting program...\n";
                flag = false;
                break;

            default:
                cout << "\nInvalid Choice. Please enter a number between 1-4.\n";
        }
    }

    return 0;
}
